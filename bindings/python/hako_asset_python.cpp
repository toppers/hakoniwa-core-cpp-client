#include "Python.h"
#include <stdio.h>
#include "hako_asset.h"

#ifdef __cplusplus
extern "C" {
#endif

PyObject* py_on_initialize_callback = NULL; // グローバル変数でPython関数の参照を保持
PyObject* py_on_simulation_step_callback = NULL;
PyObject* py_on_manual_timing_control_callback = NULL;
PyObject* py_on_reset_callback = NULL;

static int callback_wrapper(PyObject* callback, hako_asset_context_t* context) {
    if (callback != NULL) {
        PyObject* context_capsule = PyCapsule_New(context, NULL, NULL);
        if (!context_capsule) {
            // PyCapsule_Newが失敗した場合
            PyErr_Print();
            return -1; // エラーを示す
        }

        PyObject* arglist = Py_BuildValue("(O)", context_capsule);
        Py_DECREF(context_capsule); // arglistに渡した後はもう不要なので減らす

        if (!arglist) {
            // Py_BuildValueが失敗した場合
            PyErr_Print();
            return -1; // エラーを示す
        }

        PyObject* result = PyObject_CallObject(callback, arglist);
        Py_DECREF(arglist); // 使用後に参照カウントを減らす

        if (result == NULL) {
            // コールバック関数の呼び出しに失敗した場合
            PyErr_Print(); // エラーがあれば表示
            return -1; // エラーを示す
        }

        // Python関数の戻り値をCのintに変換
        long c_result = PyLong_AsLong(result);
        Py_DECREF(result); // 使用後に参照カウントを減らす

        if (PyErr_Occurred()) {
            // PyLong_AsLongでの変換エラーのチェック
            PyErr_Print();
            return -1;
        }

        return (int)c_result; // Cの関数の戻り値として使用
    }
    return 0; // コールバックが設定されていない場合は成功を示す
}

static int on_initialize_wrapper(hako_asset_context_t* context)
{
    return callback_wrapper(py_on_initialize_callback, context);
}
static int on_reset_wrapper(hako_asset_context_t* context)
{
    return callback_wrapper(py_on_reset_callback, context);
}
static int on_simulation_step_wrapper(hako_asset_context_t* context)
{
    return callback_wrapper(py_on_simulation_step_callback, context);
}
static int on_manual_timing_control_wrapper(hako_asset_context_t* context)
{
    return callback_wrapper(py_on_manual_timing_control_callback, context);
}

static struct hako_asset_callbacks_s hako_asset_callback_python;

//asset_register
static PyObject* asset_register(PyObject*, PyObject* args) {
    const char *asset_name;
    const char *config_path;
    PyObject *callbacks_dict;
    long delta_usec;
    int model;

    // 引数を解析し、コールバック関数の辞書を取得
    if (!PyArg_ParseTuple(args, "ssOli", &asset_name, &config_path, &callbacks_dict, &delta_usec, &model)) {
        return NULL;
    }

    if (!PyDict_Check(callbacks_dict)) {
        PyErr_SetString(PyExc_TypeError, "callbacks must be a dictionary");
        return NULL;
    }
    // コールバック関数を取り出し、グローバル変数に設定
    py_on_initialize_callback = PyDict_GetItemString(callbacks_dict, "on_initialize");
    Py_XINCREF(py_on_initialize_callback); // 参照カウントを増やす
    hako_asset_callback_python.on_initialize = on_initialize_wrapper;
    py_on_reset_callback = PyDict_GetItemString(callbacks_dict, "on_reset");
    Py_XINCREF(py_on_reset_callback); // 参照カウントを増やす
    hako_asset_callback_python.on_reset = on_reset_wrapper;

    hako_asset_callback_python.on_simulation_step = on_simulation_step_wrapper;
    hako_asset_callback_python.on_manual_timing_control = on_manual_timing_control_wrapper;

    PyObject* temp;

    // on_simulation_stepコールバックの取得と設定
    temp = PyDict_GetItemString(callbacks_dict, "on_simulation_step");
    if (temp == Py_None) {
        hako_asset_callback_python.on_simulation_step = NULL; // Python側でNoneが指定された場合
    } else if (temp != NULL) {
        Py_XINCREF(temp); // 参照カウントを増やす
        Py_XDECREF(py_on_simulation_step_callback); // 以前の値があれば参照カウントを減らす
        py_on_simulation_step_callback = temp;
        hako_asset_callback_python.on_simulation_step = on_simulation_step_wrapper;
    }

    // on_manual_timing_controlコールバックの取得と設定
    temp = PyDict_GetItemString(callbacks_dict, "on_manual_timing_control");
    if (temp == Py_None) {
        hako_asset_callback_python.on_manual_timing_control = NULL; // Python側でNoneが指定された場合
    } else if (temp != NULL) {
        Py_XINCREF(temp); // 参照カウントを増やす
        Py_XDECREF(py_on_manual_timing_control_callback); // 以前の値があれば参照カウントを減らす
        py_on_manual_timing_control_callback = temp;
        hako_asset_callback_python.on_manual_timing_control = on_manual_timing_control_wrapper;
    }

    // hako_asset_register関数の呼び出し
    int result = hako_asset_register(asset_name, config_path, &hako_asset_callback_python, (hako_time_t)delta_usec, (HakoAssetModelType)model);

    // 結果の返却
    if (result == 0) {
        Py_RETURN_TRUE;
    } else {
        Py_RETURN_FALSE;
    }
}

// hako_asset_start関数のPythonラッパー
static PyObject* py_hako_asset_start(PyObject*, PyObject* args) {
    // この関数が引数を取らないことを確認
    if (!PyArg_ParseTuple(args, "")) {
        return NULL; // エラー時はNULLを返す
    }

    int result = hako_asset_start();
    if (result == 0) {
        Py_RETURN_TRUE; // 成功時はPythonのTrueを返す
    } else {
        Py_RETURN_FALSE; // 失敗時はPythonのFalseを返す
    }
}
static PyObject* py_hako_asset_simulation_time(PyObject*, PyObject*) {
    hako_time_t sim_time = hako_asset_simulation_time();

    return PyLong_FromLongLong(sim_time);
}
static PyObject* py_hako_asset_usleep(PyObject*, PyObject* args) {
    hako_time_t sleep_time_usec;

    if (!PyArg_ParseTuple(args, "L", &sleep_time_usec)) {
        return NULL;
    }

    int result = hako_asset_usleep(sleep_time_usec);

    if (result == 0) {
        Py_RETURN_NONE;
    } else {
        PyErr_Format(PyExc_RuntimeError, "hako_asset_usleep failed with error code: %d", result);
        return NULL;
    }
}

static PyObject* py_hako_asset_pdu_read(PyObject*, PyObject* args) {
    const char* robo_name;
    HakoPduChannelIdType lchannel;
    Py_ssize_t buffer_len;

    // 引数を解析（ロボット名、チャンネルID、バッファサイズ）
    if (!PyArg_ParseTuple(args, "siL", &robo_name, &lchannel, &buffer_len)) {
        return NULL; // 引数解析に失敗した場合は、NULLを返す
    }

    // バッファを動的に確保
    char* buffer = (char*)malloc(buffer_len * sizeof(char));
    if (buffer == NULL) {
        PyErr_NoMemory();
        return NULL;
    }

    // hako_asset_pdu_read関数を呼び出し
    int result = hako_asset_pdu_read(robo_name, lchannel, buffer, (size_t)buffer_len);

    if (result != 0) {
        free(buffer); // エラーが発生したらバッファを解放
        PyErr_Format(PyExc_RuntimeError, "hako_asset_pdu_read failed with error code: %d", result);
        return NULL;
    }

    // 読み込んだデータをPythonのbytesオブジェクトとして返す
    PyObject* py_data = PyBytes_FromStringAndSize(buffer, buffer_len);
    free(buffer); // Pythonオブジェクトにデータをコピーした後はバッファを解放
    return py_data;
}

static PyObject* py_hako_asset_pdu_write(PyObject*, PyObject* args) {
    PyObject* py_pdu_data;
    char *robo_name;
    HakoPduChannelIdType lchannel;
    size_t len;
    if (!PyArg_ParseTuple(args, "siYL", &robo_name, &lchannel, &py_pdu_data, &len))
    {
        return NULL;
    }
    char* pdu_data = PyByteArray_AsString(py_pdu_data);
    bool ret = hako_asset_pdu_write(robo_name, lchannel, pdu_data, len);
    return Py_BuildValue("O", ret ? Py_True : Py_False);
}


static PyMethodDef hako_asset_python_methods[] = {
    {"asset_register", asset_register, METH_VARARGS, "Register asset"},
    {"start", py_hako_asset_start, METH_VARARGS, "Start the asset."},
    {"simulation_time", py_hako_asset_simulation_time, METH_NOARGS, "Get the current simulation time."},
    {"usleep", py_hako_asset_usleep, METH_VARARGS, "Sleep for the specified time in microseconds."},
    {"pdu_read", py_hako_asset_pdu_read, METH_VARARGS, "Read PDU data for the specified robot name and channel ID."},
    {"pdu_write", py_hako_asset_pdu_write, METH_VARARGS, "Write PDU data for the specified robot name and channel ID."},
    { .ml_name = NULL, .ml_meth = NULL, .ml_flags = 0,  .ml_doc = NULL},
};
//module creator
PyMODINIT_FUNC PyInit_hako_asset(void)
{
    static struct PyModuleDef hako_obj;
    hako_obj.m_base = PyModuleDef_HEAD_INIT;
    hako_obj.m_name = "hako_asset_python";
    hako_obj.m_doc = "Python3 C API Module(Hakoniwa)";
    hako_obj.m_methods = hako_asset_python_methods;
    hako_obj.m_slots = NULL;
    return PyModule_Create(&hako_obj);
}
#ifdef __cplusplus
}
#endif
