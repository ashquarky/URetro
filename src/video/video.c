/* URetro - a thing for the Wii U */
/* https://github.com/QuarkTheAwesome/URetro */

#include <string.h>
#include <stdlib.h>
#include "video.h"
#include "wiiu.h"

int buffer0Size = 0;
int buffer1Size = 0;

//For some reason this isn't in dynamic_libs
unsigned int (*OSScreenPutPixelEx)(unsigned int bufferNum, unsigned int posX, unsigned int posY, uint32_t color);

void initVideo() {
	unsigned int coreinit_handle;
	OSDynLoad_Acquire("coreinit", &coreinit_handle);
	OSDynLoad_FindExport(coreinit_handle, 0, "OSScreenPutPixelEx", &OSScreenPutPixelEx);
	
	OSScreenInit();
	
	buffer0Size = OSScreenGetBufferSizeEx(0);
	buffer1Size = OSScreenGetBufferSizeEx(1);
	
	OSScreenSetBufferEx(0, (void*)0xF4000000);
	OSScreenSetBufferEx(1, (void*)(0xF4000000 + buffer1Size));
	
	OSScreenEnableEx(0, 1);
	OSScreenEnableEx(1, 1);
	
	OSScreenClearBufferEx(0, 0);
	OSScreenClearBufferEx(1, 0);
	OSScreenFlipBuffersEx(0);
	OSScreenFlipBuffersEx(1);
	OSScreenClearBufferEx(0, 0);
	OSScreenClearBufferEx(1, 0);
}

void renderCoreVideo(struct videoData frame) {
	unsigned int x = 0;
	unsigned int y = 0;
	for (unsigned int i = 0; i < (frame.width * frame.height); i++) {
		OSScreenPutPixelEx(0, x, y, frame.pixelData[i]);
		OSScreenPutPixelEx(1, x, y, frame.pixelData[i]);
		x++;
		if (x >= frame.width) {
			y++;
			x = 0;
		}
		if (y >= frame.height) {
			break;
		}
	}
	if (frame.freeOnUse) {
		free(frame.pixelData);
	}
}

void waitForFrame() {
	
}

//Stops HBL crying every time
void shutdownVideo() {
	OSScreenClearBufferEx(0, 0);
	OSScreenClearBufferEx(1, 0);
	
	DCFlushRange((void*)0xF4000000, buffer0Size);
	DCFlushRange((void*)(0xF4000000 + buffer0Size), buffer1Size);
	OSScreenFlipBuffersEx(0);
	OSScreenFlipBuffersEx(1);
	
	OSScreenClearBufferEx(0, 0);
	OSScreenClearBufferEx(1, 0);
	
	DCFlushRange((void*)0xF4000000, buffer0Size);
	DCFlushRange((void*)(0xF4000000 + buffer0Size), buffer1Size);
	OSScreenFlipBuffersEx(0);
	OSScreenFlipBuffersEx(1);
	
	OSScreenEnableEx(0, 0);
	OSScreenEnableEx(1, 0);
}

//debug messages kinda throw all that finalizeFrame() stuff out the window
void videoDebugMessage(int line, char* msg) {
	OSScreenPutFontEx(0, 0, line, msg);
	OSScreenPutFontEx(1, 0, line, msg);
	DCFlushRange((void*)0xF4000000, buffer0Size);
	DCFlushRange((void*)(0xF4000000 + buffer0Size), buffer1Size);
	OSScreenFlipBuffersEx(0);
	OSScreenFlipBuffersEx(1);
	OSScreenPutFontEx(0, 0, line, msg);
	OSScreenPutFontEx(1, 0, line, msg);
	DCFlushRange((void*)0xF4000000, buffer0Size);
	DCFlushRange((void*)(0xF4000000 + buffer0Size), buffer1Size);
	OSScreenFlipBuffersEx(0);
	OSScreenFlipBuffersEx(1);
}

void finalizeFrame() {
	DCFlushRange((void*)0xF4000000, buffer0Size);
	DCFlushRange((void*)(0xF4000000 + buffer0Size), buffer1Size);
	
	OSScreenFlipBuffersEx(0);
	OSScreenFlipBuffersEx(1);
}

/** RGB565FrameToNative(struct videoData*)
 ** Converts a RGB565 videoData into native (RGBA8888). DOES NOT WORK.
 ** Makes a NEW videoData, old one can be released safely.
 **
 ** Uses a simple algorithim, see http://stackoverflow.com/a/2442617
 */
struct videoData RGB565FrameToNative(struct videoData* inputFrame) {
	struct videoData nativeFrame;
	nativeFrame.width = inputFrame->width;
	nativeFrame.height = inputFrame->height;
	nativeFrame.freeOnUse = 1; //We malloc() the frame data, so we want it freed
	
	unsigned short* inputData = (unsigned short*)(inputFrame->pixelData);
	unsigned int* nativeData = (unsigned int*)malloc(nativeFrame.width * nativeFrame.height * 4);
	memset(nativeData, 0, nativeFrame.width * nativeFrame.height * 4);
	
	for (unsigned int i = 0; i < (nativeFrame.width * nativeFrame.height); i++) {
		unsigned int colour = 0;
		colour |= 0xFF; //Alpha 100%
		colour |= ((inputData[i] & 0xF800) | ((inputData[i] & 0xE000) >> 5)) << 16; //Red
		colour |= ((inputData[i] & 0x7E0) | ((inputData[i] & 0x600) >> 6)) << 13; //Green
		colour |= (((inputData[i] & 0x1F) << 3) | ((inputData[i] & 0x1C) >> 5)) << 8; //Blue
		nativeData[i] = colour;
	}
	
	nativeFrame.pixelData = (void*)nativeData;
	return nativeFrame;
}