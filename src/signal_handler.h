#ifndef SIGNAL_HANDLER

#include <gst/gst.h>

#include "init_system.h"

void signal_handler_pad_added(GstElement* src, GstPad* new_pad, dynamic_comp* comp);


gboolean signal_handler_bus(GstBus * bus, GstMessage * message, gpointer data);

#define SIGNAL_HANDLER
#endif
