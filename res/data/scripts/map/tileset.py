class Tileset:
    def __init__(self, first_gid: int, last_gid: int, path: str):
        self.first_gid = first_gid
        self.last_gid = last_gid
        self.path = path

    def __str__(self):
        return f"{{ 'first_gid': {self.first_gid}, 'last_gid': {self.last_gid}, 'path': '{self.path}' }}"

    def has_in_common(self, other):
        if self.last_gid is None and other.last_gid is not None:
            return self.first_gid <= other.last_gid
        elif self.last_gid is not None and other.last_gid is None:
            return other.first_gid <= self.last_gid
        elif self.last_gid is None and other.last_gid is None:
            return True

        return self.first_gid <= other.last_gid and self.last_gid >= other.first_gid

    def __eq__(self, other):
        if isinstance(other, Tileset):
            return self.first_gid == other.first_gid and self.last_gid == other.last_gid and self.path == other.path
        return False

    def has_gid(self, gid):
        if self.last_gid is None:
            return self.first_gid <= gid
        return self.first_gid <= gid <= self.last_gid

    def as_dist(self):
        return {
            "first_gid": self.first_gid,
            "last_gid": self.last_gid,
            "path": self.path
        }
    

class TilesetsDatabase:
    def __init__(self):
        self.tilesets_unordered: list[Tileset] = []
        self.assignment = {}
        self.locked = False


    def add(self, tileset_dict: dict):
        if self.locked:
            raise Exception("TilesetsDatabase is locked")
        
        if "first_gid" not in tileset_dict or "tileset_path" not in tileset_dict:
            raise KeyError("first_gid or tileset_path not found in tileset_dict")
        
        tileset = Tileset(
            first_gid=tileset_dict["first_gid"],
            last_gid=None,
            path=tileset_dict["tileset_path"]
        )

        if tileset not in self.tilesets_unordered:
            self.tilesets_unordered.append(tileset)


    def get_tileset_by_id(self, id: int) -> Tileset:
        if not self.locked:
            raise Exception("TilesetsDatabase is not locked")
        
        if id not in self.assignment:
            raise KeyError(f"Tileset with id={id} not found")
        
        return self.assignment[id]
    
    def get_ids_of_tilesets(self, tilesets: list[dict]) -> list[int]:
        if not self.locked:
            raise Exception("TilesetsDatabase is not locked")
        
        ids = []
        for tileset in tilesets:
            if "first_gid" not in tileset:
                raise KeyError("first_gid not found in tileset")
            
            # tileset_path is hard to use
            
            for id in self.assignment.keys():
                if self.assignment[id].first_gid == tileset["first_gid"]:
                    ids.append(id)
                    break
        
        return ids


    def organize(self):
        sorted_tilesets = sorted(self.tilesets_unordered, key=lambda tileset: tileset.first_gid, reverse=False)

        """ last_gid on each tileset is first_gid on next tileset or None if last tileset """

        for idx in range(len(sorted_tilesets) - 1):
            sorted_tilesets[idx].last_gid = sorted_tilesets[idx + 1].first_gid - 1

        """ Move to assignment dict """
        for idx, tileset in enumerate(sorted_tilesets):
            self.assignment[idx] = tileset

        """ Clear and mark locked """
        self.tilesets_unordered.clear()
        self.locked = True


    def __str__(self) -> str:
        string = ""
        for id in self.assignment.keys():
            string += f"{id}: {self.assignment[id]}\n"
        return string[:-1]


    def __len__(self):
        return len(self.assignment)
