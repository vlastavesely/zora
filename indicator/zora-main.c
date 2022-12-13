/*
 * Copyright © 2022, Vlasta Vesely
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted.
 *
 * There's ABSOLUTELY NO WARRANTY, express or implied.
 */
#include <indicator/zora-application.h>

int main(int argc, char **argv)
{
	ZoraApplication *app;
	int ret;

	app = zora_application_new();

	ret = g_application_run(G_APPLICATION(app), argc, (char **) argv);
	g_object_unref(app);

	return ret;
}
