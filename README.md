## Structure
`src`: C core
`bindings/python`: Python bindings
`bindings/rust`: Rust bindings
`examples`: Examples


## Python bindings
You can install them with standard `pip` or `uv` installs.
There are two viable options: The Python C API and cffi. I tend to the C API, although it seems to have a bit of a steeper learning curve.

#### 1. The Python C API (`bindings/python/c_api`)
Make sure to enable the option `python_bind_with_c_api` in the root `CMakeLists.txt`.
Advantages:
- Fastest
- Direct design of the Python objects
- Somewhat close to nanobind

Disadvantage:
- verbose C boilerplate code
- probably easier to mess up
- have to define stubs seperately

#### 2. cffi (`bindings/python/cffi`)
Make sure to disable the option `python_bind_with_c_api` in the root `CMakeLists.txt`.
Advantags:
- little boiler plate code
- no separate stubs necessary

Disadvantages:
- raw bindings mimi c code structure
- therefore, you have to define separate pythonic objects

## Rust bindings
The bindings can be installed with `cargo build` in the root directory. Using the `cc` crate, the C library is compiled during the rust build process.

We use `bindgen` to automatically create rust bindings as defined in `bindings/rust/build.rs`, which generates structs and unsafe functions.
We can directly call the unsafe functions, or define wrapper functions for the structs, as shown in `examples/main.rs`.

Honestly, a rather straight-forward and pleasent experience!