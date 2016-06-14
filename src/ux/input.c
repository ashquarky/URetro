/* URetro - a thing for the Wii U */
/* https://github.com/QuarkTheAwesome/URetro */

#include "input.h"
#include "wiiu.h"

//This file may need a complete redo to meet libretro standards

struct controllerData {
	unsigned int buttonsPressed;
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
	
	if (error) {
		return;
	}
	
	currentData.homePressed = vpad.btns_h & VPAD_BUTTON_HOME; //For UI
	currentData.buttonsPressed = vpad.btns_h;
}
//TODO stub
int UIInputCheckButton() {
	return currentData.homePressed;
}

int inputCheckButton(int controller, unsigned int button) {
	if (currentData.buttonsPressed & button) {
		return 1;
	}
	return 0;
}