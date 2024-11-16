#include <Python.h>
#include <iostream>
#include <vector>
#include <utility> // Add for use "std::pair"

/**
 * initializeModelOnce() - Initialize the reinforcement learning model once.
 *
 * This function sets up the Python environment and imports the specified Python module
 * to initialize the reinforcement learning model for inference. It ensures that the 
 * Python interpreter is initialized and the model is loaded into memory.
 *
 * Context: Process context. Must be called only once to avoid redundant model loading.
 */
void initializeModelOnce() {
    printf("init model\n");
    Py_Initialize(); // Initialize Python interpreter
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('/home/deepracer/AWS_DeepRacer/AA/gen_ara/implementation/Machine/Calc/src/calc/integration')");
    printf("load python\n");
    PyObject *pName = PyUnicode_DecodeFSDefault("run_model");
    PyObject *pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (pModule != nullptr) {
        PyObject *pFuncInit = PyObject_GetAttrString(pModule, "initialize_model");
        if (pFuncInit && PyCallable_Check(pFuncInit)) {
            PyObject *pModelPath = PyUnicode_FromString("/home/deepracer/AWS_DeepRacer/AA/gen_ara/implementation/Machine/Calc/src/calc/integration/model.pb");
            PyObject *pInitArgs = PyTuple_Pack(1, pModelPath);
            PyObject *pInitResult = PyObject_CallObject(pFuncInit, pInitArgs);
            Py_DECREF(pModelPath);
            Py_DECREF(pInitArgs);
            Py_XDECREF(pInitResult);
        } else {
            PyErr_Print();
        }
        Py_XDECREF(pFuncInit);
        Py_DECREF(pModule);
    } else {
        PyErr_Print();
    }
}

/**
 * runModel() - Perform inference using lidar and camera data.
 * @lidar_data: Vector of floats containing processed lidar input data.
 * @camera_data: Vector of floats containing processed camera input data.
 *
 * This function loads the `run_model` Python module and invokes the `predict`
 * function, passing the lidar and camera data as arguments. The model output
 * consists of speed and steering angle predictions, represented as a pair of floats.
 *
 * Context: Process context. Assumes the Python environment is initialized.
 * Return:
 * * `std::pair<float, float>`:
 *   * `first` - Predicted speed from model.
 *   * `second` - Predicted steering angle from model.
 */
std::pair<float, float> runModel(const std::vector<float>& lidar_data, const std::vector<float>& camera_data) {
    PyObject *pName = PyUnicode_DecodeFSDefault("run_model");
    PyObject *pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    std::pair<float, float> resultValues = {0.0f, 0.0f};

    if (pModule != nullptr) {
        PyObject *pFuncPredict = PyObject_GetAttrString(pModule, "predict");
        if (pFuncPredict && PyCallable_Check(pFuncPredict)) {
            PyObject *pLidarData = PyList_New(lidar_data.size());
            for (size_t j = 0; j < lidar_data.size(); ++j) {
                PyList_SetItem(pLidarData, j, PyFloat_FromDouble(lidar_data[j]));
            }
            PyObject *pCameraData = PyList_New(camera_data.size());
            for (size_t j = 0; j < camera_data.size(); ++j) {
                PyList_SetItem(pCameraData, j, PyFloat_FromDouble(camera_data[j]));
            }

            PyObject *pPredictArgs = PyTuple_Pack(2, pLidarData, pCameraData);
            PyObject *pResult = PyObject_CallObject(pFuncPredict, pPredictArgs);
            Py_DECREF(pLidarData);
            Py_DECREF(pCameraData);
            Py_DECREF(pPredictArgs);

            if (pResult != nullptr && PyList_Check(pResult) && PyList_Size(pResult) == 2) {
                PyObject *pItem1 = PyList_GetItem(pResult, 0);
                PyObject *pItem2 = PyList_GetItem(pResult, 1);
                if (PyFloat_Check(pItem1) && PyFloat_Check(pItem2)) {
                    resultValues.first = PyFloat_AsDouble(pItem1);
                    resultValues.second = PyFloat_AsDouble(pItem2);
                }
                Py_DECREF(pResult);
            } else {
                PyErr_Print();
            }
            Py_XDECREF(pFuncPredict);
        }
        Py_DECREF(pModule);
    } else {
        PyErr_Print();
    }

    return resultValues;
}