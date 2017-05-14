/* Asteroids view
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

#include "asteroids.h"
#include "model.h"
#include "utils.h"

Display *graphics = Display::theDisplay();
static  void drawRocks(struct rock *lst);
static  void drawMissiles(struct missile *lst);
static void drawship();
static void drawSheild();
static void output();

const colour_t background = rgb(0,51,102); /* Midnight Blue */


const coordinate_t shape[] = {
    {0.0f,-10.0f}, {5.0f,8.0f}, {-5.0f,8.0f}
};

/* double buffering functions */
void init_DBuffer(void)
{   /* initialise the LCD driver to use second frame in buffer */
    uint16_t *bufferbase = graphics->getFb();
    uint16_t *nextbuffer = bufferbase+ (480*272);
    LPC_LCD->UPBASE = (uint32_t)nextbuffer;
}

void swap_DBuffer(void)
{   /* swaps frames used by the LCD driver and the graphics object */
    uint16_t *buffer = graphics->getFb();
    graphics->setFb( (uint16_t*) LPC_LCD->UPBASE);
    LPC_LCD->UPBASE = (uint32_t)buffer;
}

// main draw function calling other mehods
void draw(void)
{
	
	if( paused){ 
		graphics->fillScreen(BLACK);
		graphics->setTextColor(WHITE);
		graphics->setTextSize(7);
		graphics->setCursor(30,20); 
		graphics->printf("Asteroids");  
		graphics->setCursor(100,150); 
		graphics->setTextSize(2);
		graphics->printf("press the back button to start"); 
		graphics->setCursor(100,180); 		
		graphics->printf("Edward Pohl CM0506"); 
 		}   
	

	
	else{
   graphics->fillScreen(background);
	drawship();
	drawSheild();
	drawRocks(asteroids);
	output();
	drawMissiles(shots);
	}
swap_DBuffer();
}

void drawship(){
	// geraratres new cordinates for the ship
	float p1x=(shape[0].x*cos(radians(player.heading))-(shape[0].y)*sin(radians(player.heading)));
	float p1y=(shape[0].x*sin(radians(player.heading))+(shape[0].y)*cos(radians(player.heading)));
	float p2x=(shape[1].x*cos(radians(player.heading))-(shape[1].y)*sin(radians(player.heading)));
	float p2y=(shape[1].x*sin(radians(player.heading))+(shape[1].y)*cos(radians(player.heading)));
	float p3x=(shape[2].x*cos(radians(player.heading))-(shape[2].y)*sin(radians(player.heading)));
	float p3y=(shape[2].x*sin(radians(player.heading))+(shape[2].y)*cos(radians(player.heading)));
	//draws trinagle based on cordinates
	graphics->drawTriangle( player.p.x+p1x,player.p.y+p1y
												, player.p.x+p2x,player.p.y+p2y,
													player.p.x+p3x,player.p.y+p3y,
													RED);
}



//draws sheild based on players lives 
void drawSheild(){
	
	if (sheilds==3){
		graphics->drawCircle(player.p.x,player.p.y,10,BLUE);
	}
	if (sheilds==2){
		graphics->drawCircle(player.p.x,player.p.y,10,GREEN);
	}
if (sheilds==1){
		graphics->drawCircle(player.p.x,player.p.y,10,RED);
	}
}

// draws the top screen for output
void 	 output(){
					graphics->fillRect(0, 0,ScreenX,(gameScreenY), BLACK);
					
					graphics->setTextColor(BLACK);
					graphics->setTextColor(RED);
					graphics->setCursor(50,5);
					graphics->setTextColor(RED);
					graphics->printf("lives %i" , lives);
					graphics->setTextColor(WHITE);
					graphics->setCursor(200,5);
					graphics->printf("score %i" , score);
					
					
	
					
}


void drawRocks(struct rock *lst){ 
		while(lst){ 
			graphics->drawCircle(lst->p.x, lst->p.y,20,GREEN); 
			graphics->fillCircle(lst->p.x, lst->p.y,20,GREEN);
			lst = lst->next;
		}	 
 } 

 
 
void drawMissiles(struct missile *lst){ 
	
	while(lst){ 
 		graphics->drawCircle(lst->p.x, lst->p.y,1,WHITE); 
		lst = lst->next;
 	}	 
}




