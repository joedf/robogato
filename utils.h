#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

int strpos(char *needle, char *haystack);
int instr(const char *h, const char *n);
int instrf(const char *h, const char *n, ...);
int str_isspace(char *str);
char *strtrim_l(const char *string, int trim, char *dest);
char *StrReplaceAll(char const * const original, char const * const pattern, char const * const replacement);
void arr_zero(char a[], int len);
void irc_getuser(char *buf, char u[]);
void irc_getmsg(char *buf, char m[]);
int irc_connected(char *recvbuf);
int irc_identified(char *recvbuf);
int irc_joined(char *recvbuf);
int irc_sender(char *r, const char *u);
