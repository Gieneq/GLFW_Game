from common.config import Config


class Layer:
    def __init__(self, name: str, width: int, height: int, void_gid: int) -> None:
        self.name = name
        self.width = width
        self.height = height
        self.data: list[int] = []
        self.void_gid = void_gid


    def get_gids_count(self, gid: int) -> int:
        return self.data.count(gid)
    

    def is_filled_with_gid(self, gid: int) -> bool:
        return self.get_gids_count(gid) == self.width * self.height


    def set_data(self, data: list[int], config: Config) -> None:
        self.data = data


    def get_git_at(self, x: int, y: int) -> int:
        if x < 0 or x >= self.width:
            return None
        if y < 0 or y >= self.height:
            return None
        
        return self.data[y * self.width + x]
    

    def set_gid_at(self, x: int, y: int, gid: int) -> None:
        if x < 0 or x >= self.width:
            return
        if y < 0 or y >= self.height:
            return
        
        self.data[y * self.width + x] = gid


    def is_floor(self) -> bool:
        return "floor" in self.name.lower()
    

    def is_details(self) -> bool:
        return "det" in self.name.lower()


    def is_objects(self) -> bool:
        return "obj" in self.name.lower()
    

    def validate(self) -> bool:
        if self.width <= 0:
            return False
        if self.height <= 0:
            return False
        if len(self.data) != self.width * self.height:
            return False
        return True
    

    def __str__(self) -> str:
        return f"Layer(name={self.name}, width={self.width}, height={self.height}, void_blocks_count={self.get_gids_count(self.void_gid)})"


class Elevation:
    def __init__(self, index: int) -> None:
        self.index = index
        self.floor_layer: Layer = None
        self.details_layer: Layer = None
        self.objects_layers: list[Layer] = []


    def get_layers_count(self) -> int:
        layers_count = len(self.objects_layers)
        layers_count += 1 if self.floor_layer else 0
        layers_count += 1 if self.details_layer else 0
        return layers_count
    

    def add_layer(self, layer: Layer) -> None:
        if layer.is_floor():
            if self.floor_layer:
                raise Exception("Elevation already has floor layer")
            self.floor_layer = layer

        elif layer.is_details():
            if self.details_layer:
                raise Exception("Elevation already has details layer")
            self.details_layer = layer

        elif layer.is_objects():
            self.objects_layers.append(layer)
        
        else:
            # other are also countet as objects
            self.objects_layers.append(layer)


    def get_gid_at(self, x: int, y: int, layer_idx: int) -> int:
        if layer_idx < 0 or layer_idx >= len(self.objects_layers):
            return None
        
        layer = None
        if layer_idx == 0:
            layer = self.floor_layer
        elif layer_idx == 1:
            layer = self.details_layer
        else:
            layer = self.objects_layers[layer_idx - 2]
        
        return None if layer is None else layer.get_git_at(x, y)


    def validate(self) -> bool:
        if self.floor_layer:
            if not self.floor_layer.validate():
                return False
            
        if self.details_layer:
            if not self.details_layer.validate():
                return False
            
        for layer in self.objects_layers:
            if not layer.validate():
                return False

        return True

    def __str__(self) -> str:
        s = f"Elevation @ {self.index}(\n"
        s += f" floor_layer={self.floor_layer},\n"
        s += f" details_layer={self.details_layer},\n"
        for idx, layer in enumerate(self.objects_layers):
            s += f" objects_layer[{idx}]={layer},\n"
        s += ")"
        return s
    

def calculate_z_index(offset_y: int, tile_height: int) -> int:
    if offset_y == 0:
        return 0
    return -1 * int(offset_y / tile_height)