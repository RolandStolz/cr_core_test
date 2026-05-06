from cffi import FFI
from pathlib import Path

src = Path(__file__).parents[3] / "src"

ffi = FFI()

ffi.cdef("""
    typedef struct { double x; double y; } Point;
    Point make_point(double x, double y);
    Point Point_add(Point a, Point b);
""")

ffi.set_source("_c_python_module",
    '#include "structs.h"\n#include "functions.h"',
    include_dirs=[str(src)],
    sources=[str(src / "structs.c"), str(src / "functions.c")],
)

if __name__ == "__main__":
    import sys
    tmpdir = sys.argv[1] if len(sys.argv) > 1 else Path(__file__).parent._str
    ffi.compile(tmpdir=tmpdir, verbose=True)
