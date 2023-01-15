#include "Python.h"
#include <stdio.h>
#include "hako_capi.h"


static PyObject* asset_write_pdu(PyObject* self, PyObject* args)
{
    if (self == NULL) {
        return NULL;
    }
    else if (args == NULL) {
        return NULL;
    }
    char *asset_name;
    HakoPduChannelIdType channel_id;
    char* pdu_data;
    size_t len;
    if (!PyArg_ParseTuple(args, "sisi", &asset_name, &channel_id, &pdu_data, &len))
    {
        return NULL;
    }
    bool ret = hako_asset_write_pdu(asset_name, channel_id, pdu_data, len);
    return Py_BuildValue("O", ret ? Py_True : Py_False);
}
static PyObject* asset_read_pdu(PyObject* self, PyObject* args)
{
    if (self == NULL) {
        return NULL;
    }
    else if (args == NULL) {
        return NULL;
    }
    char *asset_name;
    HakoPduChannelIdType channel_id;
    char* pdu_data;
    size_t len;
    if (!PyArg_ParseTuple(args, "sisi", &asset_name, &channel_id, &pdu_data, &len))
    {
        return NULL;
    }
    bool ret = hako_asset_read_pdu(asset_name, channel_id, pdu_data, len);
    return Py_BuildValue("O", ret ? Py_True : Py_False);
}

static PyObject* asset_is_pdu_dirty(PyObject* self, PyObject* args)
{
    if (self == NULL) {
        return NULL;
    }
    else if (args == NULL) {
        return NULL;
    }
    char *asset_name;
    HakoPduChannelIdType channel_id;
    if (!PyArg_ParseTuple(args, "si", &asset_name, &channel_id))
    {
        return NULL;
    }
    bool ret = hako_asset_is_pdu_dirty(asset_name, channel_id);
    return Py_BuildValue("O", ret ? Py_True : Py_False);
}
static PyObject* asset_create_pdu_channel(PyObject* self, PyObject* args)
{
    if (self == NULL) {
        return NULL;
    }
    else if (args == NULL) {
        return NULL;
    }
    HakoPduChannelIdType channel_id;
    size_t pdu_size;
    if (!PyArg_ParseTuple(args, "iI", &channel_id, &pdu_size))
    {
        return NULL;
    }
    bool ret = hako_asset_create_pdu_channel(channel_id, pdu_size);
    return Py_BuildValue("O", ret ? Py_True : Py_False);
}

static PyObject* asset_get_worldtime(PyObject* self, PyObject* args)
{
    if (self == NULL) {
        return NULL;
    }
    else if (args == NULL) {
        return NULL;
    }
    hako_time_t ret = hako_asset_get_worldtime();
    return Py_BuildValue("L", ret);
}

static PyObject* asset_notify_simtime(PyObject* self, PyObject* args)
{
    if (self == NULL) {
        return NULL;
    }
    else if (args == NULL) {
        return NULL;
    }
    hako_time_t simtime;
    char *asset_name;
    if (!PyArg_ParseTuple(args, "sL", &asset_name, &simtime))
    {
        return NULL;
    }
    hako_asset_notify_simtime(asset_name, simtime);
    return Py_BuildValue("");
}

static PyObject* asset_start_feedback(PyObject* self, PyObject* args)
{
    if (self == NULL) {
        return NULL;
    }
    else if (args == NULL) {
        return NULL;
    }
    bool result = false;
    int r;
    char *asset_name;
    if (!PyArg_ParseTuple(args, "si", &asset_name, &r))
    {
        return NULL;
    }
    if (r) {
        result = true;
        //printf("TRUE\n");
    }
    else {
        //printf("FALSE\n");
    }
    bool ret = hako_asset_start_feedback(asset_name, result);
    return Py_BuildValue("O", ret ? Py_True : Py_False);
}
static PyObject* asset_stop_feedback(PyObject* self, PyObject* args)
{
    if (self == NULL) {
        return NULL;
    }
    else if (args == NULL) {
        return NULL;
    }
    bool result = false;
    int r;
    char *asset_name;
    if (!PyArg_ParseTuple(args, "si", &asset_name, &r))
    {
        return NULL;
    }
    if (r) {
        result = true;
        //printf("TRUE\n");
    }
    else {
        //printf("FALSE\n");
    }
    bool ret = hako_asset_stop_feedback(asset_name, result);
    return Py_BuildValue("O", ret ? Py_True : Py_False);
}
static PyObject* asset_reset_feedback(PyObject* self, PyObject* args)
{
    if (self == NULL) {
        return NULL;
    }
    else if (args == NULL) {
        return NULL;
    }
    bool result = false;
    int r;
    char *asset_name;
    if (!PyArg_ParseTuple(args, "si", &asset_name, &r))
    {
        return NULL;
    }
    if (r) {
        result = true;
        //printf("TRUE\n");
    }
    else {
        //printf("FALSE\n");
    }
    bool ret = hako_asset_reset_feedback(asset_name, result);
    return Py_BuildValue("O", ret ? Py_True : Py_False);
}

