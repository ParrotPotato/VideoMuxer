#include "switch_system.h"

#include <gtk/gtk.h>
#include <gst/gst.h>
#include <gst/video/videooverlay.h>

#include <gdk/gdk.h>
#if defined (GDK_WINDOWING_X11)
#include <gdk/gdkx.h>
#elif defined (GDK_WINDOWING_WIN32)
#include <gdk/gdkwin32.h>
#elif defined (GDK_WINDOWING_QUARTZ)
#include <gdk/gdkquartz.h>
#endif

#include <stdlib.h>

char ** parse_command_line_argument(int argc, char ** argv, int * stream_count)
{
	char ** ret = NULL;

	if(argc == 1)
	{
		printf("Application requirs stream uri's as command line argument\n");
		stream_count = 0;
		return NULL;
	}

	
	int count = argc - 1;

	ret = (char **) malloc(sizeof(char *) * (count));

	int i = 0;

	for(i = 0 ; i < count ; i++)
	{
		ret[i] = (char *) malloc(sizeof(char) * (PATH_MAX + 8));

		strcpy(ret[i],"file://");

		if(realpath(argv[i + 1], ret[i] + 7) == NULL)
		{
			i = i - 1;
			goto parse_exit_error;
		}
	}

	*stream_count = count;
	return ret;

parse_exit_error:
	
	while(i >= 0) 
	{
		free(ret[i]);
		i--;
	}

	free(ret);

	*stream_count = -1;
}


int main(int argc, char **argv)
{
	int stream_count = 0;
	char ** streams = parse_command_line_argument(argc, argv, &stream_count);

	if(stream_count == 0)
	{
		return 0;
	}
	else if(stream_count == -1)
	{
		perror("Error occured while getting realpath for stream args");
		return 0;
	}


	printf("Full path for streams\n");
	for(int i = 0 ; i < stream_count ; i++)
	{
		printf("Source[%d]: %s\n", i, streams[i]);
	}


	gtk_init(&argc, &argv);

	gst_init(&argc, &argv);

	control_data * data = ss_init_control_data(stream_count);

	if(data == NULL) 
	{
		return -1;
	}

	ss_bin_add_control_data(data);
	ss_control_data_link_elements(data);

	for(int i = 0 ; i < stream_count ; i++)
	{
		ss_add_media_source(data, streams[i], i);
	}


	ss_play_pipeline(data);

	return 0;
}
