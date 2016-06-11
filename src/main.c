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
	
	int error = loadCore(core);
	
	char buf[255];
	__os_snprintf(buf, 255, "Core loaded, error %d. Core at 0x%X", error, core);
	videoDebugMessage(1, buf);
	
	int api = runCore(); //runCore and setupCore are just random functions I picked and their names mean nothing as of now.
	//They will make more sense in future, however.
	
	__os_snprintf(buf, 255, "Core func is 0x%08X", api);
	videoDebugMessage(2, buf);
	
	while (1) {
		pollInputs();
		if (inputCheckButton(0xBABECAFE)) {
			break;
		}
	}
	
	int ret = setupCore();
	
	__os_snprintf(buf, 255, "OK! 0x%X", ret);
	videoDebugMessage(3, buf);
	
	while (1) {
		pollInputs();
		if (inputCheckButton(0xBABECAFE)) {
			break;
		}
	}
	
	//cleanup
	memset(core, 0, coreSize); //Delete core from memory or HBL will cry every time
	InjectSyscall36((unsigned int)clearBAT);
	RunSyscall36();
	return 0;
}