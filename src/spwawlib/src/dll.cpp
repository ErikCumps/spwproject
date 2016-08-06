/** \file
 * The SPWaW Library - DllMain.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"

BOOL APIENTRY
DllMain (HANDLE /*hModule*/, DWORD  ul_reason_for_call, LPVOID /*lpReserved*/)
{
	switch (ul_reason_for_call) {
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
	}
	return (TRUE);
}
