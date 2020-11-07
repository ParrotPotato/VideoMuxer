#include "init_system.h"

#include <gst/gst.h>

int main(int argc, char **argv)
{
	gst_init(&argc, &argv);

	control_data * data = ss_init_control_data(1);
	if(data == NULL) 
	{
		return -1;
	}

  /* Adding new comments here for testing out the auto compile
   * mechanics int SpaceVim
   */

	ss_bin_add_control_data(data);
	ss_control_data_link_elements(data);

	// add the source for each destination

	int comp_index = 0;
	char * source_location = "https://www.freedesktop.org/software/gstreamer-sdk/data/media/sintel_trailer-480p.webm";

	ss_add_media_source(data, source_location, comp_index);

	return 0;
}
