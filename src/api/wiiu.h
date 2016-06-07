/* URetro - a thing for the Wii U */
/* https://github.com/QuarkTheAwesome/URetro */
#ifndef __WIIU_DYNLIBS_H__
#define __WIIU_DYNLIBS_H__

void (*OSScreenInit)();

void setupDynamicLibs() {
	unsigned int coreinit_handle;
	OSDynLoad_Acquire("coreinit", &coreinit_handle);
	
	OSDynLoad_FindExport(coreinit_handle, 0, "OSScreenInit", &OSScreenInit);
}

#endif //__WIIU_DYNLIBS_H__