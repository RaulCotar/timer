#include <unistd.h> // for isatty
#include "common.h"

char const *COLW = "", *COLP = "", *COLR = "";
static char const *const help_msg, *const ver_nr; // defined at the bottom

struct options opt = {
	.res = R_SECOND,
	.fmt = F_SHORT,
	.style = S_NORMAL,
	.countdown = false,
	.offset = 0,
};

int cmdargs(int const argc, char const *argv[argc]) {
	if (isatty(1)) COLP = "\x1b[2;31m", COLW = "\x1b[2;33m", COLR = "\x1b[22;39m";
	for (int a=1; a<argc; a++) {
		if (argv[a][0] != '-') panic("timer (countdown) mode not supported in this version!");
		if (!strcmp(argv[a], "--help")) { puts(help_msg); puts(ver_nr); return 1; }
		if (!strcmp(argv[a], "--version")) { puts(ver_nr); return 1; }
		for (int f=1; argv[a][f]; f++)
			switch (argv[a][f]) {
			case 'a':
				opt.style = S_ART;
				read_afont("patorjk_small.afont");
				break;
			case 'f':
				opt.fmt = F_FULL;
				break;
			case 'R':
				switch (argv[a][++f]) {
				case 'h': opt.res = R_HOUR; break;
				case 'm': opt.res = R_MINUTE; break;
				case 's': opt.res = R_SECOND; break;
				case 'l': opt.res = R_MILLIS; break;
				default: panic("Unknown resolution option '%c'", argv[a][f]);
				}
				break;
			case 'r':
				worry("-r (session resume) is ignore in this version!");
				break;
			default:
				panic("Unknown option '%c'", argv[a][f]);
			}
	}
	return 0;
}

#define XX "\x1b[31m(!)\x1b[39m"
static char const *const ver_nr =
	"timer 0.9.1-alpha2\n"
	"Written by Raul Cotar, licensed under GPLv3.\n"
	"Source, documentation and support: <https://github.com/RaulCotar/timer>";
static char const *const help_msg =
	"Usage: timer [OPTIONS] [TIME]\n"
	"Simple command line stopwatch and timer."XX"\n"
	"Behaves like a stopwatch if no \x1b[3mTIME\x1b[23m is given.\n\n"
	"  -a, --art\t\t\tuse ASCII art for display\n"
	"  -f, --fullform\t\tuse \x1b[3mhh\x1b[23m:\x1b[3mmm\x1b[23m:\x1b[3mss\x1b[23m:\x1b[3mlll\x1b[23m format\n"
	"  -R, --resolution=[\x1b[3mh\x1b[23m|\x1b[3mm\x1b[23m|\x1b[3;4ms\x1b[23;24m|\x1b[3ml\x1b[23m]\tset hour/minute/second/millisecond resolution\n"
	"  -r, --resume\t\t\tresume last session"XX"\n"
	"      --help\t\t\tprint this help message and exit\n"
	"      --version\t\t\tprint version and exit\n\n"
	"\x1b[3mTIME\x1b[23m is specified as:\n  %d[h|m|s|l](:...)* with strictly increasing order of precision\n\n"
	"Examples:\n"
	"  timer -Rlf\tstopwatch with 1ms resolution displayed in full format\n"
	"  timer 1m30s\tone and a half minute timer with 1s resolution\n"
	"  timer -a\tstopwatch with ASCII art formatting and auto color\n";
