#include "signal_handler.h"

#include "init_system.h"

#include <gst/gst.h>

void 
signal_handler_pad_added(GstElement* src, GstPad* new_pad, dynamic_comp* comp)
{
	GstPad * a_sink_pad = gst_element_get_static_pad(comp->a_convert, "sink");
	GstPad * v_sink_pad = gst_element_get_static_pad(comp->v_convert, "sink");

	GstPadLinkReturn ret;
	GstCaps * new_pad_caps = NULL;
	GstStructure * new_pad_struct = NULL;

	const gchar * new_pad_type = NULL;

	g_print("Received new pad '%s' from '%s': \n", 
			GST_PAD_NAME(new_pad), GST_ELEMENT_NAME(src));

	new_pad_caps = gst_pad_get_current_caps(new_pad);
	new_pad_struct = gst_caps_get_structure(new_pad_caps, 0);
	new_pad_type = gst_structure_get_name(new_pad_struct);

	/* linking audio pad */

	if(
			g_str_has_prefix(new_pad_type, "audio/x-raw") &&
			!gst_pad_is_linked(a_sink_pad)
	  )
	{
		ret = gst_pad_link(new_pad, a_sink_pad);
		if(GST_PAD_LINK_FAILED(ret))
		{
			g_print("Type is '%s' but link failed.\n", new_pad_type);
		}
		else 
		{
			g_print("Link successfull (type: '%s').\n", new_pad_type);
		}
	}

	/* linking video pad */

	else if(
			g_str_has_prefix(new_pad_type, "video/x-raw") &&
			!gst_pad_is_linked(v_sink_pad)
	  )
	{
		ret = gst_pad_link(new_pad, v_sink_pad);
		if(GST_PAD_LINK_FAILED(ret))
		{
			g_print("Type is '%s' but link failed.\n", new_pad_type);
		}
		else 
		{
			g_print("Link successfull (type: '%s').\n", new_pad_type);
		}
	}

	else 
	{
		g_print(" The new pad has the type '%s' which is either not required or"
				" already initialised (Required: audio/x-raw, video/x-raw). Ign"
				"oring.\n", new_pad_type);

	}


	if(new_pad_caps != NULL)
	{
		gst_caps_unref(new_pad_caps);
	}

	gst_object_unref(a_sink_pad);
}
