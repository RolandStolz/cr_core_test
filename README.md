## Structure
`src`: C core
`bindings/python`: Python bindings
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

