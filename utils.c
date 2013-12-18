#include "main.h"
#include "utils.h"
#include "irc.h"

int strpos(char *needle, char *haystack) {
	char *dest = strstr(haystack, needle);
	return (dest - haystack) + 1;
}

int instr(const char *h, const char *n) {
	return (strstr(h,n)!=NULL);
}

int instrf(const char *h, const char *n, ...) {
	va_list args;
	va_start(args, n);
	char s[DEFAULT_BUFLEN];
	vsprintf(s,n,args);
	va_end(args);
	return instr(h,s);
}

int str_isspace(char *str) {
	int i;
	for(i=0;i<strlen(str);i++){
		if (!isspace(str[i]))
			return 0;
	}
	return 1;
}

char *strtrim_l(const char *string, int trim, char *dest) {
	int len=strlen(string);
	strncpy(dest,string+trim,len);
	dest[len]='\0';
	return dest;
}

//Function adatpted from: http://stackoverflow.com/a/780024/883015
char *StrReplaceAll(char const * const original, char const * const pattern, char const * const replacement) {
	
	size_t const replen = strlen(replacement);
	size_t const patlen = strlen(pattern);
	size_t const orilen = strlen(original);

	size_t patcnt = 0;
	const char * oriptr;
	const char * patloc;

	// find how many times the pattern occurs in the original string
	for (oriptr = original; patloc = strstr(oriptr, pattern); oriptr = patloc + patlen)
	{
		patcnt++;
	}

	{
		// allocate memory for the new string
		size_t const retlen = orilen + patcnt * (replen - patlen);
		char * const returned = (char *) malloc( sizeof(char) * (retlen + 1) );

		if (returned != NULL)
		{
			// copy the original string, 
			// replacing all the instances of the pattern
			char * retptr = returned;
			for (oriptr = original; patloc = strstr(oriptr, pattern); oriptr = patloc + patlen)
			{
				size_t const skplen = patloc - oriptr;
				// copy the section until the occurence of the pattern
				strncpy(retptr, oriptr, skplen);
				retptr += skplen;
				// copy the replacement 
				strncpy(retptr, replacement, replen);
				retptr += replen;
			}
			// copy the rest of the string.
			strcpy(retptr, oriptr);
		}
			return returned;
	}
}

void arr_zero(char a[], int len) {
	int i;
	for(i=0;i<len;i++)
		a[i]=0;
}

void half_arr_zero(char a[], int len) {
	int i;
	for(i=0;i<(len/2);i++)
		a[i+1]=0;
}

char *url_encode(char *str) {
	return StrReplaceAll(StrReplaceAll(str," ","%20"),"&","%26");
}

void irc_getuser(char *buf, char u[]) {
	sscanf(buf,":%s!",u);
	int n=strpos("!",u);
	if (n>0)
		u[n-1]=0;
}

void irc_getmsg(char *buf, char m[]) {
	char *p;
	char *cbuf = strtok(buf,"\r\n");
	if ((p=strpbrk(cbuf+1,":"))!=NULL)
	{
		int k = (p-cbuf+1);
		int q = strlen(cbuf)-k;
		strncpy(m,cbuf+k,q);
		m[q]=0;
	}
	else
		m[0]=0;
}

int irc_connected(char *recvbuf) {
	if (instrf(recvbuf," %d ",RplMotdEnd) || instrf(recvbuf," %d ",ErrNoMotd))
		return 1;
	else
		return 0;
}

int irc_identified(char *recvbuf) {
	if (instr(recvbuf,"You are now identified"))
		return 1;
	if (instrf(recvbuf," %d ",ErrUserOnChannel))
		return -1;
	if (instrf(recvbuf," %d ",ErrNickNameInUse))
		return -1;
	if (instr(recvbuf,"Invalid password"))
		return -1;
	if (instr(recvbuf,"Nick/channel is temporarily unavailable"))
		return -1;
	return 0;
}

int irc_joined(char *recvbuf) {
	if (instr(recvbuf,":End of /NAMES list"))
		return 1;
	return 0;
}

int irc_sender(char *r, const char *u) {
	int i;
	int ul = strlen(u);
	int rl = strlen(r);
	if ((rl > ul) && (r[0]==':') && (r[ul+1]=='!')) {
		for(i=1;i<ul+1;i++) {
			//printf("\n%c =vs= %c\n",r[i],u[i-1]);
			if (r[i]!=u[i-1])
				return 0;
		}
		return 1;
	}
	return 0;
}

