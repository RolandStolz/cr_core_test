#include <Python.h>
#include <functions.h>
#include <structs.h>

// Point definition
typedef struct {
    PyObject_HEAD Point p;
} PyPoint;

// Defining class attributes
static PyMemberDef PyPoint_members[] = {{"x", Py_T_DOUBLE, offsetof(PyPoint, p.x), 0, "float"},
                                        {"y", Py_T_DOUBLE, offsetof(PyPoint, p.y), 0, "float"},
                                        {NULL}};

// Defining init
static int PyPoint_init(PyPoint* self, PyObject* args, PyObject* kwds) {
    self->p = make_point(0, 0);
    if (!PyArg_ParseTuple(args, "dd", &self->p.x, &self->p.y))
        return -1;
    return 0;
}

static PyTypeObject PyPointType = {
    .ob_base = PyVarObject_HEAD_INIT(NULL, 0).tp_name = "c_python_module.Point",
    .tp_basicsize = sizeof(PyPoint),
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = PyType_GenericNew,
    .tp_init = (initproc)PyPoint_init,
    .tp_members = PyPoint_members,
};

// add function definition
static PyObject* PyPoint_add(PyObject* self, PyObject* args) {
    PyPoint *a, *b;
    if (!PyArg_ParseTuple(args, "O!O!", &PyPointType, &a, &PyPointType, &b))
        return NULL;
    PyPoint* result = PyObject_New(PyPoint, &PyPointType);
    if (!result)
        return NULL;
    result->p = Point_add(a->p, b->p);
    return (PyObject*)result;
}

static PyMethodDef c_python_module_methods[] = {
    {"add", PyPoint_add, METH_VARARGS, NULL},
    {NULL},
};

// Defining the module
static PyModuleDef c_python_module = {
    .m_base = PyModuleDef_HEAD_INIT,
    .m_name = "c_python_module",
    .m_size = -1,
    .m_methods = c_python_module_methods,
};

// Module initialization
PyMODINIT_FUNC PyInit_c_python_module(void) {
    if (PyType_Ready(&PyPointType) < 0)
        return NULL;
    PyObject* m = PyModule_Create(&c_python_module);
    if (!m)
        return NULL;
    PyModule_AddObjectRef(m, "Point", (PyObject*)&PyPointType);
    return m;
}