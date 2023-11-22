import xml.etree.ElementTree as ElementTree
import os
import json

from data_path import data_folder, output_folder
from errors import DataError
from level import Level, read_level_data
from tile_maker import get_image_as_bytes
from tileset import Tile, TileSet, read_tileset_data
from level import get_position_from_coordinates


class CompactIdInformation:
    def __init__(self, level_data):
        self.min_x = min([coordinates[0] for coordinates in level_data.keys()])
        self.max_x = max([coordinates[0] for coordinates in level_data.keys()])
        self.min_y = min([coordinates[1] for coordinates in level_data.keys()])

    def get_id_from_coordinates(self, coordinates):
        return ((coordinates[0] - self.min_x) +
                (coordinates[1] - self.min_y) *
                (self.max_x - self.min_x + 1))


def get_coordinates_from_filename(filename) -> tuple:
    x = int(filename[6])
    y = int(filename[7])
    return x, y


def get_world_map(world_map_filename):
    print("Reading world map")

    try:
        with open(world_map_filename, 'r') as f:
            world_map = json.load(f)
    except FileNotFoundError:
        print("File not found:", world_map)
        exit(1)
    return world_map


def get_all_level_filenames(world_map):
    level_filenames = []

    # Iterate on world_map on the "maps" array
    for level_info in world_map['maps']:
        filename = level_info['fileName']
        level_filenames.append(filename)

    return level_filenames


def compress_level_graph(level_graph):
    # Returns a compressed stream from the level graph data.
    # The input is 100 bytes long, the output is RLE encoded in the following form
    # If a byte is repeated, then the byte is replaced by a the repeat count with the 7th bit set
    # Else, the byte is presented as is

    result = []
    current_byte = level_graph[0]
    current_count = 1
    for byte in level_graph[1:]:
        if byte == current_byte:
            current_count += 1
        else:
            if current_count > 1:
                result.append(current_count | 0x80)
                result.append(current_byte)
            else:
                result.append(current_byte)
            current_byte = byte
            current_count = 1
    if current_count > 1:
        result.append(current_count | 0x80)
        result.append(current_byte)
    else:
        result.append(current_byte)
    return result


def get_c_data_for_level_graph(level: Level, tileset: TileSet):
    result = ''
    label = f"level_{level.compact_id}_graph"

    result += f"// Level coordinates: {level.get_coordinates()}\n"
    result += f"const unsigned char {label}[] = {{\n    "

    input_stream = [tileset.tiles[tile_id].compact_id for tile_id in level.data_graph]

    compressed_graph_data = compress_level_graph(input_stream)
    for i, data in enumerate(compressed_graph_data):
        result += f"0x{data:02x},"
        if (i + 1) % 10 == 0:
            result += "\n    "
        else:
            result += " "
    result += "\n};\n\n"

    return result, label


OBSTACLE_BIT = 1 << 0
DOOR_BIT = 1 << 1


def get_c_data_for_level_properties(level, tileset: TileSet):
    result = ''
    label = f"level_{level.compact_id}_properties"

    result += f"const unsigned char {label}[] = {{\n    "
    for i, tile_id in enumerate(level.data_graph):
        tile = tileset.tiles[tile_id]

        properties = 0
        if tile.is_obstacle():
            properties |= OBSTACLE_BIT
        if tile.is_door():
            properties |= DOOR_BIT

        result += f"0x{properties:02x},"
        if (i + 1) % 10 == 0:
            result += "\n    "
        else:
            result += " "
    result += "\n};\n\n"

    return result, label


def reference_level_graph_data(level_data, graph_labels):
    result = ''
    result += f"const unsigned char * const all_rooms[] = {{\n"

    max_compact_id = max(level.compact_id for level in level_data.values())

    for compact_id in range(0, max_compact_id + 1):
        if compact_id in graph_labels:
            result += f"    {graph_labels[compact_id]},\n"
        else:
            result += f"    NULL,\n"

    result += "};\n\n"
    result += f"const unsigned char level_data_count = {len(graph_labels)};\n\n"
    return result


