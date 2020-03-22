#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ncurses.h>
#include <locale.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define DEBUG_	0

/* Compile: gcc -W -Wall RcCarClientSocket.c -lncurses -o RcCarClientSocket */ 

#define PORT    4445

/* Prototypes */
void continuousSendCmd(int sockfd, struct sockaddr_in* servaddr);
void sendToAp(char charRead, int sockfd, struct sockaddr_in* servaddr);
char cmdToEnum(char charRead);

int main() 
{
  int sockfd;
  struct sockaddr_in servaddr;

  /* Creating socket file descriptor. Change SOCK_DGRAM to SOCK_STREAM for TCP socket */
  if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
    perror("Socket creation failed");
    exit(EXIT_FAILURE);
  }
  memset(&servaddr, 0, sizeof(servaddr));

  /* Filling server information */
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(PORT);
  servaddr.sin_addr.s_addr = inet_addr("192.168.4.1");

	/* Start sending commands if socket is set up successfully */
	continuousSendCmd(sockfd, &servaddr);

	/* Cleanup */
  close(sockfd);
  return 0;
}

/**
	* @brief: Loop for sending command to ESP3266
	* @param[in]: sockfd: file descriptor of socket
	* @param[in]: servaddr: socket to send command through
	* TODO: modify so that the console takes multiple input (such as 'w' + 'a' = 'q')
	*/
void continuousSendCmd(int sockfd, struct sockaddr_in* servaddr) 
{
  char currCharRead = 0;
  
  /* Window initialization */
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  
  printw("***************************************\n");
  printw("* RC Car Controller - Press Q to quit *\n");
  printw("***************************************\n");
  
  /* Begin loop */
  while ('Q' != (currCharRead = getch())) 
  {
  #ifndef DEBUG_
    printw("Key pressed: %c\n", currCharRead);
  #endif
   
    sendToAp(currCharRead, sockfd, servaddr);
    
    /* Wait 5 tenths of a second (0.5s) for the user to enter input */ 
    if (halfdelay(5) != ERR) 
    {
      while (getch() == currCharRead)
      {
      	if (halfdelay(1) == ERR) 
        {
        	break;
        }
      }
    }
    
  #ifndef DEBUG_
   	printw("Key released\n");
  #endif
  	/* If key is released, send a STOP command */
    sendToAp('9', sockfd, servaddr);
    cbreak();
  }
  
  endwin();
}

/**
	* @brief: Send a command via a socket
	* @param[in]: charRead - character read from user's input
	* @param[in]: sockfd: file descriptor of socket
	* @param[in]: servaddr: socket to send command through
	*/
void sendToAp(char charRead, int sockfd, struct sockaddr_in* servaddr)
{
	char cmdChar[2] = {0};
	
  *cmdChar = cmdToEnum(charRead);
  
  int err = sendto(sockfd, (const char *)cmdChar, strlen(cmdChar),
             MSG_CONFIRM, (const struct sockaddr *) servaddr,
             sizeof(struct sockaddr_in));
  if (err < 0)
  {
    printw("Failed sending cmd\n");
  }
}

/**
	* @brief: Convert character (command) read from user's input to an enum that
	*	can be executed by the GPIO controller's logic
	* @param[in]: charRead - character read from user's input
	* TODO: modify so that the console takes multiple input (such as 'w' + 'a' = 'q')
	*/
char cmdToEnum(char charRead)
{
	if(charRead == 'w')
	{
		return '1';
	}
	else if(charRead == 'e')
	{
		return '2';
	}
	else if(charRead == 'q')
	{
		return '3';
	}
	else if(charRead == 's')
	{
		return '4';
	}
	else if(charRead == 'c')
	{
		return '5';
	}
	else if(charRead == 'z')
	{
		return '6';
	}
	else if(charRead == 'a')
	{
		return '7';
	}
	else if(charRead == 'd')
	{
		return '8';
	}
	else 
	{
		return '9';
	}
	
	return '9';
}


