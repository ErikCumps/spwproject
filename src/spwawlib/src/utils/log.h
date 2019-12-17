/** \file
 * The SPWaW Library - utility code: logging.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	LOG_H
#define	LOG_H	1

extern void	log_init	(char *log, bool append=false);
extern void	log		(char *fmt, ...);
extern void	log_disable	(void);
extern void	log_enable	(void);
extern void	log_shutdown	(void);

#include "build_options.h"

#if	IOLOGGING

#define	IOLOG0(m_)					do { log (m_); } while (0)
#define	IOLOG1(m_,a1_)					do { log (m_, a1_); } while (0)
#define	IOLOG2(m_,a1_,a2_)				do { log (m_, a1_, a2_); } while (0)
#define	IOLOG3(m_,a1_,a2_,a3_)				do { log (m_, a1_, a2_, a3_); } while (0)

#else	/* !IOLOGGING */

#define	IOLOG0(m_)
#define	IOLOG1(m_,a1_)
#define	IOLOG2(m_,a1_,a2_)
#define	IOLOG3(m_,a1_,a2_,a3_)

#endif	/* !IOLOGGING */

#if	PACKLOGGING

#define	PACKLOG0(m_)					do { log (m_); } while (0)
#define	PACKLOG1(m_,a1_)				do { log (m_, a1_); } while (0)
#define	PACKLOG2(m_,a1_,a2_)				do { log (m_, a1_, a2_); } while (0)
#define	PACKLOG3(m_,a1_,a2_,a3_)			do { log (m_, a1_, a2_, a3_); } while (0)
#define	PACKLOG4(m_,a1_,a2_,a3_,a4_)			do { log (m_, a1_, a2_, a3_, a4_); } while (0)
#define	PACKLOG5(m_,a1_,a2_,a3_,a4_,a5_)		do { log (m_, a1_, a2_, a3_, a4_, a5_); } while (0)
#define	PACKLOG6(m_,a1_,a2_,a3_,a4_,a5_,a6_)		do { log (m_, a1_, a2_, a3_, a4_, a5_, a6_); } while (0)

#else	/* !PACKLOGGING */

#define	PACKLOG0(m_)
#define	PACKLOG1(m_,a1_)
#define	PACKLOG2(m_,a1_,a2_)
#define	PACKLOG3(m_,a1_,a2_,a3_)
#define	PACKLOG4(m_,a1_,a2_,a3_,a4_)
#define	PACKLOG5(m_,a1_,a2_,a3_,a4_,a5_)
#define	PACKLOG6(m_,a1_,a2_,a3_,a4_,a5_,a6_)

#endif	/* !PACKLOGGING */

#if	UFDLOGGING

#define	UFDLOG0(m_)					do { log (m_); } while (0)
#define	UFDLOG1(m_,a1_)					do { log (m_, a1_); } while (0)
#define	UFDLOG2(m_,a1_,a2_)				do { log (m_, a1_, a2_); } while (0)
#define	UFDLOG3(m_,a1_,a2_,a3_)				do { log (m_, a1_, a2_, a3_); } while (0)
#define	UFDLOG4(m_,a1_,a2_,a3_,a4_)			do { log (m_, a1_, a2_, a3_, a4_); } while (0)
#define	UFDLOG5(m_,a1_,a2_,a3_,a4_,a5_)			do { log (m_, a1_, a2_, a3_, a4_, a5_); } while (0)
#define	UFDLOG6(m_,a1_,a2_,a3_,a4_,a5_,a6_)		do { log (m_, a1_, a2_, a3_, a4_, a5_, a6_); } while (0)
#define	UFDLOG7(m_,a1_,a2_,a3_,a4_,a5_,a6_,a7_)		do { log (m_, a1_, a2_, a3_, a4_, a5_, a6_, a7_); } while (0)
#define	UFDLOG8(m_,a1_,a2_,a3_,a4_,a5_,a6_,a7_,a8_)	do { log (m_, a1_, a2_, a3_, a4_, a5_, a6_, a7_, a8_); } while (0)

#else	/* !UFDLOGGING */

#define	UFDLOG0(m_)
#define	UFDLOG1(m_,a1_)
#define	UFDLOG2(m_,a1_,a2_)
#define	UFDLOG3(m_,a1_,a2_,a3_)
#define	UFDLOG4(m_,a1_,a2_,a3_,a4_)
#define	UFDLOG5(m_,a1_,a2_,a3_,a4_,a5_)
#define	UFDLOG6(m_,a1_,a2_,a3_,a4_,a5_,a6_)
#define	UFDLOG7(m_,a1_,a2_,a3_,a4_,a5_,a6_,a7_)
#define	UFDLOG8(m_,a1_,a2_,a3_,a4_,a5_,a6_,a7_,a8_)

#endif	/* !UFDLOGGING */

