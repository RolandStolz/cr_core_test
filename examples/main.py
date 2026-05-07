import c_python_module
import numpy as np


def main():
    p = c_python_module.Point(1.0, 2.0)
    print(p)
    print(p.x)
    print(p.y)

    p2 = c_python_module.add(p, p)
    print(p2.x)
    print(p2.y)

    a = np.zeros(2)
    np.linalg.norm(a)


if __name__ == "__main__":
    main()
