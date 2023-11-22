import os
from xml.etree import ElementTree as ElementTree
from PIL import Image

from errors import DataError


class Tile:
    def __init__(self, id):
        self.id = int(id)
        self.properties = {}
        self.compact_id = None

    def add_property(self, name: str, value):
        self.properties[name.lower()] = value

    def is_door(self):
        return self.properties.get('door', 'false') == 'true'

    def is_obstacle(self):
        return self.properties.get('obstacle', 'false') == 'true'

    def is_object(self):
        return self.properties.get('object', 'false') == 'true'

    def is_pickable(self):
        return self.properties.get('pickable', 'false') == 'true'

    def is_key(self):
        return self.properties.get('key', 'false') == 'true'

    def is_lever(self):
        return self.properties.get('lever', 'false') == 'true'

    def get_inventory_position(self):
        return self.properties.get('inventaire', None)


class TileSet:
    def __init__(self):
        self.tiles = {}
        self.image = None

    def add_tile(self, xml_element: ElementTree.Element):
        tile = Tile(xml_element.attrib['id'])

        for child in xml_element:
            if child.tag == 'properties':
                for p in child:
                    tile.add_property(p.attrib['name'], p.attrib['value'])

        self.tiles[int(tile.id)] = tile

    def set_image(self, xml_element: ElementTree.Element, path):
        if self.image is not None:
            raise DataError("Cannot set image twice")
        try:
            filename = os.path.join(path, xml_element.attrib['source'])
            self.image = Image.open(filename)
            self.image = self.image.convert('RGBA')  # Ensures tile_image is in RGBA format

        except FileNotFoundError:
            print("File not found:", xml_element.attrib['source'])
            exit(1)


def read_tileset_data(tileset_filename):
    print("Reading tileset")

    tileset = TileSet()
    path = os.path.dirname(tileset_filename)

    try:
        with open(tileset_filename, 'r') as f:
            tree = ElementTree.parse(f)
            root = tree.getroot()
            for child in root:
                if child.tag == 'tile':
                    tileset.add_tile(child)
                elif child.tag == 'image':
                    tileset.set_image(child, path)
    except FileNotFoundError:
        print("File not found:", tileset_filename)
        exit(1)

    print(f"Tileset has {len(tileset.tiles)} tiles")
    print(f"Tileset image is {tileset.image.width}x{tileset.image.height}")
    return tileset
