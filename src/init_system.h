#ifndef INIT_HEADER

#include <gst/gst.h>

typedef struct _dynamic_segment_data
{
	int index;

	GstElement * decoder;

	int is_initialised;
	
	// audio elements
	GstElement * a_convert;
	GstElement * a_resample;
	GstElement * a_sync;

	// video elements 
	GstElement * v_convert;
	GstElement * v_sync;
	
} dynamic_comp;


typedef struct _control_data
{
	unsigned int source_count;

	GstElement 		* pipeline;
	GstBus 			* bus;

	dynamic_comp 	* comp;

	GstElement 		* v_switch;
	GstElement 		* v_sink;

	GstElement 		* a_switch;
	GstElement 		* a_sink;
} control_data;

control_data * ss_init_control_data(int src_count);

int ss_bin_add_control_data(control_data * ctrl_ptr);
int ss_control_data_link_elements(control_data * ctrl_ptr);

void ss_add_media_source(control_data * data, char * source_location, int comp_index);

void ss_quit_main_loop(void);

void ss_play_pipeline(control_data * data);

#define INIT_HEADER
#endif 
