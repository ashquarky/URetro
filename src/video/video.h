/* URetro - a thing for the Wii U */
/* https://github.com/QuarkTheAwesome/URetro */
#ifndef __VIDEO_H__
#define __VIDEO_H__

//See .c file for documentation

struct videoData {
	unsigned int width;
	unsigned int height;
	unsigned int* pixelData;
	
	unsigned char freeOnUse;
};

void clearBuffer();
void initVideo();
void renderCoreVideo(struct videoData frame);
void videoDebugMessage(int line, char* msg);
void finalizeFrame();
void waitForVSync();
struct videoData RGB565FrameToNative(struct videoData* inputFrame);
struct videoData XRGB8888FrameToNative(struct videoData* inputFrame);
void shutdownVideo();

#endif //__VIDEO_H__