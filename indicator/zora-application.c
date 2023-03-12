/*
 * Copyright © 2022, Vlasta Vesely
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted.
 *
 * There's ABSOLUTELY NO WARRANTY, express or implied.
 */
#include <indicator/zora-application.h>

G_DEFINE_TYPE(ZoraApplication, zora_application, GTK_TYPE_APPLICATION);

static void register_local_icons_theme()
{
	GtkIconTheme *theme;

	theme = gtk_icon_theme_get_default();
	gtk_icon_theme_prepend_search_path(theme, "share");
}

static void zora_application_startup(GApplication *application)
{
	GApplicationClass *app_class;
	ZoraApplication *app;

	/* calls gtk_init() */
	app_class = G_APPLICATION_CLASS(zora_application_parent_class);
	app_class->startup(application);

	register_local_icons_theme();

	app = ZORA_APPLICATION(application);
	app->indicator = zora_indicator_new(app);
}

static void zora_application_activate(GApplication *application)
{
	g_application_hold(application);
}

static void zora_application_init(ZoraApplication *application)
{
}

static void zora_application_class_init(ZoraApplicationClass *class)
{
	GApplicationClass *app_class;

	app_class = G_APPLICATION_CLASS(class);
	app_class->startup = zora_application_startup;
	app_class->activate = zora_application_activate;
}

ZoraApplication *zora_application_new()
{
	return g_object_new(ZORA_APPLICATION_TYPE, "application-id",
			    APPLICATION_ID, NULL);
}
