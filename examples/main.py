import c_python_module


def main():
    p = c_python_module.Point(1.0, 2.0)
    print(p)
    print(p.x)
    print(p.y)
    print(f"Address {p.addr()}")

    p2 = c_python_module.add(p, p)
    print(p2.x)
    print(p2.y)

    state = c_python_module.State(p, 0, 0, 0)
    print(state)
    print(f"Address {state.addr()}")
    print(state.position)

    state2 = state
    print(state2)
    print(f"Address {state2.addr()}")


if __name__ == "__main__":
    main()
