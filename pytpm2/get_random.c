#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <tss2/tss2_esys.h>
#include "pytpm2_functions.h"
#include "pytpm2.h"

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

    char *s = malloc(2*i+1);
    int k;
    for(k=0;k<bytes->size;k++){
        s[k*2] = "0123456789ABCDEF"[bytes->buffer[k] >> 4];
        s[k*2+1] = "0123456789ABCDEF"[bytes->buffer[k] & 0xF];
    }
    s[i*2] = '\0';

    PyObject* result = Py_BuildValue("s", s);

    free(bytes);
    free(s);

    return result;
}
