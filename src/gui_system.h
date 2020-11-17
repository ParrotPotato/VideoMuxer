#ifndef GUI_HEADER

#include "switch_system.h"

#include <gtk/gtk.h>

#include <gdk/gdk.h>
#if defined (GDK_WINDOWING_X11)
#include <gdk/gdkx.h>
#elif defined (GDK_WINDOWING_WIN32)
#include <gdk/gdkwin32.h>
#elif defined (GDK_WINDOWING_QUARTZ)
#include <gdk/gdkquartz.h>
#endif

typedef struct _window_handle 
{
	control_data * ctrl_data;

	GtkWidget *slider;              /* Slider widget to keep track of current position */
	GtkWidget *streams_list;        /* Text widget to display info about the streams */
	gulong slider_update_signal_id; /* Signal ID for the slider update signal */

	GstState state;                 /* Current state of the pipeline */
	gint64 duration;                /* Duration of the clip, in nanoseconds */


} window_handle;

int gui_main(control_data * data);


#define GUI_HEADER
#endif 
