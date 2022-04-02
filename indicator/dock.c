/*
 * Copyright © 2022, Vlasta Vesely
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted.
 *
 * There's ABSOLUTELY NO WARRANTY, express or implied.
 */
#include "dock.h"

G_DEFINE_TYPE(ZoraDock, zora_dock, GTK_TYPE_WINDOW);

static void zora_dock_init(ZoraDock *dock)
{
	GtkWidget *frame;
	GtkWidget *scale;
	GtkAdjustment *adjustment;

	gtk_window_set_decorated(GTK_WINDOW(dock), FALSE);
	gtk_window_resize(GTK_WINDOW(dock), 50, 200);

	frame = gtk_frame_new(NULL);
	gtk_frame_set_shadow_type(GTK_FRAME(frame), GTK_SHADOW_OUT);
	gtk_container_add(GTK_CONTAINER(dock), frame);

	adjustment = gtk_adjustment_new(10.0, 1000.0, 13000.0, 50.0, 50.0, 50.0); // FIXME
	scale = gtk_scale_new(GTK_ORIENTATION_VERTICAL, adjustment);
	gtk_container_add(GTK_CONTAINER(frame), scale);
}

static void zora_dock_class_init(ZoraDockClass *klass)
{
}

ZoraDock *zora_dock_new()
{
	return g_object_new(ZORA_DOCK_TYPE, "type", GTK_WINDOW_POPUP, NULL);
}

void zora_dock_free(ZoraDock *dock)
{
	g_object_unref(dock);
}
