/* URetro - a thing for the Wii U */
/* https://github.com/QuarkTheAwesome/URetro */

//We probably don't need all 64MB for cores.
#define CORES_BASE_ADDRESS 0xE0000000
#define CORES_MAX_ADDRESS 0xE4000000
#define CORES_ADDRESS_SIZE 0x04000000

#ifndef __CORES_H__
#define __CORES_H__

int loadCore(void* coreElf);
int setupCore();
int runCore();

#endif //__CORES_H__