#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "randr.h"

int main(int argc, const char **argv)
{
	struct zora_randr_state state = ZORA_RANDR_STATE_INIT;
	int ret;

	ret = zora_randr_initialise(&state);
	if (ret != 0) {
		puts("failed to initialise.");
		goto out;
	}

	ret = zora_randr_set_colour_temperature(&state, 1400);
	if (ret != 0) {
		puts("failed to set the temperature colour.");
		goto out;
	}

	sleep(1);

	ret = zora_randr_set_colour_temperature(&state, 6500);
	if (ret != 0) {
		puts("failed to reset the temperature colour.");
		goto out;
	}
out:
	zora_randr_finalise(&state);

	return ret;
}
