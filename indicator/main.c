/*
 * Copyright © 2022  Vlasta Vesely
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted.
 *
 * There's ABSOLUTELY NO WARRANTY, express or implied.
 */
#include <libayatana-appindicator/app-indicator.h>
#include <zora/randr.h>
#include "indicator.h"

struct zora_state {
	struct zora_randr_state state;
};

static int initialise(struct zora_state *state)
{
	zora_randr_initialise(&state->state);
	return 0;
}

static int finalise(struct zora_state *state)
{
	zora_randr_finalise(&state->state);
	return 0;
}

int main(int argc, char **argv)
{
	struct zora_state state;
	int ret;

	gtk_init(&argc, &argv);

	ret = initialise(&state);
	if (ret != 0)
		return ret;

	puts("TODO");

	finalise(&state);

	return 0;
}
