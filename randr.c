#include <stdlib.h>

#include "randr.h"

static struct xcb_screen_t *find_screen(struct xcb_connection_t *conn,
					int preferred)
{
	const struct xcb_setup_t *setup;
	struct xcb_screen_iterator_t iter;
	int i;

	setup = xcb_get_setup(conn);
	iter = xcb_setup_roots_iterator(setup);

	for (i = 0; iter.rem > 0; i++) {
		if (i == preferred) {
			return iter.data;
		}
		xcb_screen_next(&iter);
	}

	return NULL;
}

static struct xcb_randr_get_screen_resources_reply_t*
	load_screen_resources(struct xcb_connection_t *conn, unsigned int window)
{
	xcb_randr_get_screen_resources_cookie_t cookie;
	xcb_randr_get_screen_resources_reply_t *reply;
	xcb_generic_error_t *error = NULL;

	cookie = xcb_randr_get_screen_resources(conn, window);
	reply = xcb_randr_get_screen_resources_reply(conn, cookie, &error);

	if (error) {
		free(error);
	}

	return reply;
}

static int load_crtcs(struct zora_randr_state *state)
{
	struct xcb_connection_t *conn = state->conn;
	struct xcb_randr_get_screen_resources_reply_t *reply;
	unsigned int *crtcs, i, n;

	reply = load_screen_resources(conn, state->screen->root);
	if (reply == NULL)
		return -1;

	n = reply->num_crtcs;
	state->num_crtcs = n;
	state->crtcs = malloc(n * sizeof(unsigned int));

	crtcs = xcb_randr_get_screen_resources_crtcs(reply);
	for (i = 0; i < reply->num_crtcs; i++) {
		state->crtcs[i] = crtcs[i];
	}

	free(reply);

	return 0;
}

int zora_randr_initialise(struct zora_randr_state *state)
{
	int preferred, ret;

	state->conn = xcb_connect(NULL, &preferred);
	if (state->conn == NULL)
		return -1;

	state->screen = find_screen(state->conn, preferred);
	ret = load_crtcs(state);

	return ret;
}

int zora_randr_set_colour_temperature(struct zora_randr_state *state,
				      unsigned int temperature)
{
	unsigned int i;

	for (i = 0; i < state->num_crtcs; i++) {
		printf("*** %d ***\n", state->crtcs[i]);
	}

	return 0;
}

void zora_randr_finalise(struct zora_randr_state *state)
{
	xcb_disconnect(state->conn);
}
