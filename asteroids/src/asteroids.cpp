/* Asteroids
    Sample solution for assignment
    Semester 2 -- Small Embedded Systems
    Dr Alun Moon
*/

/* C libraries */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

/* hardware platform libraries */
#include <display.h>
#include <mbed.h>

/* Main game elements */
#include "model.h"
#include "view.h"
#include "controller.h"

/* Game state */
float elapsed_time; 
int   score;
int   lives;
int sheilds;
bool paused;
bool gameOver;
int ScreenX=480;
int ScreenY=260;
int gameScreenY=ScreenY-230;
struct ship player;
void initialise();
void ButtonPressed();
void setPlayer();
struct rock *asteroids = NULL;
struct missile *shots = NULL;
float Dt = 0.01f;


Ticker model, view, controller;


/* The single user button needs to have the PullUp resistor enabled */
DigitalIn userbutton(P2_10,PullUp);
int main()
{
		paused = true;
    init_DBuffer();
    
		
    view.attach( draw, 0.025);
    model.attach( physics, Dt);
    controller.attach( controls, 0.1);
		initialise();
    ButtonPressed();
  
		
    /* Pause to start */
  
  
		//waits for game over
			while(true){
			if (lives<0){
				paused=true;
				ButtonPressed();
				}
		
    }
	}
	
			//calls methods to start game
		void initialise(){
			
			initialise_asteroids();
			initialise_missiles();
			setPlayer();
			
		}
		// waits for button to be pressed before satrting new game
		
		void ButtonPressed(){
			  while( userbutton.read() ){ /* remember 1 is not pressed */
        paused=true;
        wait_ms(100);
			
    }
		paused = false;
		gameOver=false;
		setPlayer();
	}
		
	//sets the game up for player for new game
		void setPlayer(){
			
			score=0;
			sheilds=3;
			lives=3;
			player.p.x=200;
			player.p.y=200;
			player.v.x=00.00;
			player.v.y=00.00;
			player.heading=100;
		}
		
		
		
		
		