#ifndef	SPWAW_LIB_API_H
#define	SPWAW_LIB_API_H	1

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SPWAWLIB_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// SPWAWLIB_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef SPWAWLIB_EXPORTS
#define SPWAWLIB_API __declspec(dllexport)
#else
#define SPWAWLIB_API __declspec(dllimport)
#endif

/* SPWAW API error code */
typedef enum e_SPWAW_ERROR {
	SPWERR_OK = 0,
	SPWERR_FAILED,
	SPWERR_NULLARG,
	SPWERR_OUTOFMEM,
	SPWERR_NOTINIT,
	SPWERR_NOOOBFILES,
	SPWERR_BADOOB,
	SPWERR_BADSAVEGAME,
	SPWERR_BADSAVEDATA,
	SPWERR_BADSTABDATA,
	SPWERR_BADSTABREFCNT,
	SPWERR_BADSBUFDATA,
	SPWERR_FOFAILED,
	SPWERR_FRFAILED,
	SPWERR_FWFAILED,
	SPWERR_CORRUPT,
	SPWERR_BADDATE,
	SPWERR_BADSTAMP,
	SPWERR_BADPERIOD,
	SPWERR_NOMATCH_OOBDATA,
	SPWERR_NOMATCH_OOB,
	SPWERR_NOMATCH_CORECNT,
	SPWERR_DUPTURN,
	SPWERR_LAST_CODE = SPWERR_DUPTURN
} SPWAW_ERROR;

extern SPWAWLIB_API const char *	SPWAW_errstr		(SPWAW_ERROR e);

#define	SPWAW_HAS_ERROR(_rc)	(_rc != SPWERR_OK)

#endif	/* SPWAW_LIB_API_H */