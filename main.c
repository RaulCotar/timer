#define _POSIX_C_SOURCE 199309L // for nanosleep
#include <time.h>
#include "common.h"

[[gnu::const]]
static inline struct timespec timespec_dif(struct timespec const l, struct timespec const r) {
	long s = l.tv_sec - r.tv_sec;
	long ms = l.tv_nsec - r.tv_nsec;
	if (ms < 0 && s > 0)
			ms = 1'000'000'000 + ms, s--;
	return (struct timespec) {s, ms};
}

void tprint(char const *str) {
	printf("\x1b[1K\x0d""now:%s", str);
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
		if (l && opt.res == R_MILLIS)
			top += snprintf(top, buf+sizeof(buf)-top, " %ldms", l);
	}
	else {
		snprintf(buf, sizeof buf, " %02ld:%02ld:%02ld:%03ld", h, m, s, l);
	}
	return buf;
}

int main(int argc, char *argv[argc]) {
	struct timespec const stime;
	timespec_get((struct timespec*)&stime, TIME_UTC);
	{
		int const r = cmdargs(argc, (char const **)argv);
		if (r == 1) return 0; // --help and --version
		if (r) return r;
	}

	struct timespec interval;
	switch (opt.res) {
		case R_HOUR: 	interval = (struct timespec){3600, 0}; break;
		case R_MINUTE:	interval = (struct timespec){60, 0}; break;
		case R_SECOND:	interval = (struct timespec){1, 0}; break;
		case R_MILLIS:	interval = (struct timespec){0, 1'000'000}; break;
	}

	do {
		struct timespec ctime;
		timespec_get(&ctime, TIME_UTC);

		struct timespec const dt = timespec_dif(ctime, stime);
		long const h = dt.tv_sec / 3600;
		long const m = dt.tv_sec % 3600 / 60;
		long const s = dt.tv_sec % 60;
		long const ms = dt.tv_nsec / 1'000'000;
		char const *const str = tformat(h, m, s, ms);
		if (opt.style == S_ART)
			artprint(str);
		else
			printf("now:%s\x1b[K\r", str), fflush(stdout);
	} while (!nanosleep(&interval, NULL));

	return 0;
}
