/*
 * Author: Michelle Li          
 * Student Number: 51551794  
 * Lab Section: L1O     
 * Date:             11/24/2018 5:25:13 PM
 *           
 * Purpose: Displays one of two scrolling messages on the LED
 *
 */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <DAQlib.h>
#include <Windows.h>

#define ON 1
#define OFF 0

#define TRUE 1
#define FALSE 0

#define NUM_DISPLAYS 8

#define DIRECTION_SWITCH 0
#define SPEED_SWITCH 1

#define ONE_SEC 1000

void scrollMessage(void);
void writeFood(int, int);
void writeSleep(int, int);
void writeMessage(int message, int wordLen, int i);

int main(void)
{
	int setupnum;

	printf("Enter 0 for hardware and 4 for simulator: ");
	scanf("%d", &setupnum);

	if (setupDAQ(setupnum) == TRUE) {
		scrollMessage();
	}

	else {
		printf("ERROR: cannot initialize");
	}

	system("PAUSE");
	return 0;
}

/*
   Purpose: Prompts user to enter a message number to display
			increases or decreases the position of the letters 
			every 0.5 or 1 second depending on whether or not
			the SPEED or DIRECTION switch is turned on.
   Return - void
*/
void scrollMessage(void) {
	int message;

	//prompts user to choose a message number to display
	printf("\nMessage 1: Food \nMessage 2: SLEEP ");
	printf("\nSelect a message number (1 or 2) to display: ");
	scanf("%d", &message);
	
	int wordLen;

	if (message == 1) {
		wordLen = 4; //number of letters in "Food"
	}
	else if (message == 2) {
		wordLen = 5; //number of letters in "SLEEP"
	}

	while (continueSuperLoop() == TRUE) {
		int i;
		
		/*When the message finishes scrolling through one side of the display,
		start it again on the other side of the display*/
		if (digitalRead(DIRECTION_SWITCH) == ON) {
			i = NUM_DISPLAYS + wordLen-1;
		}
		else {
			i = 0;
		}
		while(i < NUM_DISPLAYS + wordLen && i>=0) {
			//if the user quits, terminate the program
			if (continueSuperLoop() == FALSE)
				break;
			
			writeMessage(message, wordLen, i);

			//makes the message scroll every 0.5 seconds if speed switch is on 
			//and every 1 second if speed switch is off
			if (digitalRead(SPEED_SWITCH) == ON) {
				Sleep(0.5*ONE_SEC);
			}
			else {
				Sleep(ONE_SEC);
			}

			//makes the message scroll right if direction switch is on and left if it is off
			if (digitalRead(DIRECTION_SWITCH) == ON) {
				i--;
			}
			else {
				i++;
			}
		}
		
	}
}

/* Purpose: Determines the position that each letter of the given 
			word will be displayed to in the LED.
   Param - message: the message number to be displayed
   Param - wordLen: the length of the message being displayed
   Param - i: the position of the first letter to be displayed
   Return - void
*/
void writeMessage(int message, int wordLen, int i) {
	//CLears all of the LED displays before setting the next display
	for (int pos = 0; pos < NUM_DISPLAYS; pos++) {
		displayWrite(0, pos);
	}
	//increments each individual letter of the word
	for (int letter = 0; letter < wordLen; letter++) {
		
		//only displays letter on the LED if it is in a valid display position
		if (i - letter >=0 && i-letter<8) { 
			//displays the letters of the first message if user entered 1
			if (message == 1) {
				writeFood(letter, i - letter);
			}

			//displays the letters of the second message if user entered 2
			else if (message == 2) {
				writeSleep(letter, i - letter);
			}
		}
	}
}


/*
   Purpose: Display the letters "Food" on the LED to their assigned position
   Param - index: the index (letter position) of the array
   Param - pos: the position to be displayed on the LED Screen
   Return - void
*/
void writeFood(int index, int pos) {
	int foodarray[4] = { 142, 58, 58, 122 }; //the numbers to light up "Food"
	displayWrite(foodarray[index], pos);
}

/* 
   Purpose: Display the letters "SLEEP" on the LED to their assigned position
   Param - index: the index (letter position) of the array
   Param - pos: the position to be displayed on the LED Screen
   Return - void
*/
void writeSleep(int index, int pos) {
	int sleeparray[5] = { 182, 28, 158, 158, 206 }; //the numbers to light up "SLEEP"
	displayWrite(sleeparray[index], pos);
}