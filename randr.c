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
	xcb_connection_t *conn = state->conn;
	xcb_randr_get_screen_resources_reply_t *reply;
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

static int crtc_get_ramp_size(struct xcb_connection_t *conn, unsigned int crtc)
{
	xcb_randr_get_crtc_gamma_size_cookie_t cookie;
	xcb_randr_get_crtc_gamma_size_reply_t *reply;
	xcb_generic_error_t *error = NULL;
	int ret;

	cookie = xcb_randr_get_crtc_gamma_size(conn, crtc);
	reply = xcb_randr_get_crtc_gamma_size_reply(conn, cookie, &error);

	if (reply) {
		ret = reply->size;
		free(reply);
	} else {
		ret = -1;
		free(error);
	}

	return ret;
}

static void initialise_gamma_ramp(unsigned short *r, unsigned short *g,
				  unsigned short *b, unsigned int len)
{
	unsigned short x;
	unsigned int i;

	for (i = 0; i < len; i++) {
		x = (double) i / len * (UINT16_MAX + 1);
		r[i] = x;
		g[i] = x;
		b[i] = x;
	}
}

static int apply_gamma_ramp(struct xcb_connection_t *conn, unsigned int crtc,
			    unsigned int len, unsigned short *r,
			    unsigned short *g, unsigned short *b)
{
	xcb_void_cookie_t cookie;
	xcb_generic_error_t *error = NULL;

	cookie = xcb_randr_set_crtc_gamma_checked(conn, crtc, len, r, g, b);
	error = xcb_request_check(conn, cookie);

	if (error) {
		free(error);
		return -1;
	}

	return 0;
}

static int set_colour_temperature(struct zora_randr_state *state,
				  unsigned int crtc, unsigned int temperature)
{
	unsigned short *ramp, *r, *g, *b;
	int len, ret;

	len = crtc_get_ramp_size(state->conn, crtc);
	if (len == -1)
		return -1;

	ramp = malloc(sizeof(unsigned short) * len * 3);

	r = ramp + (0 * len);
	g = ramp + (1 * len);
	b = ramp + (2 * len);

	initialise_gamma_ramp(r, g, b, len);

	if (temperature != 6500) {
		/* 6500K is neutral colour */
		/* TODO */
	}

	ret = apply_gamma_ramp(state->conn, crtc, len, r, g, b);

	free(ramp);

	return ret;
}

int zora_randr_set_colour_temperature(struct zora_randr_state *state,
				      unsigned int temperature)
{
	unsigned int i, crtc;
	int ret;

	for (i = 0; i < state->num_crtcs; i++) {
		crtc = state->crtcs[i];
		ret = set_colour_temperature(state, crtc, temperature);
		if (ret != 0) {
			return ret;
		}
	}

	return 0;
}

void zora_randr_finalise(struct zora_randr_state *state)
{
	xcb_disconnect(state->conn);
}
