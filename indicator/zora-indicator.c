/*
 * Copyright © 2022, Vlasta Vesely
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted.
 *
 * There's ABSOLUTELY NO WARRANTY, express or implied.
 */
#include <indicator/zora-indicator.h>
#include <indicator/zora-dock.h>

#define SCHEME "com.vlastavesely.zora"

G_DEFINE_TYPE(ZoraIndicator, zora_indicator, APP_INDICATOR_TYPE);

static void dock_seat_grab(GtkWidget *dock, GdkDisplay *display)
{
	GdkSeat *seat;
	GdkWindow *window;
	int ret;

	seat = gdk_display_get_default_seat(display);
	window = gtk_widget_get_window(dock);

	ret = gdk_seat_grab(seat, window, GDK_SEAT_CAPABILITY_ALL, TRUE, NULL,
			    NULL, NULL, NULL);
	if (ret != GDK_GRAB_SUCCESS) {
		gtk_grab_remove(dock);
		gtk_widget_hide(dock);
	}
}

static void popup_dock(GtkStatusIcon *icon, ZoraIndicator *indicator)
{
	GdkScreen *screen;
	GdkDisplay *display;
	GdkMonitor *mon_num;
	GdkRectangle monitor, area;
	GtkOrientation orientation;
	GtkRequisition dock_req;
	GtkWidget *dock;
	unsigned int x, y;

	if (!gtk_status_icon_get_geometry(icon, &screen, &area, &orientation))
		return;

	dock = indicator->dock;
	display = gdk_screen_get_display(screen);
	mon_num = gdk_display_get_monitor_at_point(display, area.x, area.y);
	gdk_monitor_get_geometry(mon_num, &monitor);
	gtk_widget_get_preferred_size(dock, &dock_req, NULL);


	if (orientation == GTK_ORIENTATION_VERTICAL) {
		x = (area.x + area.width + dock_req.width <= monitor.x + monitor.width)
			? area.x + area.width : area.x - dock_req.width;

 		y = (area.y + dock_req.height <= monitor.y + monitor.height)
			? area.y : monitor.y + monitor.height - dock_req.height;
	} else {
		y = (area.y + area.height + dock_req.height <= monitor.y + monitor.height)
			? area.y + area.height : area.y - dock_req.height;

 		x = (area.x + dock_req.width <= monitor.x + monitor.width)
			? area.x : monitor.x + monitor.width - dock_req.width;
	}

	gtk_window_move(GTK_WINDOW(dock), x, y);
	gtk_widget_show_all(dock);
	dock_seat_grab(dock, display);
	gtk_widget_grab_focus(dock);

	indicator->dock_shown = 1;
}

static void popdown_dock(GtkStatusIcon *icon, ZoraIndicator *indicator)
{
	GtkWidget *dock = indicator->dock;

	gtk_widget_hide(dock);
	gtk_grab_remove(dock);

	indicator->dock_shown = 0;
}

static int dock_key_release(GtkWidget *dock, GdkEventKey *event, void *data)
{
	ZoraIndicator *indicator = data;

	if (event->keyval == GDK_KEY_Escape) {
		popdown_dock(indicator->icon, indicator);
	}

	return TRUE;
}

static int dock_button_press(GtkWidget *dock, GdkEventButton *event, void *data)
{
	ZoraIndicator *indicator = data;

	popdown_dock(indicator->icon, indicator);

	return TRUE;
}

static void update_indicator_state(ZoraIndicator *indicator)
{
	unsigned int temperature;
	const char *icon = "zora";
	char buf[256];

	if (!indicator->icon)
		return;

	if (indicator->control->enabled) {
		temperature = indicator->control->temperature;
		snprintf(buf, 256, "Colour temperature: %dK", temperature);
	} else {
		temperature = 6500;
		snprintf(buf, 256, "Colour temperature: Disabled");
	}

	if (temperature == 6500) {
		icon = "zora-white";
	} else if (temperature > 6500) {
		icon = "zora-blue";
	}

	gtk_status_icon_set_tooltip_text(indicator->icon, buf);
	g_object_set(G_OBJECT(indicator->icon), "icon-name", icon, NULL);
}


