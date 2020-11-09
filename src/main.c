#include "init_system.h"

#include <gst/gst.h>

int main(int argc, char **argv)
{
	gst_init(&argc, &argv);

	control_data * data = ss_init_control_data(2);
	if(data == NULL) 
	{
		return -1;
	}

	/* Adding new comments here for testing out the auto compile
	 * mechanics int SpaceVim
	 */

	ss_bin_add_control_data(data);
	ss_control_data_link_elements(data);

	/* These sources are personal local file 
	 *
	 * The source can be url (local file or a web url) 
	 */

	char * source_location_1 = "file:///home/nitesh/work/design-lab/StreamSwitch/data/test_day.mp4";
	char * source_location_2 = "file:///home/nitesh/work/design-lab/StreamSwitch/data/test_code.mp4";

	ss_add_media_source(data, source_location_2, 0);
	ss_add_media_source(data, source_location_1, 1);


	/* Setting up error handling */ 

	ss_play_pipeline(data);

	return 0;
}
