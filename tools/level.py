from errors import DataError

MAX_LEVEL_WIDTH = 10
MAX_LEVEL_HEIGHT = 10
LEVEL_WIDTH = 10
LEVEL_HEIGHT = 10


class Level:
    def __init__(self, coordinates: tuple):
        self.text = None
        self.coordinates = coordinates
        self.compact_id = None
        self.data_graph = []
        self.data_spawn = []
        self.links = []
        self.levers = []

    def add_graph(self, text):
        # All tiles are shifted 1 in the file from the tileset id
        # (probably to use 0 as "no tile")
        self.data_graph = [max(0, int(x) - 1) for x in text.split(',')]

    def add_spawn(self, text):
        self.data_spawn = [max(0, int(x) - 1) for x in text.split(',')]

    def get_used_tiles(self):
        used_tiles = set()
        for tile in self.data_graph:
            used_tiles.add(tile)
        for tile in self.data_spawn:
            used_tiles.add(tile)
        return used_tiles

    def get_coordinates(self):
        return self.coordinates

    def add_links(self, link_name, coordinates):
        self.links.append((link_name, coordinates))

    def add_lever(self, lever_name, coordinates):
        self.levers.append((lever_name, coordinates))

    def get_lever_at(self, position):
        position = int(position)
        coordinates = (position % 10, position // 10)
        for lever in self.levers:
            if lever[1] == coordinates:
                return lever[0]
        return None

    def set_text(self, text):
        self.text = text.strip()


def read_level_data(coordinates: tuple, tree):
    level = Level(coordinates)
    root = tree.getroot()
    for child in root:
        if child.tag == 'layer':
            width = int(child.attrib['width'])
            height = int(child.attrib['height'])
            if width != LEVEL_WIDTH and height != LEVEL_HEIGHT:
                raise DataError(
                    f"Level {coordinates} has wrong dimensions: {width}x{height} instead of {LEVEL_WIDTH}x{LEVEL_HEIGHT}")

            name = child.attrib['name']
            if name == 'Graph':
                data = child.find('data')
                level.add_graph(data.text)
            elif name == 'Spawn':
                data = child.find('data')
                level.add_spawn(data.text)
        elif child.tag == 'objectgroup':
            if child.attrib['name'] == 'Liaisons':
                # Iterate on children names object
                for link_object in child.findall('object'):
                    x = round(float(link_object.attrib['x']) / 16)
                    y = round(float(link_object.attrib['y']) / 16)
                    # Get the text from the child object named text
                    data = link_object.find('text')
                    level.add_links(data.text, (x, y))
            elif child.attrib['name'] == 'Leviers':
                # Iterate on children names object
                for link_object in child.findall('object'):
                    x = round(float(link_object.attrib['x']) / 16)
                    y = round(float(link_object.attrib['y']) / 16)
                    # Get the text from the child object named text
                    data = link_object.find('text')
                    level.add_lever(data.text, (x, y))
            elif child.attrib['name'] == 'Texte':
                # Iterate on children names object
                for link_object in child.findall('object'):
                    data = link_object.find('text')
                    level.set_text(data.text)

    return level


def get_position_from_coordinates(coordinates):
    return coordinates[0] + coordinates[1] * LEVEL_WIDTH
