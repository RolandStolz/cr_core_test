import c_python_module


def main():
    p = c_python_module.Point(1.0, 2.0)
    print(p)
    print(p.x)
    print(p.y)

    p2 = c_python_module.add(p, p)
    print(p2.x)
    print(p2.y)


if __name__ == "__main__":
    main()
