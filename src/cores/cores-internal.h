/* URetro - a thing for the Wii U */
/* https://github.com/QuarkTheAwesome/URetro */

#ifndef __CORES_INTERNAL_H__
#define __CORES_INTERNAL_H__

#include "libretro.h"

void* coreHandle;

//cores-memory-mgmt.c
void* makeHeapForCore(unsigned int size);

//Private function signatures from cores.c
static void _fend_video_refresh(const void* data, unsigned width, unsigned height, size_t pitch);
static bool _fend_environment(unsigned cmd, void* data);
static int16_t _fend_input_state(unsigned port, unsigned device, unsigned index, unsigned id);
static void _fend_input_poll(void);
static void _fend_audio_sample(int16_t left, int16_t right);
static size_t _fend_audio_sample_batch(const int16_t *data, size_t frames);
static void _fend_log(enum retro_log_level level, const char* fmt, ...) ;

//Custom additions to the API. Don't tell the libretro team.
void (*_core_uretro_set_heap)(void* heap);

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
