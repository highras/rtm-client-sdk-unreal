/** This file is part of the Mingw32 package.
unistd.h maps (roughly) to io.h
*/
#ifndef _UNISTD_H
#define _UNISTD_H
#ifdef WIN32
#include <io.h>
#include <process.h>
#endif
#endif /* _UNISTD_H */