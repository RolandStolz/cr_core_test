import c_python_module


def main():
    p = c_python_module.Point(1.0, 2.0)
    print(p)
    print(p.x)
    print(p.y)


if __name__ == "__main__":
    main()
