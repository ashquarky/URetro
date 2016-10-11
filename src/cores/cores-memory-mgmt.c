/* URetro - a thing for the Wii U */
/* https://github.com/QuarkTheAwesome/URetro */

#include "wiiu.h"
#include <stdlib.h>

void* (*MEMCreateExpHeapEx)(void* mem, unsigned int size, int flags);

void* makeHeapForCore(unsigned int size) {
	unsigned int coreinit_handle;
	OSDynLoad_Acquire("coreinit", &coreinit_handle);
	OSDynLoad_FindExport(coreinit_handle, 0, "MEMCreateExpHeapEx", &MEMCreateExpHeapEx);
	void* coreHeapMem = malloc(size);
	if (!coreHeapMem) {
		return (void*)0;
	}
	void* coreHeap = MEMCreateExpHeapEx(coreHeapMem, size, 0);
	if (!coreHeap) {
		free(coreHeapMem);
		return (void*)0;
	}
	return coreHeap;
}
