/* URetro - a thing for the Wii U */
/* https://github.com/QuarkTheAwesome/URetro */
#ifndef __INPUT_H__
#define __INPUT_H__

void initInputs();
void pollInputs();
int UIInputCheckButton(); //libretro cores will poll the whole controller at once, so this may not be needed
int inputCheckButton(int controller, unsigned int button);

#endif //__INPUT_H__