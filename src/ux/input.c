/* URetro - a thing for the Wii U */
/* https://github.com/QuarkTheAwesome/URetro */

#include "input.h"
#include "wiiu.h"

//This file may need a complete redo to meet libretro standards

struct controllerData {
	int homePressed;
};
struct controllerData currentData;

void initInputs() {
	VPADInit();
}

//TODO rewrite for libretro
void pollInputs() {
	VPADData vpad;
	int error;
	VPADRead(0, &vpad, 1, &error);
	if (vpad.btns_h & 0x02) { //HOME
		currentData.homePressed = 1;
	} else {
		currentData.homePressed = 0;
	}
}
//TODO stub
int inputCheckButton(int button) {
	return currentData.homePressed;
}