#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <tss2/tss2_esys.h>

static char pytpm2_getrandom_docstring[] = "Get random bytes";

static PyObject *pytpm2_getrandom(PyObject *self, PyObject *args);

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


static PyObject *pytpm2_getrandom(PyObject *self, PyObject *args){

    int i;
    if(!PyArg_ParseTuple(args, "i", &i)){
        return NULL;
    }

    ESYS_CONTEXT *ectx = NULL;
    TPM2B_DIGEST *bytes;

    TSS2_RC rc = Esys_Initialize(&ectx, NULL, NULL);
    
    rc = Esys_GetRandom(ectx, ESYS_TR_NONE, ESYS_TR_NONE, ESYS_TR_NONE, i, &bytes);
    if (rc != TPM2_RC_SUCCESS){
        return NULL;
    }

    char *s = malloc(i+1);
    snprintf(s, i, "%02X", bytes);

    PyObject* result;
    result = Py_BuildValue("s", s);

    free(bytes);
    free(s);

    return result;
}
