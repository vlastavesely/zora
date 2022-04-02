#ifndef __ZORA_RANDR_H
#define __ZORA_RANDR_H

#include <xcb/randr.h>

struct zora_randr_state {
	struct xcb_connection_t *conn;
	struct xcb_screen_t *screen;
	unsigned int num_crtcs;
	unsigned int *crtcs;
};

#define ZORA_RANDR_STATE_INIT {}

int zora_randr_initialise(struct zora_randr_state *state);
int zora_randr_set_colour_temperature(struct zora_randr_state *state,
				      unsigned int temperature);
void zora_randr_finalise(struct zora_randr_state *state);

#endif /* __ZORA_RANDR_H */
