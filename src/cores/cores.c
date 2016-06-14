/* URetro - a thing for the Wii U */
/* https://github.com/QuarkTheAwesome/URetro */

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "cores.h"
#include "cores-internal.h"
#include "libretro.h"
#include "UDynLoad.h"
#include "video.h"

#include "wiiu.h"

//Objects prefixed by _core_ are in the libretro core (the ELF we load)
//Objects prefixed by _fend_ are in the URetro Frontend (this code)
//Objects prefixed by retro or RETRO are in the libretro headers (libretro.h)

//_fend_ signatures
static void _fend_video_refresh(const void* data, unsigned width, unsigned height, size_t pitch);
static bool _fend_environment(unsigned cmd, void* data);

//Functions public to other URetro files
int loadCore(void* coreElf) {
	int elfstatus = UDynLoad_CheckELF(coreElf);
	if (elfstatus) {
		return 0x4000 | elfstatus;
	}
	
	coreHandle = coreElf;
	
	int error = 0;
	error |= UDynLoad_FindExport(coreHandle, 0, "retro_init", &_core_init);
	error |= UDynLoad_FindExport(coreHandle, 0, "retro_deinit", &_core_deinit);
	error |= UDynLoad_FindExport(coreHandle, 0, "retro_api_version", &_core_api_version);
	error |= UDynLoad_FindExport(coreHandle, 0, "retro_get_system_info", &_core_get_system_info);
	error |= UDynLoad_FindExport(coreHandle, 0, "retro_get_system_av_info", &_core_get_system_av_info);
	error |= UDynLoad_FindExport(coreHandle, 0, "retro_set_controller_port_device", &_core_set_controller_port_device);
	error |= UDynLoad_FindExport(coreHandle, 0, "retro_reset", &_core_reset);
	error |= UDynLoad_FindExport(coreHandle, 0, "retro_run", &_core_run);
	error |= UDynLoad_FindExport(coreHandle, 0, "retro_load_game", &_core_load_game);
	error |= UDynLoad_FindExport(coreHandle, 0, "retro_unload_game", &_core_unload_game);
	error |= UDynLoad_FindExport(coreHandle, 0, "retro_set_environment", &_core_set_environment);
	error |= UDynLoad_FindExport(coreHandle, 0, "retro_set_video_refresh", &_core_set_video_refresh);
	error |= UDynLoad_FindExport(coreHandle, 0, "retro_set_input_poll", &_core_set_input_poll);
	error |= UDynLoad_FindExport(coreHandle, 0, "retro_set_input_state", &_core_set_input_state);
	error |= UDynLoad_FindExport(coreHandle, 0, "retro_set_audio_sample", &_core_set_audio_sample);
	error |= UDynLoad_FindExport(coreHandle, 0, "retro_set_audio_sample_batch", &_core_set_audio_sample_batch);
	
	if (error) {
		return 0x8000 | error;
	}
	return 0;
}

int setupCore() {
	return _core_api_version();
}

int runCore() {
	return (int)_core_api_version;
}

void testVideoOutput() {
	unsigned short* data = (unsigned short*)malloc(100*100*2);
	for (int i = 0; i < (100 * 100); i++) {
		data[i] = 0xDAC6; //RGB565 orange
	}
	_fend_video_refresh((const void*)data, 100, 100, 1);
	finalizeFrame();
	memset(data, 0, 100*100*2);
	free(data);
}

//Core interface

/** environment(unsigned, void*)
 ** Used by the core to figure out a lot of things about us (the frontend).
 ** Also sets a few basic settings.
 */
static bool _fend_environment(unsigned cmd, void* data) {
	switch (cmd) {
		/*case RETRO_ENVIRONMENT_GET_LOG_INTERFACE: {
			
		}*/ //No logging for you
		case RETRO_ENVIRONMENT_GET_CAN_DUPE: {
			//Frame duping is a mysterious beast, disable it here if stuff goes wrong. AFAIK it involves passing NULL to a render call.
			*((bool*)data) = true;
			break;
		}
		case RETRO_ENVIRONMENT_SET_PIXEL_FORMAT: {
			const enum retro_pixel_format* fmt = (enum retro_pixel_format*)data;
			
			/* Having looked around and found out that the best pixel format for the Wii U (XRGB8888, easy conversion to RGBA8888 which is native)
			 * isn't supported by most cores, we have to deal with whatever we can get. I picked RGB565 though 0RGB1555 should be basically identical.
			 * For now (OSScreen) we'll have to maths it out, but once we swap to GX2 we should be able to set RGB565 as native.
			 */
			if (*fmt != RETRO_PIXEL_FORMAT_RGB565) { //If format isn't RGB565...
				return false;
			}
			//Possibly replace this with a call to a video function
			return true;
		}
		default: {
			//Unimplemented env
			return false;
		}
		return true;
	}
}

/** video_refresh(const void*, unsigned, unsigned, size_t)
 ** Used by the core to give us video data.
 ** Since our environment() says we support frame duping, any value may be NULL.
 **
 ** TODO: Possibly move this onto another CPU core?
 */
static void _fend_video_refresh(const void* data, unsigned width, unsigned height, size_t pitch) {
	if (!data | !width | !height | !pitch) {
		return;
	}
	struct videoData frame;
	frame.width = (unsigned int)width;
	frame.height = (unsigned int)height;
	frame.pixelData = (void*)data;
	frame.freeOnUse = 0;
	
	struct videoData nativeFrame;
	nativeFrame = RGB565FrameToNative(&frame);
	if (!nativeFrame.width) { //dummy output - error
		return;
	}
	renderCoreVideo(nativeFrame);
}