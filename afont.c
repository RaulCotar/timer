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
	afont = calloc(ALPHABET_SIZE, afont_h*(afont_w+1));
	for (unsigned c=0; c<ALPHABET_SIZE; c++)
		for (unsigned l=0; l<afont_h; l++)
			fscanf(fin, "%[^\n]%*c", &afont[(afont_h * c + l) * (afont_w + 1)]);
	fclose(fin);
}

void artprint(char const *str) {
	worry("TODO: artprint");
}