def reference_level_property_data(level_data, property_labels):
    result = ''
    result += f"const unsigned char * const all_rooms_property[] = {{\n"

    max_compact_id = max(level.compact_id for level in level_data.values())

    for compact_id in range(0, max_compact_id + 1):
        if compact_id in property_labels:
            result += f"    {property_labels[compact_id]},\n"
        else:
            result += f"    NULL,\n"

    result += "};\n\n"
    return result


def output_level_data(level_data, tileset: TileSet, compact_id_info):
    graph_labels = {}
    property_labels = {}

    # Graph part
    result = '#ifndef LEVEL_DATA_H\n#define LEVEL_DATA_H\n\n'
    result += '#include <stddef.h>\n\n'

    print(f"Outputting {len(level_data)} levels")
    for level in level_data.values():
        c_output, label = get_c_data_for_level_graph(level, tileset)
        result += c_output
        graph_labels[level.compact_id] = label

    result += reference_level_graph_data(level_data, graph_labels)

    # Write information to be able to deduce the compact id from the coordinates
    result += f"const unsigned char level_data_min_x = {compact_id_info.min_x};\n"
    result += f"const unsigned char level_data_min_y = {compact_id_info.min_y};\n"
    result += f"const unsigned char level_data_max_x = {compact_id_info.max_x};\n"

    result += "\n#endif\n"

    output_filename = os.path.join(output_folder, 'level_data.h')
    with open(output_filename, 'w') as f:
        f.write(result)


def read_rooms(tiled_path, world_map):
    level_filenames = get_all_level_filenames(world_map)
    level_data = {}
    for filename in level_filenames:
        print("Reading", filename)

        try:
            with open(os.path.join(tiled_path, filename), 'r') as f:
                coordinates = get_coordinates_from_filename(filename)
                tree = ElementTree.parse(f)
                level_data[coordinates] = read_level_data(coordinates, tree)
        except FileNotFoundError:
            print("File not found:", filename)
            exit(1)
        except DataError as e:
            print(e)
            exit(1)
    return level_data


def get_all_used_tiles(level_data):
    all_used_tiles = set()
    for level in level_data.values():
        all_used_tiles.update(level.get_used_tiles())
    return all_used_tiles


def compute_compacted_tiles_id(level_data):
    # Collect all used tiles from all levels
    all_used_tiles = get_all_used_tiles(level_data)
    all_used_tiles = sorted(all_used_tiles)
    # Tile IDs are not contiguous and above 256 in numbers
    # We need to map them to contiguous indices
    tile_id_to_index = {}
    for index, tile_id in enumerate(all_used_tiles):
        tile_id_to_index[tile_id] = index
    return tile_id_to_index


def output_spawners(level_data, tileset, compact_tile_ids):
    # Get all the tiles that have a spawner property
    spawner_tiles = {}
    for tile in tileset.tiles.values():
        if 'spawn' in tile.properties:
            spawner_tiles[int(tile.id)] = tile.properties['spawn']

    print(f"Found {len(spawner_tiles)} spawner type of tiles")

    # In each level, identify the spawners tile from the data_spawn property
    # and register them with the compact id of the level and their position in the level
    spawners = []
    for level in level_data.values():
        for position, tile_id in enumerate(level.data_spawn):
            if tile_id in spawner_tiles.keys():
                spawners.append((level.compact_id, position, spawner_tiles[tile_id]))

    print(f"Outputting {len(spawners)} spawners")

    # Output the spawners
    result_h = '#ifndef SPAWNER_DATA_H\n#define SPAWNER_DATA_H\n\n'
    result_h += '#include <stddef.h>\n\n'

    # Output an enum for the all spawner types
    spawn_enum_values = {}
    result_h += f"typedef enum spawner_type {{\n"
    for tile_id, spawner_type in spawner_tiles.items():
        spawn_enum_name = f"E_SPAWN_{spawner_type.upper()}"
        spawn_enum_values[spawner_type] = spawn_enum_name
        result_h += f"    {spawn_enum_name} = {compact_tile_ids[tile_id]},\n"
    result_h += f"}} spawner_type_t;\n\n"
    result_h += f"extern const unsigned char spawners_count;\n"
    result_h += f"extern const unsigned char spawners[];\n\n"
    result_h += "#endif\n"

    result_c = '#include "spawner_data.h"\n\n'
    result_c += "/* level id, position, spawner_type*/\n"
    result_c += f"const unsigned char spawners[] = {{\n"
    for spawner in spawners:
        result_c += f"    {spawner[0]:3}, {spawner[1]:3}, {spawn_enum_values[spawner[2]]},\n"

    result_c += "\n};\n\n"
    result_c += f"const unsigned char spawners_count = {len(spawners)};\n\n"

    output_filename = os.path.join(output_folder, 'spawner_data.h')
    with open(output_filename, 'w') as f:
        f.write(result_h)

    output_filename = os.path.join(output_folder, 'spawner_data.c')
    with open(output_filename, 'w') as f:
        f.write(result_c)


