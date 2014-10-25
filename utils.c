#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dat.h"
#include "fns.h"

#define min(a,b) ((a) < (b) ? (a) : (b))

int
parseipv4(char *from, uchar *to)
{
	char *p;
	int i;

	p = from;
	for (i = 0; i < V4 && *p; i++) {
		to[i] = strtoul(p, &p, 0);
		/* skip '.' */
		p++;
	}

	return i == V4 ? V4 : -1;
}

int
parseipv6(char *from, uchar *to)
{
	USED(from), USED(to);
	return V6;
}

/*
 * Parse IP address, return IP version
 */
int
parseip(char *from, uchar *to)
{
	/* '.' is v4 separator; ':' is v6 */
	return strchr(from, '.')
		? parseipv4(from, to)
		: parseipv6(from, to);
}

/*
 * Parse CIDR mask
 */
int
parsecmask(int v, char *from, uchar *to)
{
	uchar m;
	int i;

	errno = 0;
	m = strtoul(from, NULL, 0);
	if (errno != 0)
		return -1;

	for (i = 0; i < min(m/8, v); i++)
		to[i] = 0xFF;

	to[i] = 0xFF << (8-m%8);

	return 0;
}

/*
 * If we don't have explicit mask, guess it from parsed
 * IP address.
 */
void
maskfromip(int v, uchar *ip, uchar *mask)
{
	USED(v), USED(ip), USED(mask);
}

int
parsecidr(char *cidr, uchar *ip, uchar *mask)
{
	char *p;
	int v;

	p = strchr(cidr, '/');
//	*p++ = '\0';

	v = parseip(cidr, ip);
	if (v == -1) return -1;

	if (parsecmask(v, p+1, mask) == -1)
		return -1;

	return v;
}

int
parseipmask(char *ip, char *mask, uchar *pip, uchar *pmask)
{
	int v, v0;

	v  = parseip(ip, pip);
	v0 = parseip(mask, pmask);
	if (v == -1 || v0 == -1 || v != v0)
		return -1;

	return v;
}

char *
ipv4str(uchar *from, char *to)
{
	sprintf(to, "%d.%d.%d.%d", from[0], from[1],
		from[2], from[3]);
	return to;
}

char *
ipv6str(uchar *from, char *to)
{
	USED(from), USED(to);
	return to;
}

/*
 * Dump IP address as string
 */
char *
ipstr(int v, uchar *from, char *to)
{
	return v == V4 ? ipv4str(from, to) : ipv6str(from, to);
}

/*
 * Convert mask to (unsigned) integer
 */
uchar
maskcnum(uchar *from)
{
	uchar i, n;
	uchar x;	/* avoid changing from */

	for (n = i = 0; from[i] == 0xFF; i++)
		n+= 8;
	for (x = from[i]; (x & 0x80) == 0x80; x <<= 1)
		n++;

	return n;
}

char *
cidrstr(int v, uchar *ip, uchar *mask, char *to)
{
	int n;
	
	n = strlen(ipstr(v, ip, to));
	sprintf(to+n, "/%d", maskcnum(mask));

	return to;
}