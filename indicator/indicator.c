/*
 * Copyright © 2022, Vlasta Vesely
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted.
 *
 * There's ABSOLUTELY NO WARRANTY, express or implied.
 */
#include "indicator.h"

G_DEFINE_TYPE(ZoraIndicator, zora_indicator, APP_INDICATOR_TYPE);

static int button_press_event(GtkStatusIcon *icon, GdkEvent *event, void *data)
{
	switch (event->button.button) {
	case 1:
		puts("activate"); /* TODO */
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
	g_signal_connect(icon, "button-press-event", G_CALLBACK(button_press_event), NULL);
	g_signal_connect(icon, "scroll-event", G_CALLBACK(scroll_event), NULL);

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
