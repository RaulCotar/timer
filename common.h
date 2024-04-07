#ifndef _COMMON_H_
#define _COMMON_H_
#include <stdio.h> // IWYU pragma: export

#define panic(MSG, ...) {fputs(COLP, stdout); printf(MSG __VA_OPT__(,) __VA_ARGS__); puts(COLR);}
#define worry(MSG, ...) {fputs(COLW, stdout); printf(MSG __VA_OPT__(,) __VA_ARGS__); puts(COLR);}

#define strlen(STR)				__builtin_strlen((STR))
#define strcmp(LHS, RHS)		__builtin_strcmp((LHS), (RHS))
#define calloc(NELEM, ELSIZE)	__builtin_calloc((NELEM), (ELSIZE))
#define free(PTR)				__builtin_free((PTR))
#define unreachable()			__builtin_unreachable()

extern char const *COLW, *COLP, *COLR;

int cmdargs(int const argc, char const *argv[argc]);

extern struct options {
	unsigned res;
	enum:char{F_SHORT, F_FULL} fmt;
	enum:char{S_NORMAL, S_ART} style;
	bool interactive;
	bool countdown;
	long duration;
	char const *dyingwish;
} opt;

extern char *afont;
extern unsigned afont_h, afont_w;

[[gnu::cold, gnu::leaf]]
extern void read_afont(char const *filename);

[[gnu::leaf]]
extern void artprint(char const *str);

[[gnu::leaf]]
extern void setup_interact();

[[gnu::leaf]]
int handle_interact();

#endif /* _COMMON_H_ */
