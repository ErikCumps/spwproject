#ifndef	LOG_H
#define	LOG_H	1

extern void	log_init	(char *log, bool append=false);
extern void	log		(char *fmt, ...);
extern void	log_nots	(char *fmt, ...);
extern void	log_shutdown	(void);

#endif	/* LOG_H */