static PyObject* asset_get_event(PyObject* self, PyObject* args)
{
    if (self == NULL) {
        return NULL;
    }
    else if (args == NULL) {
        return NULL;
    }
    char *asset_name;
    if (!PyArg_ParseTuple(args, "s", &asset_name))
    {
        return NULL;
    }
    int ret = hako_asset_get_event(asset_name);
    return Py_BuildValue("i", ret);
}
static PyObject* asset_notify_read_pdu_done(PyObject* self, PyObject* args)
{
    if (self == NULL) {
        return NULL;
    }
    else if (args == NULL) {
        return NULL;
    }
    char *asset_name;
    if (!PyArg_ParseTuple(args, "s", &asset_name))
    {
        return NULL;
    }
    hako_asset_notify_read_pdu_done(asset_name);
    return Py_BuildValue("");
}
static PyObject* asset_notify_write_pdu_done(PyObject* self, PyObject* args)
{
    if (self == NULL) {
        return NULL;
    }
    else if (args == NULL) {
        return NULL;
    }
    char *asset_name;
    if (!PyArg_ParseTuple(args, "s", &asset_name))
    {
        return NULL;
    }
    hako_asset_notify_write_pdu_done(asset_name);
    return Py_BuildValue("");
}
static PyObject* asset_register(PyObject* self, PyObject* args)
{
    if (self == NULL) {
        return NULL;
    }
    else if (args == NULL) {
        return NULL;
    }
    char *asset_name;
    if (!PyArg_ParseTuple(args, "s", &asset_name))
    {
        return NULL;
    }
    bool ret = hako_asset_register_polling(asset_name);
    return Py_BuildValue("O", ret ? Py_True : Py_False);
}
static PyObject* asset_unregister(PyObject* self, PyObject* args)
{
    if (self == NULL) {
        return NULL;
    }
    else if (args == NULL) {
        return NULL;
    }
    char *asset_name;
    if (!PyArg_ParseTuple(args, "s", &asset_name))
    {
        return NULL;
    }
    bool ret = hako_asset_unregister(asset_name);
    return Py_BuildValue("O", ret ? Py_True : Py_False);
}

static PyObject* asset_is_pdu_sync_mode(PyObject* self, PyObject* args)
{
    if (self == NULL) {
        return NULL;
    }
    else if (args == NULL) {
        return NULL;
    }
    char *asset_name;
    if (!PyArg_ParseTuple(args, "s", &asset_name))
    {
        return NULL;
    }
    bool ret = hako_asset_is_pdu_sync_mode(asset_name);
    return Py_BuildValue("O", ret ? Py_True : Py_False);
}
static PyObject* asset_init(PyObject* self, PyObject* args)
{
    if (self == NULL) {
        return NULL;
    }
    else if (args == NULL) {
        return NULL;
    }
    bool ret = hako_asset_init();
    return Py_BuildValue("O", ret ? Py_True : Py_False);
}
static PyObject* asset_is_simulation_mode(PyObject* self, PyObject* args)
{
    if (self == NULL) {
        return NULL;
    }
    else if (args == NULL) {
        return NULL;
    }
    bool ret = hako_asset_is_simulation_mode();
    return Py_BuildValue("O", ret ? Py_True : Py_False);
}
static PyObject* asset_is_pdu_created(PyObject* self, PyObject* args)
{
    if (self == NULL) {
        return NULL;
    }
    else if (args == NULL) {
        return NULL;
    }
    bool ret = hako_asset_is_pdu_created();
    return Py_BuildValue("O", ret ? Py_True : Py_False);
}

static PyObject* simevent_init(PyObject* self, PyObject* args)
{
    if (self == NULL) {
        return NULL;
    }
    else if (args == NULL) {
        return NULL;
    }
    bool ret = hako_simevent_init();
    return Py_BuildValue("O", ret ? Py_True : Py_False);
}
static PyObject* simevent_start(PyObject* self, PyObject* args)
{
    if (self == NULL) {
        return NULL;
    }
    else if (args == NULL) {
        return NULL;
    }
    bool ret = hako_simevent_start();
    return Py_BuildValue("O", ret ? Py_True : Py_False);
}
static PyObject* simevent_stop(PyObject* self, PyObject* args)
{
    if (self == NULL) {
        return NULL;
    }
    else if (args == NULL) {
        return NULL;
    }
    bool ret = hako_simevent_stop();
    return Py_BuildValue("O", ret ? Py_True : Py_False);
}
static PyObject* simevent_reset(PyObject* self, PyObject* args)
{
    if (self == NULL) {
        return NULL;
    }
    else if (args == NULL) {
        return NULL;
    }
    bool ret = hako_simevent_reset();
    return Py_BuildValue("O", ret ? Py_True : Py_False);
}
static PyObject* simevent_get_state(PyObject* self, PyObject* args)
{
    if (self == NULL) {
        return NULL;
    }
    else if (args == NULL) {
        return NULL;
    }
    int ret = hako_simevent_get_state();
    return Py_BuildValue("i", ret);
}

