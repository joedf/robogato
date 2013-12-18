#include "main.h"
#include "utils.h"
#include "irc.h"
#include "benchmark.h"

#define BOT_VERSION "1.0.0"
#define SERVERLOG_CLEAN 1
#define SAVELOG 0

#ifndef PASSWORD
	#define PASSWORD "***********"
#endif

int e;
int z;
int connected = 0;
int identified = 0;
int joined = 0;

int main(int argc, char *argv[]) {
	
	const char *server = "irc.freenode.org";
	const char *port   = DEFAULT_PORT;
		  char *chan   = "#joedf"; //"#ahkscript"; //"#Sjc_Bot"; //
		  char *nick   = "robogato";
	const char *name   = "El robot Gato de la muerte!";
	const char *master = "joedf";
	const char *pass   = PASSWORD;
	const char *email  = "ahkscript@live.ca";
	const  int timeout = 10;
	
	const char *help   = "Current Commands : slap, eat, nom, ahk, g, dd, uptime, ver[sion], say, raw, dance, party, quit, restart, nick, chan, help";
	char g__style[] = { 0xE3, 0x83, 0xBE, 0x28, 0xE2, 0x8C, 0x90, 0xE2, 0x96, 0xA0, 0x5F, 0xE2, 0x96, 0xA0, 0x29, 0xE3, 0x83, 0x8E, 0xE2, 0x99, 0xAA, 0x20, 0xE3, 0x83, 0xBE, 0x28, 0xE2, 0x8C, 0x90, 0xE2, 0x96, 0xA0, 0x5F, 0xE2, 0x96, 0xA0, 0x29, 0xE3, 0x83, 0x8E, 0xE2, 0x99, 0xAA, 0x20, 0xE3, 0x83, 0xBE, 0x28, 0xE2, 0x8C, 0x90, 0xE2, 0x96, 0xA0, 0x5F, 0xE2, 0x96, 0xA0, 0x29, 0xE3, 0x83, 0x8E, 0xE2, 0x99, 0xAA, 0x20, 0xE3, 0x83, 0xBE, 0x28, 0xE2, 0x8C, 0x90, 0xE2, 0x96, 0xA0, 0x5F, 0xE2, 0x96, 0xA0, 0x29, 0xE3, 0x83, 0x8E, 0xE2, 0x99, 0xAA, 0x20, 0x45, 0x56, 0x45, 0x52, 0x59, 0x4F, 0x4E, 0x45, 0x20, 0x44, 0x41, 0x4E, 0x43, 0x45, 0x20, 0x54, 0x4F, 0x20, 0x47, 0x41, 0x4E, 0x47, 0x4E, 0x41, 0x4D, 0x20, 0x53, 0x54, 0x59, 0x4C, 0x45, 0x0D };
	char *gstyle = malloc(123);
	for(z=0;z<120;z++)
		gstyle[z]=g__style[z];
	gstyle[119]=0;
	const char *gstyle_url = "http://youtu.be/9bZkp7q19f0";
	
	int serverlog = 0;
	int restart = 0;
	int chat_start = 0;
	double uptimeStart;
	
	#if (SAVELOG)
		FILE *pLogFile = fopen("log.txt","ab+");
		fputs("################# [Log Start] #################",pLogFile)
	#endif
	
	int recvbuflen = DEFAULT_BUFLEN;
	char recvbuf[DEFAULT_BUFLEN];
	char l_buffer[DEFAULT_BUFLEN*4];
	char buffer[DEFAULT_BUFLEN];
	char suffix[DEFAULT_BUFLEN];
		arr_zero(recvbuf,DEFAULT_BUFLEN);
		arr_zero(l_buffer,DEFAULT_BUFLEN);
		arr_zero(buffer,DEFAULT_BUFLEN);
		arr_zero(suffix,DEFAULT_BUFLEN);
	WSADATA wsaData;
	SOCKET sIRC;
	struct addrinfo *pAddr;
	
	printf("\nserver: %s\n",server);
	printf("  port: %s\n",port);
	printf("  chan: %s\n",chan);
	printf("  nick: %s\n",nick);
	puts(SEPERATOR);
	
	//////////////////////Initialize////////////////////
		puts("initializing winsock...");
		if ((e = WSAStartup(WINSOCK_REQUEST_VERSION, &wsaData)) != 0) {
			printf("WSAStartup failed: %d\n", e);
			return 1;
		}
	
	//////////////////////Resolving/////////////////////
		puts("resolving...");
		// Resolve the server address and port
		pAddr = sck_resolve(server,port);
	
	//////////////////////Socketing////////////////////
		puts("Socketing...");
		sIRC = get_socket(pAddr);
	
	//////////////////////Connecting////////////////////
		puts("Connecting...");
		sck_connect(&sIRC,pAddr);
	
	//////////////////////Sending Data//////////////////
		puts("sending data...");
		sck_sendf(&sIRC,"PASS %s\r\n",pass);
		sck_sendf(&sIRC,"USER %s 0 * :%s\r\n",nick,name);
		sck_sendf(&sIRC,"NICK %s\r\n",nick);
		sck_sendf(&sIRC,"JOIN %s\r\n",chan);
		sck_sendf(&sIRC,"PRIVMSG NickServ :IDENTIFY %s %s\r\n",nick,pass);
		
	//////////////////////Receiving Data//////////////////
		puts("Receiving data...");
		puts(SEPERATOR);
		
		// Receive data until the server closes the connection
		do {
			recvbuf[0] = 0;
			buffer[0] = 0;
			suffix[0] = 0;
			e = recv(sIRC, recvbuf, recvbuflen, 0);
			if (e > 0)
			{
				if (serverlog)
					puts(recvbuf);
				else
				{
					if ((instrf(recvbuf," PRIVMSG %s",chan)) || (instrf(recvbuf," PRIVMSG %s",nick)))
					{
						puts(strtok(recvbuf,"\r\n"));
					}
				}
				if (!connected) {
					if (instrf(recvbuf,"%d",RplMotdEnd))
						puts("[FREENODE-MOTD]-> Done");
					if(connected=irc_connected(recvbuf))
					{
						puts(SEPERATOR);
						puts("[SUCCESS]-> Connected");
					}
				}
				else
				{
					if (!identified)
					{
						if (identified=irc_identified(recvbuf))
							puts("[SUCCESS]-> Indentification");
					}
					if (!joined)
					{
						if (joined=irc_joined(recvbuf)) {
							puts("[SUCCESS]-> Join Channel");
							if (SERVERLOG_CLEAN)
								serverlog=0;
							uptimeStart=get_time();
						}
					}
					if (identified && joined && !chat_start)
					{
						chat_start=1;
						printf("%s[Chat log]%s\n",SEPERATOR,SEPERATOR);
					}
				}
				if (sscanf(recvbuf,"PING :%s\r\n",suffix) > 0) {
					printf("[REQUEST]-> %s\n",strtok(recvbuf,"\r\n"));
					sck_sendf(&sIRC,"PONG :%s\r\n",suffix);
				}
				else if (instr(recvbuf,"!slap")) { //instrf(recvbuf,"%s :!slap",chan)
					irc_getuser(recvbuf,buffer);
					printf("[REQUEST]-> <%s> !slap\n",buffer);
					z=strpos(":!slap ",recvbuf);
					if (z>0)
					{
						strtrim_l(strtok(recvbuf,"\r\n"),z+6,suffix);
						if (str_isspace(suffix))
							strcpy(suffix,buffer); //irc_getuser(recvbuf,suffix);
					}
					else
						strcpy(suffix,buffer); //irc_getuser(recvbuf,suffix);
					sck_sendf(&sIRC,"PRIVMSG %s :%cACTION slaps %s with a dictionary%c\r\n",chan,1,suffix,1);
				}
				else if (instrf(recvbuf,"%s :!eat",chan) || instrf(recvbuf,"%s :!nom",chan)) {
					sck_sendf(&sIRC,"PRIVMSG %s :%cACTION nom nom! Im hungry! :D%c\r\n",chan,1,1);
				}
				else if (instrf(recvbuf,"%s :!uptime",chan)) {
					sck_sendf(&sIRC,"PRIVMSG %s :%c%d %g seconds\r\n",chan,Color,LightGreen,get_time()-uptimeStart);
				}
				else if (instrf(recvbuf,"%s :!ver",chan)) {
					sck_sendf(&sIRC,"PRIVMSG %s :%c%d[%s] (Bot) - v%s (Written in C with WinSock)\r\n",chan,Color,LightGreen,nick,BOT_VERSION);
				}
				else if (instrf(recvbuf,"%s :!alarm",chan)) {
					
					
					
					// still working on !alarm, too lazy right now :P
					
					
					
					sck_sendf(&sIRC,"NAMES %s\r\n",chan);
					puts("______________________LIST NAMES___________________");
					do {
						e=recv(sIRC, recvbuf, recvbuflen, 0);
						if (!instrf(recvbuf," %d ",RplEndOfNames))
							strcat(l_buffer,strtok(recvbuf,"\r\n"));
						else
							break;
					} while((e>0) && (!instrf(recvbuf," %d ",RplEndOfNames)));
					puts("______________________LIST NAMES END_______________");
					sck_sendf(&sIRC,"PRIVMSG %s :Highlighting all: %s\r\n",chan,l_buffer);
				}
				else if (instrf(recvbuf,"%s :!say",chan) || instrf(recvbuf,"%s PRIVMSG %s :!say",master,nick)) {
					irc_getuser(recvbuf,suffix);
					irc_getmsg(recvbuf,buffer);
					strcpy(buffer,strtok(buffer,"\r\n"));
					printf("[REQUEST]-> <%s> %s\n",suffix,buffer);
					strtrim_l(buffer,5,buffer);
					sck_sendf(&sIRC,"PRIVMSG %s :%s\r\n",chan,buffer);
				}
				else if (instrf(recvbuf,"%s :!ahk",chan)) {
					irc_getuser(recvbuf,suffix);
					irc_getmsg(recvbuf,buffer);
					strcpy(buffer,strtok(buffer,"\r\n"));
					printf("[REQUEST]-> <%s> %s\n",suffix,buffer);
					strtrim_l(buffer,5,buffer);
					if (str_isspace(buffer))
						sck_sendf(&sIRC,"PRIVMSG %s :%c%d[AHK Search] -> http://ahkscript.co.nr\r\n",chan,Color,Orange);
					else
						sck_sendf(&sIRC,"PRIVMSG %s :%c%d[AHK Search] %c%d%s -> http://ahkscript.co.nr/?s=a&q=%s\r\n",
											chan,Color,Orange,Color,LightCyan,buffer,url_encode(buffer));
				}
				else if (instrf(recvbuf,"%s :!g",chan)) {
					irc_getuser(recvbuf,suffix);
					irc_getmsg(recvbuf,buffer);
					strcpy(buffer,strtok(buffer,"\r\n"));
					printf("[REQUEST]-> <%s> %s\n",suffix,buffer);
					strtrim_l(buffer,3,buffer);
					if (str_isspace(buffer))
						sck_sendf(&sIRC,"PRIVMSG %s :%c%d[Google Search] -> http://google.com/\r\n",chan,Color,Orange);
					else
						sck_sendf(&sIRC,"PRIVMSG %s :%c%d[Google Search] %c%d%s -> http://google.com/#q=%s\r\n",
											chan,Color,Orange,Color,LightCyan,buffer,url_encode(buffer));
				}
				else if (instrf(recvbuf,"%s :!dd",chan)) {
					irc_getuser(recvbuf,suffix);
					irc_getmsg(recvbuf,buffer);
					strcpy(buffer,strtok(buffer,"\r\n"));
					printf("[REQUEST]-> <%s> %s\n",suffix,buffer);
					strtrim_l(buffer,4,buffer);
					if (str_isspace(buffer))
						sck_sendf(&sIRC,"PRIVMSG %s :%c%d[DuckDuckGo Search] -> https://duckduckgo.com/\r\n",chan,Color,Orange);
					else
						sck_sendf(&sIRC,"PRIVMSG %s :%c%d[DuckDuckGo Search] %c%d%s -> https://duckduckgo.com/?q=%s\r\n",
											chan,Color,Orange,Color,LightCyan,buffer,url_encode(buffer));
				}
				else if (instrf(recvbuf,"%s :!raw",chan) || instrf(recvbuf,"%s PRIVMSG %s :!raw",master,nick)) {
					irc_getuser(recvbuf,suffix);
					strcpy(buffer,strtok(recvbuf,"\r\n"));
					if ((z=strpos(":!raw",buffer))>0)
					{
						if ((5+z)>=strlen(buffer))
							continue;
						strtrim_l(buffer,5+z,buffer);
						printf("[REQUEST-RAW]-> <%s> %s\n",suffix,buffer);
						if ((!strcmp(suffix,master) || instrf(recvbuf,"%s PRIVMSG %s :!raw",master,nick)) && (irc_sender(recvbuf,master)))
						{
							sck_sendf(&sIRC,"%s\r\n",buffer);
							#if (SAVELOG)
								fputs(buffer,pLogFile);
							#endif
						}
						else
						{
							sck_sendf(&sIRC,"PRIVMSG %s :%c%dYou are not authorized.\r\n",chan,Color,Red);
						}
					}
				}
				else if (instr(recvbuf,"!dance")) { //instrf(recvbuf,"%s :!dance",chan)
					sck_sendf(&sIRC,"PRIVMSG %s :%c%dDanceroo! :D %c%dyay %c%dyay %c%dyay! %c%d\\(^_^)/ %c%d:D\r\n",
							chan,Color,Orange,Color,Violet,Color,Red,Color,LightGreen,Color,LightCyan,Color,Blue);
				}
				else if (instrf(recvbuf,"%s :!quit",chan) || instrf(recvbuf,"%s PRIVMSG %s :!quit",master,nick)) {
					irc_getuser(strtok(recvbuf,"\r\n"),suffix);
					printf("[REQUEST]-> <%s> !quit\n",suffix);
					if ((!strcmp(suffix,master) || instrf(recvbuf,"%s PRIVMSG %s :!quit",master,nick)) && (irc_sender(recvbuf,master)))
					{
						sck_send(&sIRC,"QUIT :Exiting\r\n");
						//break;
					}
					else
						sck_sendf(&sIRC,"PRIVMSG %s :%c%dYou are not authorized.\r\n",
												chan,Color,Red);
				}
				else if (instrf(recvbuf,"%s :!restart",chan) || instrf(recvbuf,"%s PRIVMSG %s :!restart",master,nick)) {
					irc_getuser(strtok(recvbuf,"\r\n"),suffix);
					printf("[REQUEST]-> <%s> !restart\n",suffix);
					if ((!strcmp(suffix,master) || instrf(recvbuf,"%s PRIVMSG %s :!restart",master,nick)) && (irc_sender(recvbuf,master)))
					{
						sck_send(&sIRC,"QUIT :Restarting\r\n");
						restart=1;
						//break;
					}
					else
						sck_sendf(&sIRC,"PRIVMSG %s :%c%dYou are not authorized.\r\n",
												chan,Color,Red);
				}
				else if (instrf(recvbuf,"%s :!nick",chan) || instrf(recvbuf,"%s PRIVMSG %s :!nick",master,nick)) {
					irc_getuser(recvbuf,suffix);
					printf("[REQUEST]-> <%s> !nick\n",suffix);
					if ((!strcmp(suffix,master) || instrf(recvbuf,"%s PRIVMSG %s :!nick",master,nick)) && (irc_sender(recvbuf,master)))
					{
						z=strpos(":!nick ",recvbuf);
						if (z>0)
						{
							strtrim_l(recvbuf,z+6,suffix);
							if (str_isspace(suffix))
								continue;
							sck_sendf(&sIRC,"NICK %s\r\n",suffix);
							nick = malloc(DEFAULT_BUFLEN);
							sprintf(nick,"%s",suffix);
							nick[strlen(suffix)]=0;
						}
					}
					else
						sck_sendf(&sIRC,"PRIVMSG %s :%c%dYou are not authorized.\r\n",
												chan,Color,Red);
				}
				else if (instrf(recvbuf,"%s :!chan",chan) || instrf(recvbuf,"%s PRIVMSG %s :!chan",master,nick)) {
					irc_getuser(recvbuf,suffix);
					printf("[REQUEST]-> <%s> !chan\n",suffix);
					if ((!strcmp(suffix,master) || instrf(recvbuf,"%s PRIVMSG %s :!chan",master,nick)) && (irc_sender(recvbuf,master)))
					{
						z=strpos(":!chan ",recvbuf);
						if (z>0)
						{
							strtrim_l(recvbuf,z+6,suffix);
							if (str_isspace(suffix))
								continue;
							sck_sendf(&sIRC,"JOIN %s\r\n",suffix);
							sck_sendf(&sIRC,"PART %s\r\n",chan);
							chan = malloc(DEFAULT_BUFLEN);
							sprintf(chan,"%s",suffix);
							chan[strlen(suffix)]=0;
						}
					}
					else
						sck_sendf(&sIRC,"PRIVMSG %s :%c%dYou are not authorized.\r\n",chan,Color,Red);
				}
				else if (instrf(recvbuf,"%s :!help",chan) || instrf(recvbuf,"%s PRIVMSG %s :!help",master,nick)) {
					sck_sendf(&sIRC,"PRIVMSG %s :%c%d[%s] (Bot) - %s\r\n",chan,Color,LightGreen,nick,help);
				}
				else if (instr(recvbuf,"!party")) {
					sck_sendf(&sIRC,"PRIVMSG %s :%c%d%s >>> [ %s ]\r\n",chan,Color,Blue,gstyle,gstyle_url);
				}
			}
			else if (e == 0)
			{
				puts(SEPERATOR);
				printf("Connection closed\n");
			}
			else
			{
				printf("recv failed: %d\n", WSAGetLastError());
			}
		} while (e > 0);
	
	#if (SAVELOG)
		fputs("################# [ Log End ] #################",pLogFile)
		fclose(pLogFile);
	#endif
	
	//////////////////////Disconnecting//////////////////
		sck_disconnect(&sIRC);
		
	//////////////////////clean up//////////////////////
		closesocket(sIRC);
		WSACleanup();
	
	puts(SEPERATOR);
	
	if (restart)
	{
		puts("Starting an other instance & quitting...");
		sprintf(buffer,"start %s",argv[0]);
		buffer[6+strlen(argv[0])]=0;
		return system(buffer);
	}
	else
		puts("done.");
	
	return 0;
}