TILE_WIDTH = 16
TILE_HEIGHT = 16


def image_as_c_data(tile_name, tile_image):
    red_as_bytes, green_as_bytes, blue_as_bytes = get_image_as_bytes(tile_image)
    label = tile_name

    # Do the same but limit the bytes to 8 per line
    result = f"const unsigned char {label}[] = {{\n    "
    for i, color in enumerate(blue_as_bytes):
        result += f"0x{color:02x}, "
        if (i + 1) % 8 == 0:
            result += "\n    "
    result += "\n    "
    for i, color in enumerate(red_as_bytes):
        result += f"0x{color:02x}, "
        if (i + 1) % 8 == 0:
            result += "\n    "
    result += "\n    "
    for i, color in enumerate(green_as_bytes):
        result += f"0x{color:02x}, "
        if (i + 1) % 8 == 0:
            result += "\n    "
    result += "\n};\n\n"

    return label, result


def output_tiles_graph(compact_tile_ids, tileset):
    tileset_width_in_tiles = tileset.image.width / TILE_WIDTH
    output_reference_folder = os.path.join(output_folder, "reference")
    os.makedirs(output_reference_folder, exist_ok=True)

    result = ''
    all_labels = []

    for tile_id, compact_tile_index in sorted(compact_tile_ids.items(), key=lambda x: x[1]):
        tile_coordinates = (tile_id % tileset_width_in_tiles * TILE_WIDTH,
                            tile_id // tileset_width_in_tiles * TILE_HEIGHT)
        tile_image = tileset.image.crop((tile_coordinates[0],
                                         tile_coordinates[1],
                                         tile_coordinates[0] + TILE_WIDTH,
                                         tile_coordinates[1] + TILE_HEIGHT))
        base_name = f'tile_{compact_tile_index:03}'
        output_filename = os.path.join(output_folder, "reference", f'{base_name}.png')

        tile_image.save(output_filename)

        label, c_data = image_as_c_data(base_name, tile_image)
        result += c_data
        all_labels.append(label)

    # Write an array of all tiles
    result += f"const unsigned char * const all_tiles[] = {{\n"
    for label in all_labels:
        result += f"    {label},\n"
    result += "};\n\n"

    output_filename = os.path.join(output_folder, 'tiles_data.h')
    with open(output_filename, 'w') as f:
        f.write(result)


def output_level_links(level_data):
    # Quick and dirty way to gather all links
    links = {}
    for level in level_data.values():
        for link_name, coordinates in level.links:
            link_id = int(link_name[1:])
            if link_name.startswith('E'):
                position = get_position_from_coordinates(coordinates)
                if link_id >= 253:
                    links[link_id] = (level.compact_id, position, 255, 0)  # Escape condition
                elif link_id >= 240:
                    links[link_id] = (level.compact_id, position, 254, 0)  # Victory condition
                else:
                    links[link_id] = (level.compact_id, position)
    for level in level_data.values():
        for link_name, coordinates in level.links:
            link_id = int(link_name[1:])
            if link_name.startswith('S'):
                position = get_position_from_coordinates(coordinates)
                links[link_id] += (level.compact_id, position,)

    # Now reorder the links by ascending source level_id
    links = [(link_data[0], link_data[1], link_data[2], link_data[3]) for link_data in links.values()]
    links = sorted(links, key=lambda x: x[0])

    print(f"Outputting {len(links)} links")

    result_h = '#ifndef LINKS_DATA_H\n#define LINKS_DATA_H\n\n'
    result_h += '#include <stddef.h>\n\n'

    result_h += f"extern const unsigned char links_count;\n"
    result_h += f"extern const unsigned char links[];\n\n"
    result_h += "#endif\n"

    result_c = '#include "links_data.h"\n\n'
    result_c += "/* level id, position, spawner_type*/\n"
    result_c += f"const unsigned char links[] = {{\n"
    for link in sorted(links, key=lambda x: x[0]):
        result_c += f"    {link[0]:3}, {link[1]:3}, {link[2]:3}, {link[3]},\n"

    result_c += "\n};\n\n"
    result_c += f"const unsigned char links_count = {len(links)};\n\n"

    output_filename = os.path.join(output_folder, 'links_data.h')
    with open(output_filename, 'w') as f:
        f.write(result_h)

    output_filename = os.path.join(output_folder, 'links_data.c')
    with open(output_filename, 'w') as f:
        f.write(result_c)


def output_doors(level_data, tileset):
    opened_lever_door_tile_id = 0
    closed_lever_door_tile_id = 0

    doors = []
    for level in level_data.values():
        for position, tile_id in enumerate(level.data_graph):
            if tileset.tiles[tile_id].is_door():
                # Only supports doors with keys at the moment
                tile = tileset.tiles[tile_id]
                properties = tile.properties

                door_property = 0
                if 'key' in properties:
                    key_id = int(tile.properties.get('key', 0))
                    door_property = tileset.tiles[key_id].compact_id
                if 'with_lever' in properties:
                    lever_name = level.get_lever_at(position)
                    if lever_name is not None:
                        lever_id = int(lever_name[1:])
                        door_property = lever_id
                        door_property |= 0x40

                    if 'opened' in properties:
                        is_opened = properties['opened'] == 'true'
                        if is_opened:
                            opened_lever_door_tile_id = tile.compact_id
                        else:
                            closed_lever_door_tile_id = tile.compact_id

                doors.append((level.compact_id, position, door_property))

    print(f"Outputting {len(doors)} doors")

    result_h = '#ifndef DOORS_DATA_H\n#define DOORS_DATA_H\n\n'
    result_h += '#include <stddef.h>\n\n'

    result_h += f"extern const unsigned char doors_count;\n"
    result_h += f"extern const unsigned char doors[];\n\n"

    result_h += f"extern const unsigned char opened_lever_door_tile_id;\n"
    result_h += f"extern const unsigned char closed_lever_door_tile_id;\n\n"

    result_h += "#endif\n"

    result_c = '#include "doors_data.h"\n\n'
    result_c += "/* level id, position, property */\n"
    result_c += f"const unsigned char doors[] = {{\n"
    for door in doors:
        result_c += f"    {door[0]:3}, {door[1]:3}, {door[2]:3},\n"

    result_c += "\n};\n\n"
    result_c += f"const unsigned char doors_count = {len(doors)};\n\n"

    result_c += f"const unsigned char opened_lever_door_tile_id = {opened_lever_door_tile_id};\n"
    result_c += f"const unsigned char closed_lever_door_tile_id = {closed_lever_door_tile_id};\n\n"

    output_filename = os.path.join(output_folder, 'doors_data.h')
    with open(output_filename, 'w') as f:
        f.write(result_h)

    output_filename = os.path.join(output_folder, 'doors_data.c')
    with open(output_filename, 'w') as f:
        f.write(result_c)


OBJECT_PROPERTIES = {
    'PICKABLE': 0x01,
    'KEY': 0x02,
    'HP': 0x04,
    'LEVER': 0x08,
    'WEAPON_1': 0x10,
    'WEAPON_2': 0x20,
    'CAULDRON': 0x40,
}


def output_objects(level_data, tileset):
    lever_deactivated_tile_id = 0
    lever_activated_tile_id = 0
    hidden_treasure_tile_id = 0

    # Export all objects from the spawn layer of level
    # For each object, output the level id, the position and if it's pickable (value 0x40)
    objects = []
    for level in level_data.values():
        for position, tile_id in enumerate(level.data_spawn):
            tile = tileset.tiles[tile_id]
            if tile.is_object():
                object_property = OBJECT_PROPERTIES['PICKABLE'] if tile.is_pickable() else 0
                object_property |= OBJECT_PROPERTIES['KEY'] if tile.is_key() else 0
                object_property |= OBJECT_PROPERTIES['HP'] if 'hp' in tile.properties else 0
                object_property |= OBJECT_PROPERTIES['WEAPON_1'] if tile.properties.get('weapon',
                                                                                        None) == 'weapon_1' else 0
                object_property |= OBJECT_PROPERTIES['WEAPON_2'] if tile.properties.get('weapon',
                                                                                        None) == 'weapon_2' else 0
                object_property |= OBJECT_PROPERTIES['CAULDRON'] if 'cauldron' in tile.properties else 0
                if 'hidden_treasure' in tile.properties:
                    hidden_treasure_tile_id = tile.compact_id

                if tile.is_lever():
                    object_property |= OBJECT_PROPERTIES['LEVER']

                    is_activated = tile.properties.get('activated', 'false') == 'true'
                    object_property |= 0x80 if is_activated else 0

                    if is_activated:
                        lever_activated_tile_id = tile.compact_id
                    else:
                        lever_deactivated_tile_id = tile.compact_id

                objects.append((level.compact_id, position, tile.compact_id, object_property))

    print(f"Outputting {len(objects)} objects")

    result_h = '#ifndef OBJECTS_DATA_H\n#define OBJECTS_DATA_H\n\n'
    result_h += '#include <stddef.h>\n\n'

    result_h += f"extern const unsigned char objects_count;\n"
    result_h += f"extern const unsigned char objects[];\n\n"

    # Outputs the properties as an enum
    result_h += f"typedef enum {{\n"
    for property_name, property_value in OBJECT_PROPERTIES.items():
        result_h += f"    E_{property_name} = {property_value},\n"
    result_h += f"}} object_property_t;\n\n"

    result_h += f"extern const unsigned char lever_deactivated_tile_id;\n"
    result_h += f"extern const unsigned char lever_activated_tile_id;\n\n"
    result_h += f"extern const unsigned char hidden_treasure_tile_id;\n\n"

    result_h += "#endif\n"

    result_c = '#include "objects_data.h"\n\n'
    result_c += "/* level id, position, compact_tile_id, property */\n"
    result_c += f"const unsigned char objects[] = {{\n"
    for an_object in objects:
        result_c += f"    {an_object[0]:3}, {an_object[1]:3}, {an_object[2]:3}, {an_object[3]:3},\n"

    result_c += "\n};\n\n"
    result_c += f"const unsigned char objects_count = {len(objects)};\n\n"

    result_c += f"const unsigned char lever_deactivated_tile_id = {lever_deactivated_tile_id};\n"
    result_c += f"const unsigned char lever_activated_tile_id = {lever_activated_tile_id};\n\n"
    result_c += f"const unsigned char hidden_treasure_tile_id = {hidden_treasure_tile_id};\n\n"

    output_filename = os.path.join(output_folder, 'objects_data.h')
    with open(output_filename, 'w') as f:
        f.write(result_h)

    output_filename = os.path.join(output_folder, 'objects_data.c')
    with open(output_filename, 'w') as f:
        f.write(result_c)


def output_levers(level_data, tileset):
    # Export all levers from the spawn layer of level
    # For each lever, output the level id, the position and the lever id
    levers = []
    for level in level_data.values():
        for position, tile_id in enumerate(level.data_spawn):
            tile = tileset.tiles[tile_id]
            if tile.is_lever():
                lever_id = level.get_lever_at(position)
                lever_id = int(lever_id[1:])
                levers.append((level.compact_id, position, lever_id))

    print(f"Outputting {len(levers)} levers")

    result_h = '#ifndef LEVERS_DATA_H\n#define LEVERS_DATA_H\n\n'
    result_h += '#include <stddef.h>\n\n'

    result_h += f"extern const unsigned char levers_count;\n"
    result_h += f"extern const unsigned char levers[];\n\n"
    result_h += "#endif\n"

    result_c = '#include "levers_data.h"\n\n'
    result_c += "/* level id, position, lever id */\n"
    result_c += f"const unsigned char levers[] = {{\n"
    for lever in levers:
        result_c += f"    {lever[0]:3}, {lever[1]:3}, {lever[2]:3},\n"

    result_c += "\n};\n\n"
    result_c += f"const unsigned char levers_count = {len(levers)};\n\n"

    output_filename = os.path.join(output_folder, 'levers_data.h')
    with open(output_filename, 'w') as f:
        f.write(result_h)

    output_filename = os.path.join(output_folder, 'levers_data.c')
    with open(output_filename, 'w') as f:
        f.write(result_c)


def output_inventory(tileset):
    objects = []
    for tile in tileset.tiles.values():
        inventory_position = tile.get_inventory_position()
        if inventory_position is not None and int(inventory_position) < 255:
            objects.append((tile.compact_id, int(tile.get_inventory_position())))

    print(f"Outputting {len(objects)} inventory objects")

    result_h = '#ifndef INVENTORY_DATA_H\n#define INVENTORY_DATA_H\n\n'
    result_h += '#include <stddef.h>\n\n'

    result_h += f"extern const unsigned char inventory_count;\n"
    result_h += f"extern const unsigned char inventory[];\n\n"

    result_h += "#endif\n"

    result_c = '#include "inventory_data.h"\n\n'
    result_c += "/* compact_tile_id, position */\n"
    result_c += f"const unsigned char inventory[] = {{\n"
    for an_object in objects:
        result_c += f"    {an_object[0]:3}, {an_object[1]:3},\n"

    result_c += "\n};\n\n"
    result_c += f"const unsigned char inventory_count = {len(objects)};\n\n"

    output_filename = os.path.join(output_folder, 'inventory_data.h')
    with open(output_filename, 'w') as f:
        f.write(result_h)

    output_filename = os.path.join(output_folder, 'inventory_data.c')
    with open(output_filename, 'w') as f:
        f.write(result_c)


def output_texts(level_data):
    # Gather the texts, remove the duplicates
    all_texts = {}
    for level in level_data.values():
        text = level.text
        if text is not None:
            if text not in all_texts:
                all_texts[text] = []
            all_texts[text].append(level.compact_id)

    print(f"Outputting {len(all_texts)} texts")

    result_h = '#ifndef TEXTS_DATA_H\n#define TEXTS_DATA_H\n\n'
    result_h += '#include <stddef.h>\n\n'

    result_h += f"typedef struct {{\n"
    result_h += f"    unsigned char level_id;\n"
    result_h += f"    unsigned char text_id;\n"
    result_h += f"}} level_text_t;\n\n"

    result_h += f"extern const char * const all_room_texts[];\n"
    result_h += f"extern const level_text_t level_to_text[];\n"
    result_h += f"extern const unsigned char level_to_text_count;\n"
    result_h += "#endif\n"

    result_c = '#include "texts_data.h"\n\n'

    result_c += f"const char * const all_room_texts[] = {{\n"
    for text_id, text in enumerate(all_texts):
        result_c += f"      \"{text}\", // {text_id}\n"
    result_c += "};\n\n"

    result_c += f"const unsigned char level_to_text_count = {len(level_data)};\n\n"

    result_c += f"const level_text_t level_to_text[] = {{\n"
    for text_id, (_, level_ids) in enumerate(all_texts.items()):
        for level_id in level_ids:
            result_c += f"    {{ {level_id:3}, {text_id} }},\n"
    result_c += "\n};\n\n"

    output_filename = os.path.join(output_folder, 'texts_data.h')
    with open(output_filename, 'w') as f:
        f.write(result_h)

    output_filename = os.path.join(output_folder, 'texts_data.c')
    with open(output_filename, 'w') as f:
        f.write(result_c)


def output_tile_properties(tileset):
    # Export the properties of each tile
    # There are two considered properties at now
    # - obstacle: if the tile is an obstacle for the player, value 0x01
    # - door: if the tile is a door, value 0x02
    result_h = '#ifndef TILE_PROPERTIES_H\n#define TILE_PROPERTIES_H\n\n'
    result_h += '#include <stddef.h>\n\n'

    result_h += f"extern const unsigned char tile_properties[];\n\n"
    result_h += "#endif\n"

    result_c = '#include "tile_properties.h"\n\n'
    result_c += f"const unsigned char tile_properties[] = {{\n"

    # Works because compact_id are a compact range from 0 to used tiles count
    used_tiles_sorted_by_their_compact_id = sorted([tile for tile in tileset.tiles.values()
                                                    if tile.compact_id is not None],
                                                   key=lambda x: int(x.compact_id))
    for index, tile in enumerate(used_tiles_sorted_by_their_compact_id):
        properties = 0
        if tile.is_obstacle():
            properties |= 0x01
        if tile.is_door():
            properties |= 0x02
        if index % 10 == 0:
            result_c += "    "
        result_c += f"{properties:3},"
        if index % 10 == 9:
            result_c += "\n"
        else:
            result_c += " "

    result_c += "\n};\n\n"

    output_filename = os.path.join(output_folder, 'tile_properties.h')
    with open(output_filename, 'w') as f:
        f.write(result_h)

    output_filename = os.path.join(output_folder, 'tile_properties.c')
    with open(output_filename, 'w') as f:
        f.write(result_c)


def main():
    tiled_path = os.path.join(data_folder, 'tiled')
    world_map_filename = os.path.join(tiled_path, 'jeu-lynx.world')

    world_map = get_world_map(world_map_filename)
    level_data = read_rooms(tiled_path, world_map)

    # Compute the compact level ids
    compact_level_id_info = CompactIdInformation(level_data)
    for level in level_data.values():
        level.compact_id = compact_level_id_info.get_id_from_coordinates(level.get_coordinates())

    compact_tile_ids = compute_compacted_tiles_id(level_data)

    # Read tiles and write them on disk
    tileset_filename = os.path.join(tiled_path, 'jeu-lynx.tsx')
    tileset = read_tileset_data(tileset_filename)
    output_tiles_graph(compact_tile_ids, tileset)

    # Set compact ids for tiles in the tileset
    for tile_id, compact_tile_id in compact_tile_ids.items():
        try:
            tile = tileset.tiles[tile_id]
        except KeyError:
            tile = Tile(tile_id)
            tileset.tiles[tile_id] = tile
        tile.compact_id = compact_tile_id

    # Write the level data on disk
    output_level_data(level_data, tileset, compact_level_id_info)

    # Wrtie the tile properties on disk
    output_tile_properties(tileset)

    # Write the links data on disk
    output_level_links(level_data)

    # Write the spawners data on disk
    output_spawners(level_data, tileset, compact_tile_ids)

    # Write the doors data on disk
    output_doors(level_data, tileset)

    # Write the objects data on disk
    output_objects(level_data, tileset)

    # Write the levers data on disk
    output_levers(level_data, tileset)

    # Write inventory data on disk
    output_inventory(tileset)

    # Export level texts
    output_texts(level_data)


if __name__ == '__main__':
    main()
