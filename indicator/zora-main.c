/*
 * Copyright © 2022, Vlasta Vesely
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted.
 *
 * There's ABSOLUTELY NO WARRANTY, express or implied.
 */
#include <indicator/zora-indicator.h>

int main(int argc, char **argv)
{
	ZoraIndicator *indicator;

	gtk_init(&argc, &argv);

	indicator = zora_indicator_new();
	if (indicator == NULL)
		return -1;

	gtk_main();

	zora_indicator_free(indicator);

	return 0;
}
