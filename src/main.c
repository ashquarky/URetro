/* URetro - a thing for the Wii U */
/* https://github.com/QuarkTheAwesome/URetro */

#include <string.h>
#include <stdlib.h>
#include "main.h"
#include "wiiu.h"
#include "video.h"
#include "kernel.h"
#include "input.h" // /ux
#include "exception.h"
#include "cores.h"
#include "fs_utils.h"
#include "sd_fat_devoptab.h"

int __entry_menu(int argc, char** argv) {
	InitOSFunctionPointers();
	InitVPadFunctionPointers();
	InitFSFunctionPointers();
	InstallExceptionHandler();
	mount_sd_fat("sd");
	
	//Setup BATs for later
	InjectSyscall36((unsigned int)injectBAT);
	RunSyscall36();
	
	initInputs();
	
	initVideo();
	videoDebugMessage(0, "All running!");
	
	//*((int*)0x04) = 4243;
	
	unsigned char* MEM2core = 0;
	unsigned int coreSize = 0;
	LoadFileToMem("sd://stella.elf", &MEM2core, &coreSize);

	void* core = (void*)CORES_BASE_ADDRESS;
	
	memcpy(core, MEM2core, coreSize); //Move core to MEM1
	free(MEM2core);
	
	int error = loadCore(core); //Load core symbols (UDynLoad)
	
	char buf[255];
	__os_snprintf(buf, 255, "Core loaded, error %d. Core at 0x%X", error, core);
	videoDebugMessage(1, buf);
	
	unsigned char* game = 0;
	unsigned int gameSize = 0;
	LoadFileToMem("sd://game.bin", &game, &gameSize);
	
	error = setupCore((void*)game, gameSize); //Initialise core
	
	__os_snprintf(buf, 255, "Setup core, error %d", error);
	videoDebugMessage(2, buf);
	
	testVideoOutput();
	
	while (1) {
		pollInputs();
		if (UIInputCheckButton()) {
			break;
		}
	}
	
	//cleanup
	shutdownVideo();
	memset(core, 0, coreSize); //Delete core from memory or HBL will cry every time
	memset(game, 0, gameSize);
	InjectSyscall36((unsigned int)clearBAT);
	RunSyscall36();
	return 0;
}