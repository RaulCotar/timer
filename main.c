#include <stdio.h>
#include <unistd.h> // for isatty
#define __USE_POSIX199309 // for nanosleep
#include <time.h>
#include "artfonts.h"

#define panic(MSG, ...) {fputs(COLP, stdout); printf(MSG __VA_OPT__(,) __VA_ARGS__); puts(COLR); return -1;}
#define worry(MSG, ...) {fputs(COLW, stdout); printf(MSG __VA_OPT__(,) __VA_ARGS__); puts(COLR); return -1;}

static char const *COLW = "", *COLP = "", *COLR = "";

[[gnu::leaf, gnu::pure]]
static bool scmp(char const *var, char const *lit) {
	for (unsigned i=0; lit[i]; i++)
		if (var[i] != lit[i])
			return 1;
	return 0;
}

[[gnu::leaf, gnu::pure]]
static inline unsigned slen(char const *restrict str) {
	unsigned l = 0;
	while (str[l]) l++;
	return l;
}

static char const *const help_msg, *const ver_nr; // defined at the bottom

static struct options {
	enum:char{R_HOUR, R_MINUTE, R_SECOND, R_MILLIS} res;
	enum:char{F_SHORT, F_FULL} fmt;
	enum:char{S_NORMAL, S_ART_BOX, S_ART_PSMALL} style;
	bool countdown;
	long offset;
} opt = {
	.res = R_SECOND,
	.fmt = F_SHORT,
	.style = S_NORMAL,
	.countdown = false,
	.offset = 0l,
};

int cmdargs(int const argc, char const *argv[argc]) {
	if (isatty(1)) COLP = "\x1b[2;31m", COLW = "\x1b[2;33m", COLR = "\x1b[22;39m";
	enum {ARG_OFS} needargs[argc-1];
	unsigned nargs = 0;
	for (int a=1; a<argc; a++) {
		if (argv[a][0] != '-') panic("timer (countdown) mode not supported in this version!");
		if (!scmp(argv[a], "--help")) { puts(help_msg); puts(ver_nr); return 1; }
		if (!scmp(argv[a], "--version")) { puts(ver_nr); return 1; }
		for (int f=1; argv[a][f]; f++)
			switch (argv[a][f]) {
			case 'c':
				COLP = "", COLW = "", COLR = "";
				break;
			case 'C':
				COLP = "\x1b[2;31m", COLW = "\x1b[2;33m", COLR = "\x1b[22;39m";
				break;
			case 'f':
				opt.fmt = F_FULL;
				break;
			case 'o':
				needargs[nargs++] = ARG_OFS;
				worry("-o (offset) is ignored in this version!");
				break;
			case 'r':
				switch (argv[a][++f]) {
				case 'h': opt.res = R_HOUR; break;
				case 'm': opt.res = R_MINUTE; break;
				case 's': opt.res = R_SECOND; break;
				case 'l': opt.res = R_MILLIS; break;
				default: panic("Unknown resolution option '%c'", argv[a][f]);
				}
				break;
			case 's':
				if (argv[a][++f] >= '0' && argv[a][f] < '3')
					opt.style = argv[a][f] - '0';
				else
					panic("unknown style option '%c'", argv[a][f]);
				break;
			default:
				panic("Unknown option '%c'", argv[a][f]);
			}
	}
	return 0;
}

void print_art(unsigned const height, char const *const (*const font)[14][height], char const* str) {
	unsigned len = slen(str);
	for (unsigned line=0; line<height; line++) {
		for (unsigned c=0, id; c<len; c++) {
			switch (str[c]) {
			case 'h': id = 10; break;
			case 'm': id = 11; break;
			case 's': id = 12; break;
			case ':': id = 13; break;
			case ' ': id = 14; break;
			default: id = str[c] - '0'; // we don't handle non-mappable chars for now
			}
			fputs((*font)[id][line], stdout);
		}
		fputs("\x1b[K\n", stdout);
	}
	printf("\x1b[%cF", height+'0');
}

