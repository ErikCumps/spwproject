/** \file
 * The Steel Panthers World at War report tool.
 *
 * Copyright (C) 2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL V2
 */

#ifndef	INTERNAL_XML_UTILS_H
#define	INTERNAL_XML_UTILS_H	1

#include "stdafx.h"

static const char *indentation = "    ";

class Xml
{
public:
	Xml (FILE *rf) : rf(rf)	{
		fprintf (rf, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
		level = 0;
		memset (scope, 0, sizeof (scope));
		memset (indent, 0, sizeof (indent));
	};

	~Xml (void) {
		rf = NULL;
	};

	void scope_push (char *label) {
		fprintf (rf, "%s<%s>\n", indent, label);
		push (label);
	};

	void scope_push (char *label, char *al, int av) {
		fprintf (rf, "%s<%s %s=\"%d\">\n", indent, label, al, av);
		push (label);
	};

	void scope_push (char *label, char *al, char *av) {
		fprintf (rf, "%s<%s %s=\"%s\">\n", indent, label, al, av);
		push (label);
	};

	void scope_pop (void) {
		char *label = pop();

		fprintf (rf, "%s</%s>\n", indent, label);
	};

	void item (char *label, char *value) {
		fprintf (rf, "%s<%s>%s</%s>\n", indent, label, value, label);
	};

	void item (char *label, const char *value) {
		fprintf (rf, "%s<%s>%s</%s>\n", indent, label, value, label);
	};

	void item (char *label, int value) {
		fprintf (rf, "%s<%s>%d</%s>\n", indent, label, value, label);
	};

	void item (char *label, unsigned int value) {
		fprintf (rf, "%s<%s>%u</%s>\n", indent, label, value, label);
	};

	void item (char *label, long value) {
		fprintf (rf, "%s<%s>%d</%s>\n", indent, label, value, label);
	};

	void item (char *label, unsigned long value) {
		fprintf (rf, "%s<%s>%u</%s>\n", indent, label, value, label);
	};

	void item (char *label, double value) {
		fprintf (rf, "%s<%s>%.3f</%s>\n", indent, label, value, label);
	};

	void comment (char *text)
	{
		fprintf (rf, "%s, <!-- %s -->\n", indent, text);
	}

private:
	FILE	*rf;
	int	level;
	char	*scope[256];
	char	indent[256*4];

	void	push (char *label)
	{
		scope[level] = label;
		level++;
		strcat (indent, indentation);
	}

	char	*pop (void)
	{
		if (level == 0) return (NULL);

		level--;
		indent[strlen(indent)-strlen(indentation)] = '\0';
		return (scope[level]);
	}
};

class XmlScope
{
public:
	XmlScope (Xml &xml, char *label) : xml(xml) {
		xml.scope_push (label);
	};

	XmlScope (Xml &xml, char *label, char *al, int av) : xml(xml) {
		xml.scope_push (label, al, av);
	};

	XmlScope (Xml &xml, char *label, char *al, char *av) : xml(xml) {
		xml.scope_push (label, al, av);
	};

	~XmlScope (void) {
		xml.scope_pop();
	};

private:
	Xml	&xml;

private:
	XmlScope(const XmlScope&);
	XmlScope& operator=(const XmlScope&);
};

#endif	/* INTERNAL_XML_UTILS_H */
