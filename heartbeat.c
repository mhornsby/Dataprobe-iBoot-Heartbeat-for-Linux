/*
 
  heartbeat.c
  Program to generate heartbeat signal for Dataprobe (www.dataprobe.com )
  iBoot remote power boot over internet device.
 
  This program sends a datapacket to the iBoot at a defined interval. Should the
  iBoot stop recieving packets the iBoot can be configured via its web interface to reboot
  equipment or perform other power switching functions.
 
  Usage:
 
  heartbeat -h hostname [-p port] [-r secs] [-d]
 
  -h hostname. Host name or IP address of iBoot
  -p port. UDP port default port is 9100
  -r time in seconds to repeat messages in seconds. Default 10 seconds. 0 means run once only.
  -d enable diagnostic messages
  
  Author: Michael Hornsby michaelhmail-iboot@yahoo.com
  Version 1.1  17 Nov 2007
  
  This source code is released for free distribution under the terms of the GNU General Public License.
 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/file.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>

#define BUFLEN 20

int   iPort    = 9100;         // Port to send data to
int   iRepeat   = 10;          // Seconds between message repeats
char  *iAddress = NULL;        // iBoot's IP or hostname

void diep(char *s)
{
  perror(s);
  exit(1);
}

void usage()
{
    printf("usage: heatbeat -h hostname [-p port] [-r seconds] [-d]\n");
    printf("       -h Recipients IP address or hostname\n");
    printf("       -p Remote port. Default 9100\n");
    printf("       -r Repeat message time in seconds. Default 10, 0 = run once\n");
    printf("       -d Enable diagnostic messages\n");
    exit(1);
}


int main(int argc, char **argv)
{
  struct sockaddr_in recipient;
  int s, i, slen=sizeof(recipient);
  char buf[BUFLEN];
  int opterr = 0, c = 0;
  int dflag = 0;
  
  if(argc < 3) usage();

  while ((c = getopt(argc, argv, "dh:p:r:")) != -1 ) 
  	      switch(c)
  	      {
                case 'h': iAddress = optarg; break;
                case 'p': iPort = atol(optarg); break;
                case 'r': iRepeat = atol(optarg); break;
		case 'd': dflag = 1; break;
                default: usage();
            }
	    
  if (iAddress == NULL) usage();

  if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
    diep("socket");

  memset((char *) &recipient, 0, sizeof(recipient));
  recipient.sin_family = AF_INET;
  recipient.sin_port = htons(iPort);
  if ((recipient.sin_addr.s_addr = inet_addr(iAddress)) == INADDR_NONE)
    {
        struct hostent *host=NULL;

        host = gethostbyname(iAddress);
        if (host)
            bcopy( host->h_addr, &(recipient.sin_addr.s_addr), host->h_length);
        else
        {
            printf("%s: gethostbyname() failed: %s\n", argv[0], iAddress);
            return 1;
        }
     }
     
     do
     {
    if (dflag) printf("%s: Sending 'iBootHB' to: %s port: %d\n", argv[0], inet_ntoa(recipient.sin_addr),iPort);
    sprintf(buf, "iBootHB\n");
    if (sendto(s, buf, BUFLEN, 0, (struct sockaddr *) &recipient, slen)==-1) perror("sendto()");
      sleep(iRepeat);
    }	while(iRepeat > 0);
  close(s);
  return 0;
}