//definition of all methods of my module
static PyMethodDef hako_methods[] = {
    { .ml_name = "asset_init", .ml_meth = asset_init, .ml_flags = METH_VARARGS,  .ml_doc = "Initialize asset"},
    { .ml_name = "asset_is_pdu_sync_mode", .ml_meth = asset_is_pdu_sync_mode, .ml_flags = METH_VARARGS,  .ml_doc = "TODO"},
    { .ml_name = "asset_is_simulation_mode", .ml_meth = asset_is_simulation_mode, .ml_flags = METH_VARARGS,  .ml_doc = "TODO"},
    { .ml_name = "asset_is_pdu_created", .ml_meth = asset_is_pdu_created, .ml_flags = METH_VARARGS,  .ml_doc = "TODO"},

    { .ml_name = "asset_write_pdu", .ml_meth = asset_write_pdu, .ml_flags = METH_VARARGS,  .ml_doc = "TODO"},
    { .ml_name = "asset_read_pdu", .ml_meth = asset_read_pdu, .ml_flags = METH_VARARGS,  .ml_doc = "TODO"},
    { .ml_name = "asset_is_pdu_dirty", .ml_meth = asset_is_pdu_dirty, .ml_flags = METH_VARARGS,  .ml_doc = "TODO"},
    { .ml_name = "asset_create_pdu_channel", .ml_meth = asset_create_pdu_channel, .ml_flags = METH_VARARGS,  .ml_doc = "TODO"},
    { .ml_name = "asset_get_worldtime", .ml_meth = asset_get_worldtime, .ml_flags = METH_VARARGS,  .ml_doc = "TODO"},
    { .ml_name = "asset_notify_simtime", .ml_meth = asset_notify_simtime, .ml_flags = METH_VARARGS,  .ml_doc = "TODO"},


    { .ml_name = "asset_start_feedback", .ml_meth = asset_start_feedback, .ml_flags = METH_VARARGS,  .ml_doc = "TODO"},
    { .ml_name = "asset_stop_feedback", .ml_meth = asset_stop_feedback, .ml_flags = METH_VARARGS,  .ml_doc = "TODO"},
    { .ml_name = "asset_reset_feedback", .ml_meth = asset_reset_feedback, .ml_flags = METH_VARARGS,  .ml_doc = "TODO"},



    { .ml_name = "asset_get_event", .ml_meth = asset_get_event, .ml_flags = METH_VARARGS,  .ml_doc = "TODO"},
    { .ml_name = "asset_notify_read_pdu_done", .ml_meth = asset_notify_read_pdu_done, .ml_flags = METH_VARARGS,  .ml_doc = "TODO"},
    { .ml_name = "asset_notify_write_pdu_done", .ml_meth = asset_notify_write_pdu_done, .ml_flags = METH_VARARGS,  .ml_doc = "TODO"},
    { .ml_name = "asset_register", .ml_meth = asset_register, .ml_flags = METH_VARARGS,  .ml_doc = "TODO"},
    { .ml_name = "asset_unregister", .ml_meth = asset_unregister, .ml_flags = METH_VARARGS,  .ml_doc = "TODO"},
    { .ml_name = "asset_is_pdu_sync_mode", .ml_meth = asset_is_pdu_sync_mode, .ml_flags = METH_VARARGS,  .ml_doc = "TODO"},

    { .ml_name = "event_init", .ml_meth = simevent_init, .ml_flags = METH_VARARGS,  .ml_doc = "Initialize hakoniwa simobject"},
    { .ml_name = "start", .ml_meth = simevent_start, .ml_flags = METH_VARARGS,  .ml_doc = "Start Simulation"},
    { .ml_name = "stop", .ml_meth = simevent_stop, .ml_flags = METH_VARARGS,  .ml_doc = "Stop Simulation"},
    { .ml_name = "reset", .ml_meth = simevent_reset, .ml_flags = METH_VARARGS,  .ml_doc = "Reset Simulation"},
    { .ml_name = "state", .ml_meth = simevent_get_state, .ml_flags = METH_VARARGS,  .ml_doc = "Get simulation state"},
    { .ml_name = NULL, .ml_meth = NULL, .ml_flags = 0,  .ml_doc = NULL},
};

// hello module definition struct
static struct PyModuleDef hako_capis = {
    .m_base = PyModuleDef_HEAD_INIT,
    .m_name = "hakoc",
    .m_doc = "Python3 C API Module(Hakoniwa)",
    .m_size = -1,
    .m_methods = hako_methods,
    .m_slots = NULL
};

#ifdef __cplusplus
extern "C" {
#endif

//module creator
PyMODINIT_FUNC PyInit_hakoc(void)
{
    return PyModule_Create(&hako_capis);
}
#ifdef __cplusplus
}
#endif
