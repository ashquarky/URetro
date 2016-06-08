/* URetro - A thing for the Wii U */
/* https://github.com/QuarkTheAwesome/URetro */

#include "wiiu.h"

//TODO write a proper memory management system

void* (*OSAllocFromSystem)(unsigned int size, unsigned int align);
void (*OSFreeToSystem)(void* mem);

void initMemory() {
	unsigned int coreinit_handle;
	OSDynLoad_Acquire("coreinit", &coreinit_handle);
	OSDynLoad_FindExport(coreinit_handle, 0, "OSAllocFromSystem", &OSAllocFromSystem);
	OSDynLoad_FindExport(coreinit_handle, 0, "OSFreeToSystem", &OSFreeToSystem);
}

void* __wrap_malloc(unsigned int size) {
	return OSAllocFromSystem(size, 4);
}

void __wrap_free(void* mem) {
	OSFreeToSystem(mem);
}

void* __wrap_memalign(unsigned int size, unsigned int align) {
	if (align < 4) {
		align = 4;
	}
	return OSAllocFromSystem(size, align);
}