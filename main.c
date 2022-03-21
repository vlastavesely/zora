#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

#include "randr.h"

#define PROGNAME "zora"
#define VERSION "0.1"

static const char *usage_str =
	"Usage:\n"
	"  " PROGNAME " [OPTIONS...]\n"
	"\n"
	"Options:\n"
	"  -o TEMP, --output TEMP  the temperature in Kelvins\n"
	"  -x, --reset             reset the temperature\n"
	"  -h, --help              show this help and exit\n"
	"  -v, --version           show version number and exit\n";

static const char *short_opts = "o:xhv";

static const struct option long_opts[] = {
	{"output",     required_argument,  0, 'o'},
	{"reset",      no_argument,        0, 'x'},
	{"help",       no_argument,        0, 'h'},
	{"version",    no_argument,        0, 'v'},
	{0, 0, 0, 0}
};

static void print_usage()
{
	fprintf(stdout, "%s\n", usage_str);
}

static void print_version()
{
	fprintf(stdout, PROGNAME " " VERSION "\n");
}

int main(int argc, const char **argv)
{
	struct zora_randr_state state = ZORA_RANDR_STATE_INIT;
	int ret, c = 0, opt_index = 0;
	unsigned int temperature = 6500;

	if (argc == 1) {
		print_usage();
		return -1;
	}

	while (c != -1) {
		c = getopt_long(argc, (char * const *) argv, short_opts,
				long_opts, &opt_index);

		switch (c) {
		case 'o':
			temperature = atoi(optarg);
			break;
		case 'x':
			temperature = 6500;
			break;
		case 'h':
			print_usage();
			return 0;
		case 'v':
			print_version();
			return 0;
		default:
			break;
		}
	}

	ret = zora_randr_initialise(&state);
	if (ret != 0) {
		puts("error: failed to initialise.");
		goto out;
	}

	ret = zora_randr_set_colour_temperature(&state, temperature);
	if (ret != 0) {
		puts("error: failed to set the temperature colour.");
	}

out:
	zora_randr_finalise(&state);

	return ret;
}
