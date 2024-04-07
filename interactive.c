#include <termios.h>
#include "common.h"

void setup_interact() {
	struct termios attrs;
	tcgetattr(0, &attrs);
	attrs.c_lflag &= ~ECHO;
	if (opt.interactive)
		attrs.c_lflag &= ~ICANON;
	tcsetattr(0, 0, &attrs);
}

int handle_interact() {
	if (!opt.interactive) return 0;
	// TODO: try to read and process any input
	return 0;
}
