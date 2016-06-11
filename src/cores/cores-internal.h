/* URetro - a thing for the Wii U */
/* https://github.com/QuarkTheAwesome/URetro */

#ifndef __CORES_INTERNAL_H__
#define __CORES_INTERNAL_H__

#include "libretro.h"

void* coreHandle;

//libretro function pointers.
//Prefixed by _core_ to denote that they are functions present in the core's code.
//Should never really leave /cores on the codebase.
void (*_core_set_environment)(retro_environment_t);
void (*_core_set_video_refresh)(retro_video_refresh_t);
void (*_core_set_input_poll)(retro_input_poll_t);
void (*_core_set_input_state)(retro_input_state_t);
void (*_core_set_audio_sample)(retro_audio_sample_t);
void (*_core_set_audio_sample_batch)(retro_audio_sample_batch_t);

void (*_core_init)(void);
void (*_core_deinit)(void);
unsigned (*_core_api_version)(void);
void (*_core_get_system_info)(struct retro_system_info *info);
void (*_core_get_system_av_info)(struct retro_system_av_info *info);
void (*_core_set_controller_port_device)(unsigned port, unsigned device);
void (*_core_reset)(void);
void (*_core_run)(void);
bool (*_core_load_game)(const struct retro_game_info *game);
void (*_core_unload_game)(void);
#endif //__CORES_INTERNAL_H__