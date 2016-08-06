#include "stdafx.h"
#include "utils/textconv.h"
#include "common/internal.h"

bool
TC_v2t (FILE *f, char *name, TC_DATA type, void *data, unsigned int len, int idx)
{
	char		buf[4096];
	char		*p;
	unsigned int	left;

	if (!f || !name ) return (false);
	if ((type != TC_DATA_MARKER) && !data) return (false);

	switch (type) {
		case TC_DATA_MARKER:
			fprintf (f, "%s\n", name);
			break;
		case TC_DATA_STRBUF:
			fprintf (f, "%s: <\n", name);
			p = (char *)data; left = len;
			while (left) {
				fprintf (f, "\t%s\n", p);
				left -= (strlen (p) + 1); p += (strlen (p) + 1);
			}
			fprintf (f, ">\n");
			break;
		case TC_DATA_BOOL:
			fprintf (f, "%s: %u\n", name, *((bool *)data));
			break;
		case TC_DATA_BYTE:
			fprintf (f, "%s: %d\n", name, *((BYTE *)data));
			break;
		case TC_DATA_USHORT:
			fprintf (f, "%s: %u\n", name, *((USHORT *)data));
			break;
		case TC_DATA_ULONG:
			fprintf (f, "%s: %u\n", name, *((ULONG *)data));
			break;
		case TC_DATA_DWORD:
			fprintf (f, "%s: %u\n", name, *((DWORD *)data));
			break;
		case TC_DATA_ASCIIZ:
			if (len >= sizeof (buf)) return (false);
			memcpy (buf, data, len); buf[len] = '\0';
			fprintf (f, "%s: (%d) <%s>\n", name, len, buf);
			break;
		case TC_DATA_TIMESTAMP:
			fprintf (f, "%s: 0x%16.16I64x\n", name, *((LONGLONG *)data));
			break;
		case TC_DATA_IDX_BOOL:
			fprintf (f, "%s: [%d] %u\n", name, idx, *((bool *)data));
			break;
		case TC_DATA_IDX_BYTE:
			fprintf (f, "%s: [%d] %d\n", name, idx, *((BYTE *)data));
			break;
		case TC_DATA_IDX_USHORT:
			fprintf (f, "%s: [%d] %u\n", name, idx, *((USHORT *)data));
			break;
		case TC_DATA_IDX_ULONG:
			fprintf (f, "%s: [%d] %u\n", name, idx, *((ULONG *)data));
			break;
		case TC_DATA_IDX_DWORD:
			fprintf (f, "%s: [%d] %u\n", name, idx, *((DWORD *)data));
			break;
		case TC_DATA_IDX_ASCIIZ:
			if (len >= sizeof (buf)) return (false);
			memcpy (buf, data, len); buf[len] = '\0';
			fprintf (f, "%s: [%d] (%d) <%s>\n", name, idx, len, buf);
			break;
		case TC_DATA_IDX_TIMESTAMP:
			fprintf (f, "%s: [%d] 0x%16.16I64x\n", name, idx, *((LONGLONG *)data));
			break;
		default:
			return (false);
			break;
	}
	return (true);
}

#define	PERR(m)		do { ERROR0 ("syntax error: " m "\n"); return (false); } while (0)
#define	PERR0(m)	do { ERROR0 ("syntax error: " m "\n"); ERROR1 ("%s\n", line); return (false); } while (0)
#define	PERR1(m,a0)	do { ERROR1 ("syntax error: " m "\n", a0); ERROR1 ("%s\n", line); return (false); } while (0)
#define	PERR2(m,a0,a1)	do { ERROR2 ("syntax error: " m "\n", a0, a1); ERROR1 ("%s\n", line); return (false); } while (0)

