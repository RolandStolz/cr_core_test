 #include <Python.h>
  #include <structs.h>

  typedef struct {
      PyObject_HEAD
      Point p;
  } PyPoint;

  static PyMemberDef PyPoint_members[] = {
      {"x", Py_T_DOUBLE, offsetof(PyPoint, p.x), 0, "float"},
      {"y", Py_T_DOUBLE, offsetof(PyPoint, p.y), 0, "float"},
      {NULL}
  };

  static int PyPoint_init(PyPoint *self, PyObject *args, PyObject *kwds) {
      self->p = make_point(0, 0);
      if (!PyArg_ParseTuple(args, "dd", &self->p.x, &self->p.y))
          return -1;
      return 0;
  }

  static PyTypeObject PyPointType = {
      .ob_base = PyVarObject_HEAD_INIT(NULL, 0)
      .tp_name = "c_python_module.Point",
      .tp_basicsize = sizeof(PyPoint),
      .tp_flags = Py_TPFLAGS_DEFAULT,
      .tp_new = PyType_GenericNew,
      .tp_init = (initproc)PyPoint_init,
      .tp_members = PyPoint_members,
      .tp_doc = "Point(x: float, y: float)\n--\n\nA 2D point.",
  };

  static PyModuleDef c_python_module = {
      .m_base = PyModuleDef_HEAD_INIT,
      .m_name = "c_python_module",
      .m_size = -1,
  };

  PyMODINIT_FUNC PyInit_c_python_module(void) {
      if (PyType_Ready(&PyPointType) < 0) return NULL;
      PyObject *m = PyModule_Create(&c_python_module);
      if (!m) return NULL;
      PyModule_AddObjectRef(m, "Point", (PyObject *)&PyPointType);
      return m;
  }