static void property_changed(ZoraControl *control, const GParamSpec *pspec,
			     ZoraIndicator *indicator)
{
	update_indicator_state(indicator);
}

static int button_press_event(GtkStatusIcon *icon, GdkEvent *event, void *data)
{
	ZoraIndicator *indicator = data;

	switch (event->button.button) {
	case 1:
		if (!indicator->dock_shown) {
			popup_dock(icon, indicator);
		} else {
			popdown_dock(icon, indicator);
		}
		break;
	case 2:
		if (!zora_control_get_enabled(indicator->control)) {
			zora_control_set_enabled(indicator->control, TRUE);
		} else {
			zora_control_set_enabled(indicator->control, FALSE);
		}
		break;
	case 3:
		puts("popup"); /* TODO */
		break;
	}

	return 0;
}

static void scroll_event(GtkStatusIcon *icon, GdkEvent *event, void *userdata)
{
	ZoraIndicator *indicator = userdata;
	ZoraControl *control = indicator->control;
	unsigned int temperature;

	if (zora_control_get_enabled(control) == FALSE) {
		zora_control_set_temperature(control, 6500);
		zora_control_set_enabled(control, TRUE);
	}

	temperature = zora_control_get_temperature(control);

	switch (event->scroll.direction) {
	case GDK_SCROLL_UP:
		temperature += 100;
		if (temperature > 13000) {
			temperature = 13000;
		}
		break;

	case GDK_SCROLL_DOWN:
		temperature -= 100;
		if (temperature < 1000) {
			temperature = 1000;
		}
		break;

	default:
		return;
	}

	zora_control_set_temperature(control, temperature);
}

static GtkStatusIcon *fallback(AppIndicator *app_inidcator)
{
	ZoraIndicator *indicator;
	GtkStatusIcon *icon;

	indicator = ZORA_INDICATOR(app_inidcator);
	icon = gtk_status_icon_new_from_icon_name("zora");

	g_signal_connect(G_OBJECT(icon), "button-press-event",
			 G_CALLBACK(button_press_event), indicator);
	g_signal_connect(G_OBJECT(icon), "scroll-event",
			 G_CALLBACK(scroll_event), indicator);

	indicator->icon = icon;
	update_indicator_state(indicator);

	return icon;
}

static void unfallback(AppIndicator *indicator, GtkStatusIcon *icon)
{
	g_object_unref(icon);
}

static void indicator_finalise(GObject *object)
{
	ZoraIndicator *indicator = ZORA_INDICATOR(object);

	zora_control_free(indicator->control);

	G_OBJECT_CLASS(zora_indicator_parent_class)->finalize(object);
}

static void zora_indicator_class_init(ZoraIndicatorClass *klass)
{
	AppIndicatorClass *indicator_class;
	GObjectClass *object_class = G_OBJECT_CLASS(klass);

	indicator_class = APP_INDICATOR_CLASS(klass);
	indicator_class->fallback = fallback;
	indicator_class->unfallback = unfallback;

	object_class->finalize = indicator_finalise;
}

static void zora_indicator_init(ZoraIndicator *indicator)
{
	ZoraControl *control;
	ZoraDock *dock;

	control = zora_control_new();
	dock = zora_dock_new(control);

	g_signal_connect(G_OBJECT(control), "notify::temperature",
			 G_CALLBACK(property_changed), indicator);
	g_signal_connect(G_OBJECT(control), "notify::enabled",
			 G_CALLBACK(property_changed), indicator);

	/* for closing the dock */
	g_signal_connect(G_OBJECT(dock), "button-press-event",
			 G_CALLBACK(dock_button_press), indicator);
	g_signal_connect(G_OBJECT(dock), "key-release-event",
			 G_CALLBACK(dock_key_release), indicator);

	indicator->control = control;
	indicator->dock = GTK_WIDGET(dock);
	indicator->dock_shown = 0;
}

ZoraIndicator *zora_indicator_new()
{
	return g_object_new(ZORA_INDICATOR_TYPE, "id", SCHEME,
			    "category", "Other", NULL);
}

void zora_indicator_free(ZoraIndicator *indicator)
{
	g_object_unref(indicator);
}
