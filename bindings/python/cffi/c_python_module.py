from _c_python_module import lib


class Point:
    def __init__(self, x: float, y: float) -> None:
        self._p = lib.make_point(x, y)

    @property
    def x(self) -> float:
        return self._p.x

    @property
    def y(self) -> float:
        return self._p.y


def add(a: Point, b: Point) -> Point:
    r = lib.Point_add(a._p, b._p)
    return Point(r.x, r.y)
