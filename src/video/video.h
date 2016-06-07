/* URetro - a thing for the Wii U */
/* https://github.com/QuarkTheAwesome/URetro */
#ifndef __VIDEO_H__
#define __VIDEO_H__

struct videoData {
	unsigned int width;
	unsigned int height;
	unsigned int* pixelData;
}

void initVideo();
void render(struct videoData);
void waitForFrame();

#endif //__VIDEO_H__