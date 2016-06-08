/* URetro - a thing for the Wii U */
/* https://github.com/QuarkTheAwesome/URetro */

#include "video.h"
#include "wiiu.h"

int buffer0Size = 0;
int buffer1Size = 0;

void initVideo() {
	OSScreenInit();
	
	buffer0Size = OSScreenGetBufferSizeEx(0);
	buffer1Size = OSScreenGetBufferSizeEx(1);
	
	OSScreenSetBufferEx(0, (void*)0xF4000000);
	OSScreenSetBufferEx(1, (void*)(0xF4000000 + buffer1Size));
	
	OSScreenEnableEx(0, 1);
	OSScreenEnableEx(1, 1);
}

void render(struct videoData data) {
	
}

void waitForFrame() {
	
}

void videoDebugMessage(char* msg) {
	OSScreenPutFontEx(0, 0, 0, msg);
	OSScreenPutFontEx(1, 0, 0, msg);
}

void finalizeFrame() {
	DCFlushRange((void*)0xF4000000, buffer0Size);
	DCFlushRange((void*)(0xF4000000 + buffer0Size), buffer1Size);
	
	OSScreenFlipBuffersEx(0);
	OSScreenFlipBuffersEx(1);
}