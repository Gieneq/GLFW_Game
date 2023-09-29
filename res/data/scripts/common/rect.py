from math import ceil, floor

class Rect:
    def __init__(self, x, y, width, height):
        self.x = x
        self.y = y
        self.width = width
        self.height = height

    @staticmethod
    def create_from(other_rect):
        return Rect(other_rect.x, other_rect.y, other_rect.width, other_rect.height)

    def get_align_to(self, align_x, align_y):
        aligned_left = floor(self.left() / align_x) * align_x
        aligned_right = ceil(self.right() / align_x) * align_x
        aligned_top = floor(self.top() / align_y) * align_y
        aligned_bottom = ceil(self.bottom() / align_y) * align_y
        return Rect(aligned_left, aligned_top, aligned_right - aligned_left, aligned_bottom - aligned_top)

    def left(self):
        return self.x

    def right(self):
        return self.x + self.width

    def top(self):
        return self.y

    def bottom(self):
        return self.y + self.height

    def is_intersecting(self, other):
        if (self.left() < other.right() and self.right() > other.left() and
                self.top() < other.bottom() and self.bottom() > other.top()):
            return True
        else:
            return False

    def contains_point(self, px, py):
        return self.left() <= px < self.right() and self.top() <= py < self.bottom()

    def __str__(self):
        return f"Rect(x={self.x}, y={self.y}, width={self.width}, height={self.height})"


def test_intersection(r1, r2, should_be):
    print(f"{r1} & {r2} : {r1.is_intersecting(r2)} ? {should_be}")


def test_align(r1, align_x, align_y):
    r1_aligned = r1.get_align_to(align_x, align_y)
    print(f"{r1} align ({align_x, align_y}) : {r1_aligned}")


def test_contains(r1, px, py):
    print(f"{r1} contains ({px}, {py}) ? {r1.contains_point(px, py)}")


if __name__ == '__main__':
    test_intersection(Rect(0, 0, 10, 10), Rect(5, 5, 10, 10), True)
    test_intersection(Rect(0, 0, 5, 5), Rect(5, 5, 10, 10), False)
    test_intersection(Rect(0, 0, 5, 5), Rect(4.9999, 4.9999, 10, 10), True)
    test_align(Rect(0, 0, 5, 5), 11, 12)
    test_align(Rect(-1, -2, 1, 22), 11, 12)
    test_contains(Rect(0, 0, 5, 5), 0, 0)
    test_contains(Rect(0, 0, 5, 5), 5, 5)