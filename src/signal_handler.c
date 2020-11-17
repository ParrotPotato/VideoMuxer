#include "signal_handler.h"

#include "switch_system.h"

#include <gst/gst.h>

gboolean
signal_handler_switch_buffs (GstElement * video_switch)
{
  gint nb_sources;
  GstPad *active_pad, *new_pad;
  gchar *active_name;

  g_message ("switching");
  g_object_get (G_OBJECT (video_switch), "n-pads", &nb_sources, NULL);
  g_object_get (G_OBJECT (video_switch), "active-pad", &active_pad, NULL);

  active_name = gst_pad_get_name (active_pad);
  if (strcmp (active_name, "sink_0") == 0) {
    new_pad = gst_element_get_static_pad (video_switch, "sink_1");
  } else {
    new_pad = gst_element_get_static_pad (video_switch, "sink_0");
  }
  g_object_set (G_OBJECT (video_switch), "active-pad", new_pad, NULL);
  g_free (active_name);
  gst_object_unref (new_pad);

  g_message ("current number of sources : %d, active source %s",
      nb_sources, gst_pad_get_name (active_pad));

  return (GST_STATE (GST_ELEMENT (video_switch)) == GST_STATE_PLAYING);
}

gboolean
signal_handler_bus(GstBus * bus, GstMessage * message, gpointer data)
{
	// TODO(nitesh): Improve logs for handling buffer

	g_print ("Got %s message\n", GST_MESSAGE_TYPE_NAME (message));

  	switch (GST_MESSAGE_TYPE (message)) {
    	case GST_MESSAGE_ERROR:{
      		GError *err;
      		gchar *debug;

     		 gst_message_parse_error (message, &err, &debug);
     		 g_print ("Error: %s\n", err->message);
     		 g_error_free (err);
     		 g_free (debug);


	 		 ss_quit_main_loop();

     		 break;
    	}
    	case GST_MESSAGE_EOS:
      		/* end-of-stream */
	  		ss_quit_main_loop();
      		break;
    	default:
      		/* unhandled message */
      		break;
  	}

	/* we want to be notified again the next time there is a message
	 * on the bus, so returning TRUE (FALSE means we want to stop watching
	 * for messages on the bus and our callback should not be called again)
	 */

	return TRUE;
}


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
