#ifndef acComponentsInit_h
#define acComponentsInit_h

/**
* acComponentsInit() is for initializing static data when acIcons
* is built as a static library.  Otherwise, linkers may drop out entire
* object files such as files generated by rcc or other files in acIcons.
*/

void acComponentsInit();

#endif