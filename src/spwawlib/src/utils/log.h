/** \file
 * The SPWaW Library - utility code: logging.
 *
 * Copyright (C) 2007-2018 Erik Cumps <erik.cumps@gmail.com>
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

#define	IOLOG0(m_)		do { log (m_); } while (0)
#define	IOLOG1(m_,a1_)		do { log (m_, a1_); } while (0)
#define	IOLOG2(m_,a1_,a2_)	do { log (m_, a1_, a2_); } while (0)
#define	IOLOG3(m_,a1_,a2_,a3_)	do { log (m_, a1_, a2_, a3_); } while (0)

#else	/* !IOLOGGING */

#define	IOLOG0(m_)
#define	IOLOG1(m_,a1_)
#define	IOLOG2(m_,a1_,a2_)
#define	IOLOG3(m_,a1_,a2_,a3_)

#endif	/* !IOLOGGING */

#endif	/* LOG_H */
