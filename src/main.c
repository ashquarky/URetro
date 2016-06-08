/* URetro - a thing for the Wii U */
/* https://github.com/QuarkTheAwesome/URetro */

#include "main.h"
#include "wiiu.h"
#include "video.h"
#include "kernel.h"
#include "input.h" // /ux
#include "exception.h"

int __entry_menu(int argc, char** argv) {
	InitOSFunctionPointers();
	InitVPadFunctionPointers();
	OSSetExceptionCallback(2, &exception_handler);
	OSSetExceptionCallback(3, &exception_handler);
	OSSetExceptionCallback(6, &exception_handler);
	
	//Setup BATs for later
	InjectSyscall36((unsigned int)injectBAT);
	RunSyscall36();
	
	initInputs();
	
	initVideo();
	videoDebugMessage("All running!");
	finalizeFrame();
	
	while (1) {
		pollInputs();
		if (inputCheckButton(0)) {
			break;
		}
	}
	
	//cleanup
	InjectSyscall36((unsigned int)clearBAT);
	RunSyscall36();

	return 0;
}