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

static PyObject* PyPoint_addr(PyPoint* self, PyObject* Py_UNUSED(ignored)) {
    return PyLong_FromVoidPtr(&self->p);
}

static PyMethodDef PyPoint_methods[] = {
    {"addr", (PyCFunction)PyPoint_addr, METH_NOARGS, "raw pointer to underlying Point"}, {NULL}};

static PyTypeObject PyPointType = {
    .ob_base = PyVarObject_HEAD_INIT(NULL, 0).tp_name = "c_python_module.Point",
    .tp_basicsize = sizeof(PyPoint),
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = PyType_GenericNew,
    .tp_init = (initproc)PyPoint_init,
    .tp_members = PyPoint_members,
    .tp_methods = PyPoint_methods,
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

// State definition
typedef struct {
    PyObject_HEAD State s;
} PyState;

static PyMemberDef PyState_members[] = {
    {"orientation", Py_T_DOUBLE, offsetof(PyState, s.orientation), 0, "float"},
    {"velocity", Py_T_DOUBLE, offsetof(PyState, s.velocity), 0, "float"},
    {"time", Py_T_PYSSIZET, offsetof(PyState, s.time), 0, "int"},
    {NULL}};

static PyObject* PyState_get_position(PyState* self, void* closure) {
    printf("[c-copy] State.position -> new PyPoint (Point struct copied)\n");
    PyPoint* p = PyObject_New(PyPoint, &PyPointType);
    if (!p)
        return NULL;
    p->p = self->s.position;
    return (PyObject*)p;
}

static int PyState_set_position(PyState* self, PyObject* value, void* closure) {
    if (!PyObject_TypeCheck(value, &PyPointType)) {
        PyErr_SetString(PyExc_TypeError, "position must be a Point");
        return -1;
    }
    self->s.position = ((PyPoint*)value)->p;
    return 0;
}

static PyObject* PyState_addr(PyState* self, PyObject* Py_UNUSED(ignored)) {
    return PyLong_FromVoidPtr(&self->s);
}

static PyMethodDef PyState_methods[] = {
    {"addr", (PyCFunction)PyState_addr, METH_NOARGS, "raw pointer to underlying State"}, {NULL}};

static PyGetSetDef PyState_getset[] = {
    {"position", (getter)PyState_get_position, (setter)PyState_set_position, "Point", NULL},
    {NULL}};

static int PyState_init(PyState* self, PyObject* args, PyObject* kwds) {
    PyPoint* position;
    double orientation = 0, velocity = 0;
    Py_ssize_t time = 0;
    if (!PyArg_ParseTuple(args, "O!ddn", &PyPointType, &position, &orientation, &velocity, &time))
        return -1;
    self->s = make_state(position->p.x, position->p.y, orientation, velocity, (size_t)time);
    return 0;
}

static PyTypeObject PyStateType = {
    .ob_base = PyVarObject_HEAD_INIT(NULL, 0).tp_name = "c_python_module.State",
    .tp_basicsize = sizeof(PyState),
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = PyType_GenericNew,
    .tp_init = (initproc)PyState_init,
    .tp_members = PyState_members,
    .tp_getset = PyState_getset,
    .tp_methods = PyState_methods,
};

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
    if (PyType_Ready(&PyStateType) < 0)
        return NULL;
    PyObject* m = PyModule_Create(&c_python_module);
    if (!m)
        return NULL;
    PyModule_AddObjectRef(m, "Point", (PyObject*)&PyPointType);
    PyModule_AddObjectRef(m, "State", (PyObject*)&PyStateType);
    return m;
}