#if	UFDTRACING

#define	UFDTRACE0(m_)					do { log (m_); } while (0)
#define	UFDTRACE1(m_,a1_)				do { log (m_, a1_); } while (0)
#define	UFDTRACE2(m_,a1_,a2_)				do { log (m_, a1_, a2_); } while (0)
#define	UFDTRACE3(m_,a1_,a2_,a3_)			do { log (m_, a1_, a2_, a3_); } while (0)
#define	UFDTRACE4(m_,a1_,a2_,a3_,a4_)			do { log (m_, a1_, a2_, a3_, a4_); } while (0)
#define	UFDTRACE5(m_,a1_,a2_,a3_,a4_,a5_)		do { log (m_, a1_, a2_, a3_, a4_, a5_); } while (0)
#define	UFDTRACE6(m_,a1_,a2_,a3_,a4_,a5_,a6_)		do { log (m_, a1_, a2_, a3_, a4_, a5_, a6_); } while (0)
#define	UFDTRACE7(m_,a1_,a2_,a3_,a4_,a5_,a6_,a7_)	do { log (m_, a1_, a2_, a3_, a4_, a5_, a6_, a7_); } while (0)

#else	/* !UFDTRACING */

#define	UFDTRACE0(m_)
#define	UFDTRACE1(m_,a1_)
#define	UFDTRACE2(m_,a1_,a2_)
#define	UFDTRACE3(m_,a1_,a2_,a3_)
#define	UFDTRACE4(m_,a1_,a2_,a3_,a4_)
#define	UFDTRACE5(m_,a1_,a2_,a3_,a4_,a5_)
#define	UFDTRACE6(m_,a1_,a2_,a3_,a4_,a5_,a6_)
#define	UFDTRACE7(m_,a1_,a2_,a3_,a4_,a5_,a6_,a7_)

#endif	/* !UFDTRACING */

#if	RATRACING

#define	RATRACE0(m_)					do { log (m_); } while (0)
#define	RATRACE1(m_,a1_)				do { log (m_, a1_); } while (0)
#define	RATRACE2(m_,a1_,a2_)				do { log (m_, a1_, a2_); } while (0)
#define	RATRACE3(m_,a1_,a2_,a3_)			do { log (m_, a1_, a2_, a3_); } while (0)
#define	RATRACE4(m_,a1_,a2_,a3_,a4_)			do { log (m_, a1_, a2_, a3_, a4_); } while (0)
#define	RATRACE5(m_,a1_,a2_,a3_,a4_,a5_)		do { log (m_, a1_, a2_, a3_, a4_, a5_); } while (0)
#define	RATRACE6(m_,a1_,a2_,a3_,a4_,a5_,a6_)		do { log (m_, a1_, a2_, a3_, a4_, a5_, a6_); } while (0)
#define	RATRACE7(m_,a1_,a2_,a3_,a4_,a5_,a6_,a7_)	do { log (m_, a1_, a2_, a3_, a4_, a5_, a6_, a7_); } while (0)

#else	/* !RATRACING */

#define	RATRACE0(m_)
#define	RATRACE1(m_,a1_)
#define	RATRACE2(m_,a1_,a2_)
#define	RATRACE3(m_,a1_,a2_,a3_)
#define	RATRACE4(m_,a1_,a2_,a3_,a4_)
#define	RATRACE5(m_,a1_,a2_,a3_,a4_,a5_)
#define	RATRACE6(m_,a1_,a2_,a3_,a4_,a5_,a6_)
#define	RATRACE7(m_,a1_,a2_,a3_,a4_,a5_,a6_,a7_)

#endif	/* !RATRACING */

#if	RASCORETRACING

#define	RASCORETRACE0(m_)				do { log (m_); } while (0)
#define	RASCORETRACE1(m_,a1_)				do { log (m_, a1_); } while (0)
#define	RASCORETRACE2(m_,a1_,a2_)			do { log (m_, a1_, a2_); } while (0)
#define	RASCORETRACE3(m_,a1_,a2_,a3_)			do { log (m_, a1_, a2_, a3_); } while (0)
#define	RASCORETRACE4(m_,a1_,a2_,a3_,a4_)		do { log (m_, a1_, a2_, a3_, a4_); } while (0)
#define	RASCORETRACE5(m_,a1_,a2_,a3_,a4_,a5_)		do { log (m_, a1_, a2_, a3_, a4_, a5_); } while (0)
#define	RASCORETRACE6(m_,a1_,a2_,a3_,a4_,a5_,a6_)	do { log (m_, a1_, a2_, a3_, a4_, a5_, a6_); } while (0)
#define	RASCORETRACE7(m_,a1_,a2_,a3_,a4_,a5_,a6_,a7_)	do { log (m_, a1_, a2_, a3_, a4_, a5_, a6_, a7_); } while (0)

