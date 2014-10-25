#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dat.h"
#include "fns.h"

struct {
	char *ip;
	char *rip;	/* string(parsed(IP)) */
	int v;
} ips[] = {
	{ "127.0.0.1",       "127.0.0.1",       V4 },
	{ "255.255.255.255", "255.255.255.255", V4 },
	{ "255.",            "",                -1 },
	{ "0.0.0.0",         "0.0.0.0",         V4 },
	{ NULL,              NULL,              -1 },
};

/*
 * parseipv4, parseipv6, parseip,
 * ip4str,    ip6str,    ipstr
 */
int
testips(void)
{
	char  ip[LenStr];
	uchar pip[V6];
	int i, v;

	Z(ip); Z(pip);

	for (i = 0; ips[i].ip; i++) {
		v = parseip(ips[i].ip, pip);
		if (v != ips[i].v)
			fprintf(stderr, "testparse: %d != %d\n", ips[i].v, v);
		else if (v != -1)
		if (strcmp(ips[i].rip, ipstr(v, pip, ip)) != 0)
			fprintf(stderr, "testparse: %s != %s\n",
				ips[i].rip, ip);
	}

	return 0;
}

struct {
	char *cidr;
	char *ip;
	char *mask;
	int v;
} cidrs[] = {
	{ "127.0.0.1/1",    "127.0.0.1",   "128.0.0.0",       V4 },
	{ "10.0.0.0/6",     "10.0.0.0",    "252.0.0.0",       V4 },
	{ "100.64.0.0/10",  "100.64.0.0",  "255.192.0.0",     V4 },
	{ "172.16.0.0/12",  "172.16.0.0",  "255.240.0.0",     V4 },
	{ "198.18.0.0/15",  "198.18.0.0",  "255.254.0.0",     V4 },
	{ "192.88.99.0/24", "192.88.99.0", "255.255.255.0",   V4 },
	{ "10.0.0.0/29",    "10.0.0.0",    "255.255.255.248", V4 },
	{ "10.0.0.0/27",    "10.0.0.0",    "255.255.255.224", V4 },
	{ NULL,             NULL,          NULL,              -1 },
};

int
testcidrs(void)
{
	char ip[LenStr], mask[LenStr], cidr[2*LenStr];
	uchar pip[V6], pmask[V6];
	int i, v;

	for (i = 0; cidrs[i].cidr; i++) {
		Z(ip);  Z(mask), Z(cidr);
		Z(pip); Z(pmask);

		/* CIDR to IP mask */
		v = parsecidr(cidrs[i].cidr, pip, pmask);
		if (v != cidrs[i].v) {
			fprintf(stderr, "testcidrs: %d != %d\n", cidrs[i].v, v);
			continue;
		}
		if (v == -1)
			continue;
		if (strcmp(cidrs[i].mask, ipstr(v, pmask, mask)) != 0)
			fprintf(stderr, "testcidrs: %s != %s\n", cidrs[i].mask, mask);
		if (strcmp(cidrs[i].ip, ipstr(v, pip, ip)) != 0)
			fprintf(stderr, "testcidrs: %s != %s\n", cidrs[i].ip, ip);
		if (strcmp(cidrs[i].cidr, cidrstr(v, pip, pmask, cidr)) != 0)
			fprintf(stderr, "testcidrs: %s != %s\n", cidrs[i].cidr, cidr);
	}

	return 0;
}

int
main(void)
{
	testips();
	testcidrs();

	return 0;
}
