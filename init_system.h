#ifndef INIT_HEADER
#define INIT_HEADER

#include <gst/gst.h>

typedef struct _dynamic_segment_data
{
	int index;

	GstElement * decoder;
	
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

	GstElement * pipeline;

	dynamic_comp * comp;

	GstElement * v_switch;
	GstElement * v_sink;

	GstElement * a_switch;
	GstElement * a_sink;
} control_data;

control_data * init_control_data(char ** src, int src_count);

int ss_bin_add_control_data(control_data * ctrl_ptr);
int ss_control_data_link_elements(control_data * ctrl_ptr);

#endif 