#else	/* !RASCORETRACING */

#define	RASCORETRACE0(m_)
#define	RASCORETRACE1(m_,a1_)
#define	RASCORETRACE2(m_,a1_,a2_)
#define	RASCORETRACE3(m_,a1_,a2_,a3_)
#define	RASCORETRACE4(m_,a1_,a2_,a3_,a4_)
#define	RASCORETRACE5(m_,a1_,a2_,a3_,a4_,a5_)
#define	RASCORETRACE6(m_,a1_,a2_,a3_,a4_,a5_,a6_)
#define	RASCORETRACE7(m_,a1_,a2_,a3_,a4_,a5_,a6_,a7_)

#endif	/* !RASCORETRACING */

#if	UHTLOGGING

#define	UHTLOG0(m_)					do { log (m_); } while (0)
#define	UHTLOG1(m_,a1_)					do { log (m_, a1_); } while (0)
#define	UHTLOG2(m_,a1_,a2_)				do { log (m_, a1_, a2_); } while (0)
#define	UHTLOG3(m_,a1_,a2_,a3_)				do { log (m_, a1_, a2_, a3_); } while (0)
#define	UHTLOG4(m_,a1_,a2_,a3_,a4_)			do { log (m_, a1_, a2_, a3_, a4_); } while (0)
#define	UHTLOG5(m_,a1_,a2_,a3_,a4_,a5_)			do { log (m_, a1_, a2_, a3_, a4_, a5_); } while (0)
#define	UHTLOG6(m_,a1_,a2_,a3_,a4_,a5_,a6_)		do { log (m_, a1_, a2_, a3_, a4_, a5_, a6_); } while (0)
#define	UHTLOG7(m_,a1_,a2_,a3_,a4_,a5_,a6_,a7_)		do { log (m_, a1_, a2_, a3_, a4_, a5_, a6_, a7_); } while (0)
#define	UHTLOG8(m_,a1_,a2_,a3_,a4_,a5_,a6_,a7_,a8_)	do { log (m_, a1_, a2_, a3_, a4_, a5_, a6_, a7_, a8_); } while (0)

#else	/* !UHTLOGGING */

#define	UHTLOG0(m_)
#define	UHTLOG1(m_,a1_)
#define	UHTLOG2(m_,a1_,a2_)
#define	UHTLOG3(m_,a1_,a2_,a3_)
#define	UHTLOG4(m_,a1_,a2_,a3_,a4_)
#define	UHTLOG5(m_,a1_,a2_,a3_,a4_,a5_)
#define	UHTLOG6(m_,a1_,a2_,a3_,a4_,a5_,a6_)
#define	UHTLOG7(m_,a1_,a2_,a3_,a4_,a5_,a6_,a7_)
#define	UHTLOG8(m_,a1_,a2_,a3_,a4_,a5_,a6_,a7_,a8_)

#endif	/* !UHTLOGGING */

#if	UHTTRACING

#define	UHTTRACE0(m_)					do { log (m_); } while (0)
#define	UHTTRACE1(m_,a1_)				do { log (m_, a1_); } while (0)
#define	UHTTRACE2(m_,a1_,a2_)				do { log (m_, a1_, a2_); } while (0)
#define	UHTTRACE3(m_,a1_,a2_,a3_)			do { log (m_, a1_, a2_, a3_); } while (0)
#define	UHTTRACE4(m_,a1_,a2_,a3_,a4_)			do { log (m_, a1_, a2_, a3_, a4_); } while (0)
#define	UHTTRACE5(m_,a1_,a2_,a3_,a4_,a5_)		do { log (m_, a1_, a2_, a3_, a4_, a5_); } while (0)
#define	UHTTRACE6(m_,a1_,a2_,a3_,a4_,a5_,a6_)		do { log (m_, a1_, a2_, a3_, a4_, a5_, a6_); } while (0)
#define	UHTTRACE7(m_,a1_,a2_,a3_,a4_,a5_,a6_,a7_)	do { log (m_, a1_, a2_, a3_, a4_, a5_, a6_, a7_); } while (0)

#else	/* !UHTTRACING */

#define	UHTTRACE0(m_)
#define	UHTTRACE1(m_,a1_)
#define	UHTTRACE2(m_,a1_,a2_)
#define	UHTTRACE3(m_,a1_,a2_,a3_)
#define	UHTTRACE4(m_,a1_,a2_,a3_,a4_)
#define	UHTTRACE5(m_,a1_,a2_,a3_,a4_,a5_)
#define	UHTTRACE6(m_,a1_,a2_,a3_,a4_,a5_,a6_)
#define	UHTTRACE7(m_,a1_,a2_,a3_,a4_,a5_,a6_,a7_)

#endif	/* !UHTTRACING */

#endif	/* LOG_H */
