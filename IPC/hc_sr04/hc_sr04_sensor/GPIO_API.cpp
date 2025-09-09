#include "GPIO.h"
#include <Python.h>
#include <iostream>

//---------------------------------------------------------------------//

/*                                                          
*       Define API of GPIO link from the GPIO.py                   *
*/                                                          

//---------------------------------------------------------------------//

/*
* API set up path for file python in function main() of C++
*/
void setupPythonSysPath() {
    // List the link of this folder C++
    const char* currentPath = "./";
    // Create PyObject from this link
    PyObject* pathObj = PyUnicode_FromString(currentPath);
    // Change const pointer to pointer
    PyObject* pathObjPtr = const_cast<PyObject*>(pathObj);
    // Create array for string "path"
    char* path = const_cast<char*>("path");
    // Add character link for sys.path of Python
    PyObject* sysPath = PySys_GetObject(path);
    PyList_Append(sysPath, pathObjPtr);
}

/*
* API set mode of pin GPIO
*/
void DIO_SetModePin(int pin_number, const char* modepin) {
    PyObject* pModule = PyImport_ImportModule("GPIO_API");
    if (pModule == NULL) {
        std::cout << "Failed to import GPIO module." << std::endl;
        return;
    }

    PyObject* pFunc = PyObject_GetAttrString(pModule, "DIO_SetModePin");
    if (pFunc && PyCallable_Check(pFunc)) {
        PyObject* pArgs = PyTuple_New(2);
        PyTuple_SetItem(pArgs, 0, PyLong_FromLong(pin_number));
        PyTuple_SetItem(pArgs, 1, PyUnicode_FromString(modepin));

        PyObject_CallObject(pFunc, pArgs);

        Py_DECREF(pArgs);
    }
    else {
        if (PyErr_Occurred()) {
            PyErr_Print();
        }
        std::cout << "Failed to call DIO_SetModePin function." << std::endl;
    }

    Py_XDECREF(pFunc);
    Py_DECREF(pModule);
}

/*
* API set up GPIO mode
*/
void DIO_SetUp(const char* mode) {
    PyObject* pModule = PyImport_ImportModule("GPIO_API");
    if (pModule == NULL) {
        std::cout << "Failed to import GPIO module." << std::endl;
        return;
    }

    PyObject* pFunc = PyObject_GetAttrString(pModule, "DIO_SetUp");
    if (pFunc && PyCallable_Check(pFunc)) {
        PyObject* pArgs = PyTuple_New(1);
        PyTuple_SetItem(pArgs, 0, PyUnicode_FromString(mode));

        PyObject_CallObject(pFunc, pArgs);

        Py_DECREF(pArgs);
    }
    else {
        if (PyErr_Occurred()) {
            PyErr_Print();
        }
        std::cout << "Failed to call DIO_SetUp function." << std::endl;
    }

    Py_XDECREF(pFunc);
    Py_DECREF(pModule);
}

/*
* API set GPIO output
*/
void DIO_WritePin(int pin_number, const char* state) {
    PyObject* pModule = PyImport_ImportModule("GPIO_API");
    if (pModule == NULL) {
        std::cout << "Failed to import GPIO module." << std::endl;
        return;
    }

    PyObject* pFunc = PyObject_GetAttrString(pModule, "DIO_WritePin");
    if (pFunc && PyCallable_Check(pFunc)) {
        PyObject* pArgs = PyTuple_New(2);
        PyTuple_SetItem(pArgs, 0, PyLong_FromLong(pin_number));
        PyTuple_SetItem(pArgs, 1, PyUnicode_FromString(state));

        PyObject_CallObject(pFunc, pArgs);

        Py_DECREF(pArgs);
    }
    else {
        if (PyErr_Occurred()) {
            PyErr_Print();
        }
        std::cout << "Failed to call DIO_WritePin function." << std::endl;
    }

    Py_XDECREF(pFunc);
    Py_DECREF(pModule);
}

/*
* API get GPIO input
*/
int DIO_ReadPin(int pin_number) {
    int result = -1; // Initialize result with a default value

    PyObject* pModule = PyImport_ImportModule("GPIO_API");
    if (pModule == NULL) {
        std::cout << "Failed to import GPIO module." << std::endl;
        return result;
    }

    PyObject* pFunc = PyObject_GetAttrString(pModule, "DIO_ReadPin");
    if (pFunc && PyCallable_Check(pFunc)) {
        PyObject* pArgs = PyTuple_New(1);
        PyTuple_SetItem(pArgs, 0, PyLong_FromLong(pin_number));

        PyObject* pResult = PyObject_CallObject(pFunc, pArgs);

        result = PyLong_AsLong(pResult);

        Py_DECREF(pArgs);
        Py_DECREF(pResult);
    }
    else {
        if (PyErr_Occurred()) {
            PyErr_Print();
        }
        std::cout << "Failed to call DIO_ReadPin function." << std::endl;
    }

    Py_XDECREF(pFunc);
    Py_DECREF(pModule);

    return result;
}

/*
* API get GPIO input
*/
void DIO_cleanup() {
    PyObject* pModule = PyImport_ImportModule("GPIO_API");
    if (pModule == NULL) {
        std::cout << "Failed to import GPIO module." << std::endl;
        return;
    }

    PyObject* pFunc = PyObject_GetAttrString(pModule, "DIO_cleanup");
    if (pFunc && PyCallable_Check(pFunc)) {
        PyObject* pArgs = PyTuple_New(0); 

        PyObject_CallObject(pFunc, pArgs);

        Py_DECREF(pArgs);
    }
    else {
        if (PyErr_Occurred()) {
            PyErr_Print();
        }
        std::cout << "Failed to call DIO_cleanup function." << std::endl;
    }

    Py_XDECREF(pFunc);
    Py_DECREF(pModule);
}