bool
TC_t2v (FILE *f, char *name, TC_DATA type, void *data, unsigned int len, int *idx)
{
	char		line[4096], buf[4096];
	unsigned int	llen;
	char		*p, *q;
	unsigned int	dlen;
	bool		stop;

	if (!f || !name ) return (false);
	if ((type != TC_DATA_MARKER) && !data) return (false);
	if (idx) *idx = 0;

	if (!fgets (line, sizeof (line) - 1, f)) PERR ("unexpected end of file!");
	p = strchr (line, '\n'); if (!p) PERR ("input line too long!");
	llen = p - line + 1;
	memcpy (buf, line, llen); buf[llen] = '\0';

	if (strncmp (buf, name, strlen (name)) != 0) {
		PERR1 ("invalid name, expected \"%s\"!", name);
	}
	p = buf + strlen (name);

	switch (type) {
		case TC_DATA_MARKER:
			if (*p != '\n') {
				PERR0 ("unexpected data after marker!");
			}
			break;
		case TC_DATA_STRBUF:
			if (strncmp (p, ": <\n", 4) != 0) {
				PERR0 ("invalid STRBUF definition!");
			}
			stop = false;
			while (!stop && ((p = fgets (line, sizeof (line) - 1, f)) != NULL)) {
				if (!p) PERR ("unexpected end of file!");
				p = strchr (line, '\n'); if (!p) PERR ("input line too long!");
				llen = p - line + 1;
				memcpy (buf, line, llen); buf[llen] = '\0';

				if (strncmp (buf, ">\n", 2) == 0) stop = true;
			}
			if (!stop) {
				PERR ("unexpected end of file parsing STRBUF!");
			}
			break;
		case TC_DATA_BOOL:
			if (strncmp (p, ": ", 2) != 0) {
				PERR0 ("invalid BOOL definition!");
			}
			p += 2;

			//fprintf (f, "%s: %u\n", name, *((bool *)data));
			break;
		case TC_DATA_BYTE:
			if (strncmp (p, ": ", 2) != 0) {
				PERR0 ("invalid BYTE definition!");
			}
			p += 2;

			//fprintf (f, "%s: %d\n", name, *((BYTE *)data));
			break;
		case TC_DATA_USHORT:
			if (strncmp (p, ": ", 2) != 0) {
				PERR0 ("invalid USHORT definition!");
			}
			p += 2;

			//fprintf (f, "%s: %u\n", name, *((USHORT *)data));
			break;
		case TC_DATA_ULONG:
			if (strncmp (p, ": ", 2) != 0) {
				PERR0 ("invalid ULONG definition!");
			}
			p += 2;

			//fprintf (f, "%s: %u\n", name, *((ULONG *)data));
			break;
		case TC_DATA_DWORD:
			if (strncmp (p, ": ", 2) != 0) {
				PERR0 ("invalid DWORD definition!");
			}
			p += 2;

			//fprintf (f, "%s: %u\n", name, *((DWORD *)data));
			break;
		case TC_DATA_ASCIIZ:
			if (strncmp (p, ": ", 2) != 0) {
				PERR0 ("invalid ASCIIZ definition!");
			}
			p += 2;

			if (*p != '(') PERR0 ("invalid ASCIIZ definition!");
			p++; q = strchr (p, ')'); if (!q) PERR0 ("invalid ASCIIZ definition!");
			*q = '\0';
			dlen = (int)strtol (p, NULL, 0);
			p = q + 2;
			if (dlen > len) PERR1 ("ASCIIZ data overflow: expected maximum %d chars!", len);

			if (*p != '<') PERR0 ("invalid ASCIIZ definition!");
			p++; q = strchr (p, '>'); if (!q) PERR0 ("invalid ASCIIZ definition!");
			if ((unsigned int)(q-p) > dlen) PERR2 ("invalid ASCIIZ definition: actual data length (%d) does not match indicated length (%d)!", (q-p), dlen);
			dlen = (q-p);
			memset (data, 0, len); memcpy (data, p, dlen);
			break;
		case TC_DATA_TIMESTAMP:
			if (strncmp (p, ": ", 2) != 0) {
				PERR0 ("invalid TIMESTAMP definition!");
			}
			p += 2;

			//fprintf (f, "%s: 0x%16.16I64x\n", name, *((LONGLONG *)data));
			break;
		case TC_DATA_IDX_BOOL:
			if (strncmp (p, ": ", 2) != 0) {
				PERR0 ("invalid IDX_BOOL definition!");
			}
			p += 2;

			if (*p != '[') PERR0 ("invalid IDX_BOOL definition!");
			p++; q = strchr (p, ']'); if (!q) PERR0 ("invalid IDX_BOOL definition!");
			*q = '\0';
			if (idx) {
				*idx = (int)strtol (p, NULL, 0);
			}
			p = q + 1;


			//fprintf (f, "%s: [%d] %u\n", name, idx, *((bool *)data));
			break;
		case TC_DATA_IDX_BYTE:
			if (strncmp (p, ": ", 2) != 0) {
				PERR0 ("invalid IDX_BYTE definition!");
			}
			p += 2;

			if (*p != '[') PERR0 ("invalid IDX_BYTE definition!");
			p++; q = strchr (p, ']'); if (!q) PERR0 ("invalid IDX_BYTE definition!");
			*q = '\0';
			if (idx) {
				*idx = (int)strtol (p, NULL, 0);
			}
			p = q + 1;


			//fprintf (f, "%s: [%d] %d\n", name, idx, *((BYTE *)data));
			break;
		case TC_DATA_IDX_USHORT:
			if (strncmp (p, ": ", 2) != 0) {
				PERR0 ("invalid IDX_USHORT definition!");
			}
			p += 2;

			if (*p != '[') PERR0 ("invalid IDX_USHORT definition!");
			p++; q = strchr (p, ']'); if (!q) PERR0 ("invalid IDX_USHORT definition!");
			*q = '\0';
			if (idx) {
				*idx = (int)strtol (p, NULL, 0);
			}
			p = q + 1;


			//fprintf (f, "%s: [%d] %u\n", name, idx, *((USHORT *)data));
			break;
		case TC_DATA_IDX_ULONG:
			if (strncmp (p, ": ", 2) != 0) {
				PERR0 ("invalid IDX_ULONG definition!");
			}
			p += 2;

			if (*p != '[') PERR0 ("invalid IDX_ULONG definition!");
			p++; q = strchr (p, ']'); if (!q) PERR0 ("invalid IDX_ULONG definition!");
			*q = '\0';
			if (idx) {
				*idx = (int)strtol (p, NULL, 0);
			}
			p = q + 1;


			//fprintf (f, "%s: [%d] %u\n", name, idx, *((ULONG *)data));
			break;
		case TC_DATA_IDX_DWORD:
			if (strncmp (p, ": ", 2) != 0) {
				PERR0 ("invalid IDX_DWORD definition!");
			}
			p += 2;

			if (*p != '[') PERR0 ("invalid IDX_DWORD definition!");
			p++; q = strchr (p, ']'); if (!q) PERR0 ("invalid IDX_DWORD definition!");
			*q = '\0';
			if (idx) {
				*idx = (int)strtol (p, NULL, 0);
			}
			p = q + 1;

			//fprintf (f, "%s: [%d] %u\n", name, idx, *((DWORD *)data));
			break;
		case TC_DATA_IDX_ASCIIZ:
			if (strncmp (p, ": ", 2) != 0) {
				PERR0 ("invalid IDX_ASCIIZ definition!");
			}
			p += 2;

			if (*p != '[') PERR0 ("invalid IDX_ASCIIZ definition!");
			p++; q = strchr (p, ']'); if (!q) PERR0 ("invalid IDX_ASCIIZ definition!");
			*q = '\0';
			if (idx) {
				*idx = (int)strtol (p, NULL, 0);
			}
			p = q + 1;

			//if (len >= sizeof (buf)) return (false);
			//memcpy (buf, data, len); buf[len] = '\0';
			//fprintf (f, "%s: [%d] (%d) <%s>\n", name, idx, len, buf);
			break;
		case TC_DATA_IDX_TIMESTAMP:
			if (strncmp (p, ": ", 2) != 0) {
				PERR0 ("invalid IDX_TIMESTAMP definition!");
			}
			p += 2;

			if (*p != '[') PERR0 ("invalid IDX_TIMESTAMP definition!");
			p++; q = strchr (p, ']'); if (!q) PERR0 ("invalid IDX_TIMESTAMP definition!");
			*q = '\0';
			if (idx) {
				*idx = (int)strtol (p, NULL, 0);
			}
			p = q + 1;

			//fprintf (f, "%s: [%d] 0x%16.16I64x\n", name, idx, *((LONGLONG *)data));
			break;
		default:
			return (false);
			break;
	}
	return (true);
}
