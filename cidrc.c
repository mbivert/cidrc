#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdint.h>

#include "dat.h"
#include "fns.h"

uint32_t
ipv4num(uchar *pip)
{
	return (pip[3]<<0) | (pip[2]<<8) | (pip[1]<<16) | (pip[0]<<24);
}

uchar *
numipv4(uint32_t ip, uchar *pip)
{
	pip[0] = (ip & 0xFF000000) >> 24;
	pip[1] = (ip & 0x00FF0000) >> 16;
	pip[2] = (ip & 0x0000FF00) >>  8;
	pip[3] = (ip & 0x000000FF) >>  0;

	return pip;
}

void
listips(int v, uchar *pip, uchar *pmask)
{
	uchar maxip[V6], tmp[V6];
	char ip[LenStr];
	uint32_t n;
	int i;

	for (i = 0; i < v; i++)
		maxip[i] = (pip[i] & pmask[i]) | ~pmask[i];

	if (v == V4) {
		for (n = ipv4num(pip); n <= ipv4num(maxip); n++)
			printf("%s\n", ipstr(v, numipv4(n, tmp), ip));
	}

/*
	printf("ip range: %s ", ipstr(v, pip, ip));
	printf("- %s\n", ipstr(v, nip, ip));
*/
}

int
list(int argc, char *argv[])
{
	uchar pip[V6], pmask[V6];
	int v;

	switch(argc) {
	case 1:
		fprintf(stderr, "error: expecting a network\n");
		return -1;
	case 2:
		v = parsecidr(argv[1], pip, pmask);
		break;
	default:
		v = parseipmask(argv[1], argv[2], pip, pmask);
		break;
	}

	if (v == -1)
		return -1;

	listips(v, pip, pmask);

	return 0;
}

int
fromcidr(char *cidr, char *ip, char *mask)
{
	uchar pip[V6] , pmask[V6];
	int v;

	Z(pip), Z(pmask);
	v = parsecidr(cidr, pip, pmask);
	if (v == -1)
		return -1;

	ipstr(v, pip, ip);
	ipstr(v, pmask, mask);

	return 0;
}

int
tocidr(char *ip, char *mask, char *cidr)
{
	uchar pip[V6] , pmask[V6];
	int v;

	Z(pip), Z(pmask);
	v = parseipmask(ip, mask, pip, pmask);
	if (v == -1)
		return -1;

	cidrstr(v, pip, pmask, cidr);

	return 0;
}

int
conv(int argc, char *argv[])
{
	char ip[LenStr], mask[LenStr];
	char cidr[2*LenStr];

	switch(argc) {
	case 1:
		fprintf(stderr, "error: expecting a network\n");
		return -1;
	case 2:
		Z(ip), Z(mask);
		if (fromcidr(argv[1], ip, mask) == -1)
			return -1;
		printf("%s %s\n", ip, mask);
		break;
	default:
		Z(cidr);
		if (tocidr(argv[1], argv[2], cidr) == -1)
			return -1;
		printf("%s\n", cidr);
		break;
	}

	return 0;
}

/*
 * List IPs address from specified network.
 * Convert cidr notation to ip netmask and forth
 * Simplify IPv6 notation.
 * IPv4 to IPv6 address and forth.
 */
struct {
	char *name;
	int (*fn)(int argc, char *argv[]);
	char *help;
} functions[] = {
	{ "list", list, "<network>" },
	{ "conv", conv, "<network>" },
	{ NULL,   NULL, NULL        },
};

int
lookupfn(char *name)
{
	int i;

	for (i = 0; functions[i].name; i++)
		if (strcmp(functions[i].name, name) == 0)
			return i;

	return -1;	
}

void
help(char *argv0)
{
	int i;

	for (i = 0; functions[i].name; i++)
		fprintf(stderr, "%s: %s %s\n", argv0,
			functions[i].name, functions[i].help);
}

int
main(int argc, char *argv[])
{
	int fn;

	if (argc < 2) {
		help(argv[0]);
		return 1;
	}

	fn = lookupfn(argv[1]);
	if (fn == -1) {
		fprintf(stderr, "%s: unknown function '%s'\n", argv[0], argv[1]);
		return -1;
	}

	return functions[fn].fn(argc-1, argv+1);
}
