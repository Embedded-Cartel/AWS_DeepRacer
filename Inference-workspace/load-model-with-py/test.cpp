#include <Python.h>
#include <iostream>
#include <vector>

int main() {
    Py_Initialize();

    // Add current dir to sys.path
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('.')");

    PyObject *pName, *pModule, *pFuncInit, *pFuncPredict;

    // Setting module name (Example : run_model.py -> ("run_model")
    pName = PyUnicode_DecodeFSDefault("run_model");

    //Get module
    pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (pModule != nullptr) {
        // Get initialize_model function
        pFuncInit = PyObject_GetAttrString(pModule, "initialize_model");
        // Get predict function
        pFuncPredict = PyObject_GetAttrString(pModule, "predict");

        if (pFuncInit && PyCallable_Check(pFuncInit) && pFuncPredict && PyCallable_Check(pFuncPredict)) {
            // Init model
            PyObject* pModelPath = PyUnicode_FromString("/home/ubuntu/workspace-tensor/model.pb"); /* MODEL PATH */
            PyObject* pInitArgs = PyTuple_Pack(1, pModelPath);
            PyObject* pInitResult = PyObject_CallObject(pFuncInit, pInitArgs);
            Py_DECREF(pModelPath);
            Py_DECREF(pInitArgs);
            if (pInitResult == nullptr) {
                PyErr_Print();
                std::cerr << "Failed to initialize model" << std::endl;
                Py_DECREF(pFuncInit);
                Py_DECREF(pFuncPredict);
                Py_DECREF(pModule);
                Py_Finalize();
                return 1;
            }
            Py_DECREF(pInitResult);

            /* MAIN LOOP!!!!!! IF YOU WANT TO INPUT THIS IN AA -> Change For loop to While Loop*/
            for (int i = 0; i < 10; ++i) {
                // Intput Data create!!!!!! (@TODO : Make here to real data)
                std::vector<float> lidar_data(8, 0.5f); // Example....
                std::vector<float> camera_data(120 * 160 * 2, 0.5f); // Example....

                // Convert to Python list....
                PyObject* pLidarData = PyList_New(lidar_data.size());
                for (size_t j = 0; j < lidar_data.size(); ++j) {
                    PyList_SetItem(pLidarData, j, PyFloat_FromDouble(lidar_data[j]));
                }

                PyObject* pCameraData = PyList_New(camera_data.size());
                for (size_t j = 0; j < camera_data.size(); ++j) {
                    PyList_SetItem(pCameraData, j, PyFloat_FromDouble(camera_data[j]));
                }

                // predict function
                PyObject* pPredictArgs = PyTuple_Pack(2, pLidarData, pCameraData);
                PyObject* pResult = PyObject_CallObject(pFuncPredict, pPredictArgs);
                Py_DECREF(pLidarData);
                Py_DECREF(pCameraData);
                Py_DECREF(pPredictArgs);

                if (pResult != nullptr) {
                    // result
                    if (PyList_Check(pResult)) {
                        Py_ssize_t size = PyList_Size(pResult);
                        std::cout << "Inference result:";
                        for (Py_ssize_t k = 0; k < size; ++k) {
                            PyObject* pItem = PyList_GetItem(pResult, k);
                            if (PyFloat_Check(pItem)) {
                                double value = PyFloat_AsDouble(pItem);
                                std::cout << " " << value;
                            } else if (PyLong_Check(pItem)) {
                                long value = PyLong_AsLong(pItem);
                                std::cout << " " << value;
                            } else {
                                std::cout << " [Unknown Type]";
                            }
                        }
                        std::cout << std::endl;
                    } else {
                        std::cout << "Unexpected result type from predict function" << std::endl;
                    }
                    Py_DECREF(pResult);
                } else {
                    PyErr_Print();
                    std::cerr << "Failed to call predict function" << std::endl;
                }

                // TODO : Mutex or delay or Additional function......
            }
        } else {
            if (PyErr_Occurred())
                PyErr_Print();
            std::cerr << "Cannot find functions 'initialize_model' or 'predict'" << std::endl;
        }
        // Python REF EXIT
        Py_XDECREF(pFuncInit);
        Py_XDECREF(pFuncPredict);
        Py_DECREF(pModule);
    } else {
        PyErr_Print();
        std::cerr << "Failed to load 'run_model' module" << std::endl;
        Py_Finalize();
        return 1;
    }

    // Python CLOSE!!!
    Py_Finalize();

    return 0;
}
