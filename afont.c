#include "common.h"

#define ALPHABET_SIZE 66 // 10 + 26*2 + 4

char *afont;
unsigned afont_h, afont_w;

[[gnu::cold]]
void read_afont(char const *filename) {
	FILE *fin = fopen(filename, "r");
	if (fscanf(fin, "afont:%u:%u%*[^\n]%*c", &afont_w, &afont_h) != 2) {
		worry("Bad afont file \"%s\"!", filename);
		fclose(fin);
		return;
	}
	free(afont);
	afont = calloc(ALPHABET_SIZE, afont_h*(++afont_w));
	for (unsigned c=0; c<ALPHABET_SIZE; c++)
		for (unsigned l=0; l<afont_h; l++)
			fscanf(fin, "%[^\n]%*c", &afont[(afont_h * c + l) * afont_w]);
	fclose(fin);
}

[[gnu::const]]
static inline unsigned afont_sym(char c) {
	if (c >= '0' && c <= '9')
		return c - '0';
	if (c >= 'A' && c <= 'Z')
		return c - 'A' + 10;
	if (c >= 'a' && c <= 'z')
		return c - 'a' + 36;
	switch (c) {
	case ' ': return 62;
	case ':': return 63;
	case '+': return 64;
	case '-': return 65;
	default: panic("Char '%c' not in afont!", c); return 62;
	}
}

[[maybe_unused]]
static void afont_dump() {
	char const *const restrict col[2] = {"\x1b[30;1;47m", "\x1b[30;1;100m"};
	unsigned const ran[] = {10, 26, 26, 4};
	unsigned rs=0;
	printf("afont: %p (%d+1)x%d\n", (void*)afont, afont_w-1, afont_h);
	for (unsigned r=0; r<sizeof(ran)/sizeof(ran[0]); rs+=ran[r++], putchar('\n'))
		for (unsigned line=0; line<afont_h; line++, puts("\x1b[K"))
			for (unsigned sym=rs; sym-rs<ran[r]; sym++)
				printf("%s%s\x1b[49m", col[sym%2], &afont[(afont_h * sym + line) * afont_w]);
}

void artprint(char const *str) {
	unsigned const len = strlen(str);
	for (unsigned line=0; line<afont_h; line++) {
		for(unsigned c=0; c<len; c++)
			fputs(&afont[(afont_h * afont_sym(str[c]) + line) * afont_w], stdout);
		puts("\x1b[K");
	}
	printf("\x1b[%cF", afont_h + '0');
	fflush(stdout);
}
