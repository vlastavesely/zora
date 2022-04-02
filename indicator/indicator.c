/*
 * Copyright © 2022, Vlasta Vesely
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted.
 *
 * There's ABSOLUTELY NO WARRANTY, express or implied.
 */
#include "indicator.h"
#include "dock.h"

G_DEFINE_TYPE(ZoraIndicator, zora_indicator, APP_INDICATOR_TYPE);

static void popup_dock(ZoraIndicator *indicator)
{
	unsigned int x, y;

	x = y = 100; /* FIXME */

	gtk_window_move(GTK_WINDOW(indicator->dock), x, y);
	gtk_widget_show_all(indicator->dock);
}

static int button_press_event(GtkStatusIcon *icon, GdkEvent *event, void *data)
{
	ZoraIndicator *indicator = data;

	switch (event->button.button) {
	case 1:
		popup_dock(indicator);
		break;
	case 2:
		puts("enable"); /* TODO */
		break;
	case 3:
		puts("popup"); /* TODO */
		break;
	}

	return 0;
}

static void scroll_event(AppIndicator *indicator, int delta, int direction,
			 void *userdata)
{
	puts("scroll"); /* TODO */
}

static GtkStatusIcon *fallback(AppIndicator *indicator)
{
	GtkStatusIcon *icon;

	icon = gtk_status_icon_new_from_icon_name("zora");
	g_signal_connect(icon, "button-press-event", G_CALLBACK(button_press_event),
			 indicator);
	g_signal_connect(icon, "scroll-event", G_CALLBACK(scroll_event),
			 indicator);

	return icon;
}

static void unfallback(AppIndicator *indicator, GtkStatusIcon *icon)
{
	g_object_unref(icon);
}

static void zora_indicator_class_init(ZoraIndicatorClass *klass)
{
	AppIndicatorClass *indicator_class;

	indicator_class = APP_INDICATOR_CLASS(klass);
	indicator_class->fallback = fallback;
	indicator_class->unfallback = unfallback;
}

static void zora_indicator_init(ZoraIndicator *self)
{
	self->dock = GTK_WIDGET(zora_dock_new());
}

ZoraIndicator *zora_indicator_new()
{
	return g_object_new(ZORA_INDICATOR_TYPE, "id", "org.mate.zora",
			    "category", "Other", NULL);
}

void zora_indicator_free(ZoraIndicator *indicator)
{
	g_object_unref(indicator);
}
