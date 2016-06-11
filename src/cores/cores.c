/* URetro - a thing for the Wii U */
/* https://github.com/QuarkTheAwesome/URetro */

#include "cores.h"
#include "cores-internal.h"
#include "libretro.h"
#include "UDynLoad.h"

int loadCore(void* coreElf) {
	int elfstatus = UDynLoad_CheckELF(coreElf);
	if (elfstatus) {
		return 0x4000 | elfstatus;
	}
	
	coreHandle = coreElf;
	
	int error = 0;
	//error |= UDynLoad_FindExport(coreHandle, 0, "retro_init", &_core_init);
	//error |= UDynLoad_FindExport(coreHandle, 0, "retro_deinit", &_core_deinit);
	error |= UDynLoad_FindExport(coreHandle, 0, "retro_api_version", &_core_api_version);
	//error |= UDynLoad_FindExport(coreHandle, 0, "retro_get_system_info", &_core_get_system_info);
	//error |= UDynLoad_FindExport(coreHandle, 0, "retro_get_system_av_info", &_core_get_system_av_info);
	//error |= UDynLoad_FindExport(coreHandle, 0, "retro_set_controller_port_device", &_core_set_controller_port_device);
	//error |= UDynLoad_FindExport(coreHandle, 0, "retro_reset", &_core_reset);
	//error |= UDynLoad_FindExport(coreHandle, 0, "retro_run", &_core_run);
	//error |= UDynLoad_FindExport(coreHandle, 0, "retro_load_game", &_core_load_game);
	//error |= UDynLoad_FindExport(coreHandle, 0, "retro_unload_game", &_core_unload_game);
	//error |= UDynLoad_FindExport(coreHandle, 0, "retro_set_environment", &_core_set_environment);
	//error |= UDynLoad_FindExport(coreHandle, 0, "retro_set_video_refresh", &_core_set_video_refresh);
	//error |= UDynLoad_FindExport(coreHandle, 0, "retro_set_input_poll", &_core_set_input_poll);
	//error |= UDynLoad_FindExport(coreHandle, 0, "retro_set_input_state", &_core_set_input_state);
	//error |= UDynLoad_FindExport(coreHandle, 0, "retro_set_audio_sample", &_core_set_audio_sample);
	//error |= UDynLoad_FindExport(coreHandle, 0, "retro_set_audio_sample_batch", &_core_set_audio_sample_batch);
	
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