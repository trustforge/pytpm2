#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "/usr/tmp/tssdir/tpm2-tss/include/tss2/tss2_esys.h"


static PyMethodDef pytpm2_methods[] = {
	{"getrandom", (PyCFunction)pytpm2_getrandom, METH_VARARGS, pytpm2_getrandom_docstring},
	{NULL, NULL, 0, NULL}
};

static struct PyModuleDef pytpm2_module = {
	PyModuleDef_HEAD_INIT,
	"pytpm2",
	"pytpm2 bindings",
	-1,
	pytpm2_methods
};

PyMODINIT_FUNC PyInit_pytpm2(void){
	return PyModule_Create(&pytpm2_module);
}
