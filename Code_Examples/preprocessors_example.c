/*

#define sum(x, y) (x) + (y) // macro

// Check if HDR defined and if not, define it
#if !defined(HDR)
#define HDR "default.h"
#endif

// Chose header to include depending on the SYSTEM
#if SYSTEM == SYSV
	#define HDR "sysv.h"
#elif SYSTEM == BSD
	#define HDR "bsd.h"
#elif SYSTEM == MSDOS
	#define HDR "msdos.h"
#else
	#define HDR "default.h"
#endif
#include HDR

// Short method of first example (#ifdef means if defined and #ifndef means if !defined)
#ifndef HDR
#def HDR "default.h"
#endif

*/
