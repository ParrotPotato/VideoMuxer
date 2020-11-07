#include "init_system.h"

#include <stdio.h>
#include <gst/gst.h>

int
init_dynamic_comp(dynamic_comp * comp, char * filesource, int index)
{
	char buffer[255];
	comp->index = index;
	sprintf(buffer, "dyn_source_%d", index);
	comp->decoder 	= gst_element_factory_make("uridecodebin", buffer);
	if(!comp->decoder)
	{
		printf("element not being initialised: %s\n", buffer);
		return -1;
	}
	printf("initialised %s\n", buffer);

	sprintf(buffer, "dyn_a_convert_%d", index);
	comp->a_convert	= gst_element_factory_make("audioconvert", buffer);
	if(!comp->a_convert)
	{
		printf("element not being initialised: %s\n", buffer);
		return -1;
	}
	printf("initialised %s\n", buffer);

	sprintf(buffer, "dyn_a_resample_%d", index);
	comp->a_resample	= gst_element_factory_make("audioresample", buffer);
	if(!comp->a_resample)
	{
		printf("element not being initialised: %s\n", buffer);
		return -1;
	}
	printf("initialised %s\n", buffer);

	sprintf(buffer, "dyn_a_sync_%d", index);
	comp->a_sync= gst_element_factory_make("identity", buffer);
	if(!comp->a_sync)
	{
		printf("element not being initialised: %s\n", buffer);
		return -1;
	}
	printf("initialised %s\n", buffer);
	g_object_set(G_OBJECT(comp->a_sync), "sync", TRUE, NULL);

	sprintf(buffer, "dyn_v_convert_%d", index);
	comp->v_convert	= gst_element_factory_make("videoconvert", buffer);
	if(!comp->v_convert)
	{
		printf("element not being initialised: %s\n", buffer);
		return -1;
	}
	printf("initialised %s\n", buffer);

	sprintf(buffer, "dyn_v_sync_%d", index);
	comp->v_sync= gst_element_factory_make("identity", buffer);
	if(!comp->v_sync)
	{
		printf("element not being initialised: %s\n", buffer);
		return -1;
	}
	g_object_set(G_OBJECT(comp->v_sync), "sync", TRUE, NULL);
	printf("initialised %s\n", buffer);

	if(	
		!comp->decoder 		||
		!comp->a_convert 	||
		!comp->a_resample 	||
		!comp->a_sync		||
		!comp->v_convert 	||
		!comp->v_sync
	  )
	{
		printf("Error unable to inititalise dymanic element %d!\n", index);
		return -1;
	}

	return 0;
}

int bin_add_dynamic_comp(GstElement * pipeline, dynamic_comp * comp)
{
	gst_bin_add_many(
			GST_BIN(pipeline),
			comp->decoder,
			comp->a_convert,
			comp->a_resample,
			comp->a_sync,
			comp->v_convert,
			comp->v_sync,
			NULL);

	printf("dynamic_comp added to pipeline\n");

	return 0;

}

int link_dynamic_comp(dynamic_comp * comp)
{
	if(
		!gst_element_link_many(
			comp->a_convert, 
			comp->a_resample, 
			comp->a_sync, 
			NULL)
	  )
	{
		printf("Audio pipline for elemnt %d failed to link\n", comp->index);
		return -1;
	}

	printf("Audio pipline linked\n");

	if(
		!gst_element_link_many(
			comp->v_convert, 
			comp->v_sync, 
			NULL)
	  )
	{
		printf("Video pipline for elemnt %d failed to link\n", comp->index);
		return -1;
	}

	printf("Video pipline linked\n");

	return 0;
}


control_data *
init_control_data(char ** sources, int source_count)
{
	control_data * data = NULL;
	
	data = (control_data *) malloc(sizeof(control_data));
	if(data == NULL) return NULL;

	data->source_count = source_count;
	data->comp = (dynamic_comp*) malloc(sizeof(dynamic_comp) * source_count);

	data->pipeline = gst_pipeline_new("test-pipeline");

	/* 
	 * Creating all the GElements for handling streaming
	 */
	
	data->v_switch = gst_element_factory_make("input-selector", "v_switch");
	printf("initialised v_switch\n");
	data->v_sink   = gst_element_factory_make("autovideosink", "v_sink");
	printf("initialised v_sink\n");

	data->a_switch = gst_element_factory_make("input-selector", "a_switch");
	printf("initialised a_switch\n");
	data->a_sink   = gst_element_factory_make("autoaudiosink", "a_sink");
	printf("initialised a_sink\n");

	for(int i = 0 ; i < data->source_count ; i++)
	{
		if(init_dynamic_comp(&data->comp[i], NULL, i) < 0)
		{
			return NULL;
		} 

	}

	return data;
}

int ss_bin_add_control_data(control_data * data)
{

	/*
	 * Adding all elements to same bin
	 */

	gst_bin_add_many(
			GST_BIN(data->pipeline),
			data->v_switch,
			data->v_sink,
			data->a_switch,
			data->a_sink,
			NULL);


	for(int i = 0 ; i < data->source_count ; i++)
	{
		if(bin_add_dynamic_comp(data->pipeline, &data->comp[i]) < 0)
		{
			return -1;
		}
	}

	return 0;
}

int ss_control_data_link_elements(control_data * data)
{
	/*
	 * Linking relevant elements with each other in right order
	 */

	if(!gst_element_link_many(data->v_switch, data->v_sink, NULL))
	{
		printf("Error occured while linking v switch and v sink\n");
		return -1;
	}

	if(!gst_element_link_many(data->a_switch, data->a_sink, NULL))
	{
		printf("Error occured while linking a switch and a sink\n");
		return -1;
	}

	for(int i = 0; i < data->source_count ; i++)
	{
		if(link_dynamic_comp(&data->comp[i]) < 0)
		{
			return -1;
		}
	}

	for(int i = 0; i < data->source_count ; i++)
	{
		if(!gst_element_link(data->comp[i].v_sync, data->v_switch))
		{
			printf("Link error : [%d] component sync -> v_switch\n", i);
			return -1;
		}
		if(!gst_element_link(data->comp[i].a_sync, data->a_switch))
		{
			printf("Link error : [%d] component sync -> a_switch\n", i);
			return -1;
		}
	}

	printf("component sync and switch linked\n");

	return 0;
}

