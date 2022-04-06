/*
 * Copyright © 2022, Vlasta Vesely
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted.
 *
 * There's ABSOLUTELY NO WARRANTY, express or implied.
 */
#include "dock.h"

enum {
	PROP_CONTROL = 1,
	N_PROPERTIES
};

static GParamSpec *obj_properties[N_PROPERTIES] = {NULL};

G_DEFINE_TYPE(ZoraDock, zora_dock, GTK_TYPE_WINDOW);

static void value_adjusted(GtkAdjustment *adjustment, ZoraDock *dock)
{
	ZoraControl *control = dock->control;
	unsigned int value;

	value = (unsigned int) gtk_adjustment_get_value(adjustment);
	if (zora_control_get_temperature(control) != value) {
		zora_control_set_temperature(control, value);
	}

	if (zora_control_get_enabled(control) == FALSE) {
		zora_control_set_enabled(control, TRUE);
	}
}

/* Just update the adjustment, but do not trigger the gamma correction. */
static void update_adjustment_value_silent(ZoraDock *dock)
{
	unsigned int value;

	value = zora_control_get_temperature(dock->control);
	if (zora_control_get_enabled(dock->control) == FALSE) {
		value = 6500;
	}

	value = value > 6500 ? 6500 : value;

	g_signal_handlers_block_by_func(dock->adjustment, value_adjusted, dock);
	gtk_adjustment_set_value(dock->adjustment, value);
	g_signal_handlers_unblock_by_func(dock->adjustment, value_adjusted, dock);
}

static void control_property_changed(ZoraControl *control,
					const GParamSpec *pspec,
					ZoraDock *dock)
{
	update_adjustment_value_silent(dock);
}

static void watch_control_temperature(ZoraDock *dock)
{
	g_signal_connect(G_OBJECT(dock->control), "notify::temperature",
			 G_CALLBACK(control_property_changed), dock);
	g_signal_connect(G_OBJECT(dock->control), "notify::enabled",
			 G_CALLBACK(control_property_changed), dock);
	update_adjustment_value_silent(dock);
}

static void set_property(GObject *object, unsigned int prop_id,
			 const GValue *value, GParamSpec *pspec)
{
	ZoraDock *dock = ZORA_DOCK(object);

	switch (prop_id) {
	case PROP_CONTROL:
		dock->control = g_value_get_object(value);
		watch_control_temperature(dock);
		break;

	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
		break;
	}
}

static void get_property(GObject *object, unsigned int prop_id, GValue *value,
			 GParamSpec *pspec)
{
	ZoraDock *dock = ZORA_DOCK(object);

	switch (prop_id) {
	case PROP_CONTROL:
		g_value_set_object(value, dock->control);
		break;

	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
		break;
	}
}

static void zora_dock_init(ZoraDock *dock)
{
	GtkWidget *frame;
	GtkWidget *scale;
	GtkAdjustment *adjustment;

	gtk_window_set_decorated(GTK_WINDOW(dock), FALSE);
	gtk_window_resize(GTK_WINDOW(dock), 10, 160);

	frame = gtk_frame_new(NULL);
	gtk_frame_set_shadow_type(GTK_FRAME(frame), GTK_SHADOW_OUT);
	gtk_container_add(GTK_CONTAINER(dock), frame);

	adjustment = gtk_adjustment_new(6500.0, 1000.0, 6500.0, 200.0, 200.0, 0.0);
	scale = gtk_scale_new(GTK_ORIENTATION_VERTICAL, adjustment);
	gtk_scale_set_draw_value(GTK_SCALE(scale), FALSE);
	gtk_range_set_inverted(GTK_RANGE(scale), TRUE);

	gtk_widget_set_margin_start(scale, 5);
	gtk_widget_set_margin_end(scale, 5);
	gtk_widget_set_margin_top(scale, 10);
	gtk_widget_set_margin_bottom(scale, 10);

	g_signal_connect(G_OBJECT(adjustment), "value-changed",
			 G_CALLBACK(value_adjusted), dock);

	gtk_container_add(GTK_CONTAINER(frame), scale);
	dock->adjustment = adjustment;
	dock->scale = scale;
}

static void zora_dock_class_init(ZoraDockClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);

	object_class->get_property = get_property;
	object_class->set_property = set_property;

	obj_properties[PROP_CONTROL] = g_param_spec_object("control", "Control",
			"Gamma control", ZORA_CONTROL_TYPE, G_PARAM_READWRITE);

	g_object_class_install_properties(object_class, N_PROPERTIES,
					  obj_properties);
}

ZoraDock *zora_dock_new(ZoraControl *control)
{
	return g_object_new(ZORA_DOCK_TYPE, "type", GTK_WINDOW_POPUP,
			    "control", control, NULL);
}

void zora_dock_free(ZoraDock *dock)
{
	g_object_unref(dock);
}
