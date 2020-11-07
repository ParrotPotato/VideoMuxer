#include "init_system.h"

#include <gst/gst.h>

int main(int argc, char **argv)
{
	gst_init(&argc, &argv);

	control_data * data = init_control_data(NULL, 3);
	if(data == NULL) 
	{
		return -1;
	}

  /* Adding new comments here for testing out the auto compile
   * mechanics int SpaceVim
   */

	ss_bin_add_control_data(data);
	ss_control_data_link_elements(data);

	return 0;
}
