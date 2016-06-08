/* URetro - a thing for the Wii U */
/* https://github.com/QuarkTheAwesome/URetro */

#include "main.h"
#include "wiiu.h"
#include "video.h"
#include "kernel.h"
#include "input.h" // /ux
#include "exception.h"
#include "memory.h"
#include "fs_utils.h"
#include "sd_fat_devoptab.h"

int __entry_menu(int argc, char** argv) {
	InitOSFunctionPointers();
	InitVPadFunctionPointers();
	InitFSFunctionPointers();
	initMemory();
	InstallExceptionHandler();
	mount_sd_fat("sd");
	
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