typedef unsigned char uchar;

enum {
	V4 = 4,	 /* IPv4 is 4*4 =32  bits=4  bytes */
	V6 = 16, /* IPv6 is 16*8=128 bits=16 bytes */

	/* could be smaller as we don't care about ::IPv4 address */
	LenStr = 46,  /* INET6_ADDRSTRLEN6+1 */
};
