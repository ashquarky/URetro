/* URetro - a thing for the Wii U */
/* https://github.com/QuarkTheAwesome/URetro */

#include "main.h"
#include "video.h"

int __entry_menu(int argc, char** argv) {
	//HBL entrypoint
	initVideo();
	return 1;
}