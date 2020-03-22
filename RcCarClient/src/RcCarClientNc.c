#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include <locale.h>
#include <ctype.h>
#include <string.h> 

#define DEBUG_ 0

/* Compile: gcc -W -Wall RcCarClientNc.c -lncurses -o RcCarClientNc */ 

/* Prototypes */
void sendToAp(char cmdChar);
char cmdToEnum(char charRead);

int main() {
  char currCharRead = 0;
  
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  
  printw("***************************************\n");
  printw("* RC Car Controller - Press Q to quit *\n");
  printw("***************************************\n");
  
  /* TODO: modify so that the console takes multiple input (such as 'w' + 'a' = 'q') */
  while ('Q' != (currCharRead = getch())) 
  {
  #ifndef DEBUG_
    printw("Key pressed: %c\n", currCharRead);
  #endif
   
    sendToAp(currCharRead);
    
    /* Wait 1/5th of a second for the user to enter input */ 
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
    sendToAp('9');
    cbreak();
  }
  
  endwin();
  return 0;
}

/**
	* @brief: Echo a message to AP using netcat
	* @param[in]: charRead - character read from user's input
	*/
void sendToAp(char charRead)
{
	char cmdToGo[100] = {0};
	char cmdChar[2] = {0};
	
	strcpy(cmdToGo, "echo \"");
  *cmdChar = cmdToEnum(charRead);
  strcat(cmdToGo, cmdChar);
  strcat(cmdToGo, "\" | nc -4u -w0 192.168.4.1 4445");
#ifndef DEBUG_
  printw("Send to AP: %s\n", cmdToGo);
#endif
  system(cmdToGo);
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

