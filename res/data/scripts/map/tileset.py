class Tileset:
    def __init__(self, first_gid, last_gid, path):
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