void tprint(char const *str) {
	switch (opt.style) {
	case S_ART_BOX:
		print_art(artfont_boxdraw_height, &artfont_boxdraw, str);
		break;
	case S_ART_PSMALL:
		print_art(artfont_patorjk_small_height, &artfont_patorjk_small, str);
		break;
	default:
		printf("\x1b[1K\x0d""now:%s", str);
	}
}

char *tformat(long const h, long const m, long const s, long const l) {
	static char buf[128];
	if (opt.fmt == F_SHORT) {
		char *top = buf;
		if (h)
			top += snprintf(top, buf+sizeof(buf)-top, " %ldh", h);
		if ((m && opt.res > R_MINUTE) || opt.res == R_MINUTE)
			top += snprintf(top, buf+sizeof(buf)-top, " %ldm", m);
		if ((s && opt.res > R_SECOND) || opt.res == R_SECOND)
			top += snprintf(top, buf+sizeof(buf)-top, " %lds", s);
		if ((l && opt.res > R_MILLIS) || opt.res == R_MILLIS)
			top += snprintf(top, buf+sizeof(buf)-top, " %ldms", l);
	}
	else {
		snprintf(buf, sizeof buf, "%2ld:%02ld:%02ld:%03ld", h, m, s, l);
	}
	return buf;
}

int main(int argc, char *argv[argc]) {
	struct timespec const stime;
	timespec_get((struct timespec*)&stime, TIME_UTC);
	{
		int const r = cmdargs(argc, (char const **)argv);
		if (r == 1) return 0;
		if (r) return r;
	}

	struct timespec rqtp; switch (opt.res) {
		case R_HOUR: 	rqtp = (struct timespec){3600, 0}; break;
		case R_MINUTE:	rqtp = (struct timespec){60, 0}; break;
		case R_SECOND:	rqtp = (struct timespec){1, 0}; break;
		case R_MILLIS:	rqtp = (struct timespec){0, 1'000'000}; break;
	}
	do {
		struct timespec ctime;
		timespec_get(&ctime, TIME_UTC);
		ctime.tv_sec -= stime.tv_sec;
		ctime.tv_nsec -= stime.tv_nsec;
		if (ctime.tv_nsec < 0) {
			ctime.tv_nsec = 1'000'000'000 + ctime.tv_nsec;
			ctime.tv_sec--;
		}
		long const h = ctime.tv_sec / 3600;
		long const m = ctime.tv_sec % 3600 / 60;
		long const s = ctime.tv_sec % 60;
		long const ms = ctime.tv_nsec / 1'000'000;
		
		tprint(tformat(h, m, s, ms));
		fflush(stdout);
	} while (!nanosleep(&rqtp, NULL));

	return 0;
}

static char const *const ver_nr =
	"timer 0.9.0-beta1\n"
	"Written by Raul Cotar, licensed under GPLv3.\n"
	"Source, documentation and support: <https://github.com/RaulCotar/timer>";
static char const *const help_msg =
	"Usage: timer [OPTION] [TIME]\n"
	"Simple command line stopwatch and timer.\n\n"
	"Behaves like a stopwatch if no TIME is given.\n\n"
	"  -c\t\tforce prety printing off\n"
	"  -C\t\tforce prety printing on (default: auto)\n"
	"  -f\t\tuse HH:MM:SS:LLL format\n"
	"  -o OFS\tstart with an offset, see input format below\n"
	"  -r[hmsl]\tset hour/minute/second/millisecond resolution (default: second)\n"
	"  -s[012]\tset display style: normal, box drawing, ASCII art\n"
	"  --help\tprint this help message and exit\n"
	"  --version\tprint version and exit\n\n"
	"TIME and OFS are specified as:\n  (%d[hmsl])+ with strictly increasing order of precision\n\n"
	"Examples:\n"
	"  timer -rlf\tstopwatch with 1ms resolution displayed in full format\n"
	"  timer 1m30s\tone and a half minute timer with 1s resolution\n"
	"  timer -a\tstopwatch with ASCII art formatting and auto color\n";
