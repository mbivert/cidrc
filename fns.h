/* utils.c */
int parseipv4(char *from, uchar *to);
int parseipv6(char *from, uchar *to);
int parseip(char *from, uchar *to);
int parsecmask(int v, char *from, uchar *to);
void maskfromip(int v, uchar *ip, uchar *mask);
int parsecidr(char *cidr, uchar *ip, uchar *mask);
int parseipmask(char *ip, char *mask, uchar *pip, uchar *pmask);
char *ipv4str(uchar *from, char *to);
char *ipv6str(uchar *from, char *to);
char *ipstr(int v, uchar *from, char *to);
uchar maskcnum(uchar *from);
char *cidrstr(int v, uchar *ip, uchar *mask, char *to);

#define USED(x) ((x) = (x))
#define    Z(x) (memset((x), '\0', sizeof(x)))
