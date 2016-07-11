// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <objbase.h>
#include <stdio.h>
#include <stdlib.h>

#define	snprintf	_snprintf
#define	vsnprintf	_vsnprintf
#define	spawnlp		_spawnlp
