#include <unistd.h> // for isatty
#include "common.h"

char const *COLW = "", *COLP = "", *COLR = "";
static char const *const help_msg, *const ver_nr; // defined at the bottom

struct options opt = {
	.res = 1'000,
	.fmt = F_SHORT,
	.style = S_NORMAL,
	.interactive = true,
	.countdown = false,
	.duration = 0,
	.dyingwish = NULL,
};

static bool read_duration(char const *str, long *ret) {
	unsigned long val, ofs;
	*ret = 0;
	while (sscanf(str, "%lu%ln", &val, &ofs) == 1) {
		str += ofs;
		switch (*str) {
		case 'm':
			if (str[1]=='s') *ret += val, str++;
			else *ret += val * 60'000;
			break;
		case 'h': *ret += val * 3600'000; break;
		case 's': *ret += val * 1000; break;
		default:  goto L_read_duration_exit;
		}
		str++;
	}
	if (*str) {
		L_read_duration_exit:
		panic("Bad time formatting! (%s)", str);
		return false;
	}
	return true;
}

int cmdargs(int const argc, char const *argv[argc]) {
	if (isatty(1)) COLP = "\x1b[2;31m", COLW = "\x1b[2;33m", COLR = "\x1b[22;39m";
	for (int a=1; a<argc; a++) {
		if (argv[a][0] != '-') {
			if (opt.countdown) {
				panic("Superfluous argument \"%s\"", argv[a]);
				return -1;
			}
			opt.countdown = true;
			if (!read_duration(argv[a], &opt.duration))
				return -1;
			continue;
		}
		if (!strcmp(argv[a], "--help")) { puts(help_msg); puts(ver_nr); return 1; }
		if (!strcmp(argv[a], "--version")) { puts(ver_nr); return 1; }
		for (int f=1; argv[a][f]; f++) {
			switch (argv[a][f]) {
			case 'a':
				opt.style = S_ART;
				read_afont("patorjk_small.afont");
				break;
			case 'c':
				opt.dyingwish = &argv[a][f+1];
				f += strlen(opt.dyingwish);
				break;
			case 'f':
				opt.fmt = F_FULL;
				break;
			case 'n':
				opt.interactive = false;
				break;
			case 'R':
				long tmp;
				if (!read_duration(argv[a]+f+1, &tmp))
					return -1;
				opt.res = tmp;
				f += strlen(argv[a]+f+1);
				break;
			case 'r':
				worry("-r (session resume) is ignore in this version!");
				break;
			default:
				panic("Unknown option '%c'", argv[a][f]); return -1;
			}
		}
	}
	return 0;
}

#define XX "\x1b[31m(!)\x1b[39m"
static char const *const ver_nr =
	"timer 0.9.4-alpha5\n"
	"Written by Raul Cotar, licensed under GPLv3.\n"
	"Source, documentation and support: <https://github.com/RaulCotar/timer>";
static char const *const help_msg =
	"Usage: timer [OPTIONS] [TIME]\n"
	"Simple command line stopwatch and timer.\n"
	"Behaves like a stopwatch if no \x1b[3mTIME\x1b[23m is given.\n\n"
	"  -a, --art\t\t\tuse ASCII art for display\n"
	"  -c, --endcmd=\x1b[3mCMD\x1b[23m\t\trun the given command in sh at program exit\n"
	"  -f, --fullform\t\tuse \x1b[3mhh\x1b[23m:\x1b[3mmm\x1b[23m:\x1b[3mss\x1b[23m:\x1b[3mlll\x1b[23m format\n"
	"  -n, --no-interact\t\trun in non-interactive mode\n"
	"  -R, --resolution=\x1b[3mRES\x1b[23m\t\tset timer resolution (default: 1s)\n"
	"  -r, --resume\t\t\tresume last session"XX"\n"
	"      --help\t\t\tprint this help message and exit\n"
	"      --version\t\t\tprint version and exit\n\n"
	"\x1b[3mTIME\x1b[23m and \x1b[3mRES\x1b[23m are specified as: \x1b[3m([[:uint:]][hmsl])+\x1b[23m\n\n"
	"Examples:\n"
	"  timer -Rlf\tstopwatch with 1ms resolution displayed in full format\n"
	"  timer 1m30s\tone and a half minute timer with 1s resolution\n"
	"  timer -a\tstopwatch with ASCII art formatting and auto color\n";
