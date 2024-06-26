#define _POSIX_C_SOURCE 199309L // for nanosleep
#include <time.h>
#include <stdlib.h> // for system(dyingwish)
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
		if (m || (opt.res < 3600'000 && opt.res >= 60'000))
			top += snprintf(top, buf+sizeof(buf)-top, " %ldm", m);
		if (s || opt.res < 60'000)
			top += snprintf(top, buf+sizeof(buf)-top, " %lds", s);
		if (l && opt.res < 1'000)
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
	setup_interact();

	struct timespec interval = (struct timespec) {opt.res / 1000, opt.res % 1000};
	struct timespec const ttime = {opt.duration / 1000, opt.duration % 1000};

	do {
		struct timespec ctime;
		timespec_get(&ctime, TIME_UTC);

		struct timespec dt = timespec_dif(ctime, stime);
		if (opt.countdown)
			dt = timespec_dif(ttime, dt);
		long const h = dt.tv_sec / 3600;
		long const m = dt.tv_sec % 3600 / 60;
		long const s = dt.tv_sec % 60;
		long const ms = dt.tv_nsec / 1'000'000;
		char const *const str = tformat(h, m, s, ms);
		if (opt.style == S_ART)
			artprint(str);
		else
			printf("now:%s\x1b[K\r", str), fflush(stdout);
	
		handle_interact();

		if (opt.countdown && (dt.tv_sec < 0 || dt.tv_nsec < 0))
			break;
	} while (!nanosleep(&interval, NULL));
	
	if (opt.dyingwish)
		putchar('\n'), system(opt.dyingwish);

	return 0;
}
