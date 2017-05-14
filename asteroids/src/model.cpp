/* Asteroids model */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#include "model.h"
#include "utils.h"
#include "asteroids.h"



static void moveShip(ship);
static void update_rock_list(struct rock *l);

static rock_t rock_data[20];
static missile_t missile_data[2];
static missile_t *freeMissileNodes;
static void update_missile_list(struct missile *s);
void new_missile(struct missile *r);
static rock_t *freeRockNodes;


//private methods 
static void shipCrash(struct rock *l);
static  void missileHit(struct missile *l, struct rock *r);
struct rock * new_asteroids();
void move_asteroids(struct rock *l, float x);
struct missile * new_missile();
struct missile * new_missile();
static void move_missile(struct missile *l, float x);




void physics(void)
{
	 shipCrash(asteroids);
	missileHit(shots, asteroids);
 moveShip(player);   
asteroids=new_asteroids();
update_rock_list(asteroids);
move_asteroids(asteroids, Dt);
shots=new_missile();
update_missile_list(shots);
move_missile(shots, Dt);
void fire();


	
}


//****************************Ateroid*********************


// creates asteroid with random values 
struct rock * new_asteroids(){
	int i;
	for(i = 0; i < 20-1; i++){
		struct rock *newRock = allocate_rock_node();
		if(newRock){
			newRock->next = asteroids;
			asteroids = newRock;
			
					newRock->p.x = randrange(0,ScreenX);
					newRock->p.y = randrange(0,ScreenY);
					newRock->v.x = randrange(0,60);
					newRock->v.y = randrange(0,20);
						
			newRock->ttl = 2000;
		}
		update_rock_list(asteroids);
	}
	return asteroids;
}



void initialise_asteroids(){
		int n;
			for(n=0; n<(20-1) ; n++){
				rock_data[n].next = &rock_data[n+1];
			}
		rock_data[n].next = NULL;
		freeRockNodes = &rock_data[0];	
}

rock_t *allocate_rock_node(void){
		rock_t *node = NULL;
			if (freeRockNodes){
				node = freeRockNodes;
				freeRockNodes = freeRockNodes->next;
	}
	return node;
}

void update_rock_list(struct rock *l){
		for ( ; l ; l = l->next){
				if (l->next->ttl <= 0){
					struct rock *expiredR = l->next;
					l->next = l->next->next;
					free_rock_node(expiredR);
			}		
		}
}

void free_rock_node(rock_t *n){
	n->next = freeRockNodes;
	freeRockNodes = n;
}
	

// moves asteroid and spawns asteroid on other side of map if cordinates go out 
void move_asteroids(struct rock *l, float x){
	for ( ; l ; l = l->next){
		l->p.x += l->v.x * x;
		l->p.y += l->v.y * x;
		if (l->p.y >= 272) 	l->p.y -= 272;
		if (l->p.y < 0) 		l->p.y += 272;
		if (l->p.x >= ScreenX) 	l->p.x -= ScreenX;
		if (l->p.x < 0) 		l->p.x += ScreenX;
		
		l->ttl -= Dt;	
	}	
}
///////////////////////////////////////////////START MISSILE



// creates missile with cordinates based on player 
struct missile * new_missile(){
	int i;
	for(i = 0; i < 1; i++){
		struct missile *newMissile = allocate_missile_node();
		if(newMissile){
			newMissile->next = shots;
			shots = newMissile;
			newMissile->heading = player.heading; 	
			newMissile->p.x = player.p.x; 
			newMissile->p.y = player.p.y; 
			newMissile->v.x = sin(radians(newMissile->heading)); 
			newMissile->v.y = -(cos(radians(newMissile->heading))); 
			newMissile->p.x += newMissile->v.x*20; 
			newMissile->p.y += newMissile->v.y*20; 
			newMissile->v.x *=500;  	newMissile->v.y *=500; 
			newMissile->ttl = 20; 					
			newMissile->ttl = 2000;
		}
		update_missile_list(shots);
	}
	return shots;
}


//initailises the missile list 
void initialise_missiles(){
		int n;
		for(n=0; n<(1) ; n++){
		missile_data[n].next = &missile_data[n+1];
	}
		missile_data[n].next = NULL;
		freeMissileNodes = &missile_data[0];	
}


missile_t *allocate_missile_node(void){
		missile_t *node = NULL;
		if (freeMissileNodes){
			node = freeMissileNodes;
			freeMissileNodes = freeMissileNodes->next;
		}
		return node;
	}

void update_missile_list(struct missile *l){
	for ( ; l ; l = l->next){
		if (l->next->ttl <= 0){
		struct missile *expiredR = l->next;
		l->next = l->next->next;
		free_missile_node(expiredR);
		}		
	}
}

void free_missile_node(missile_t *n){
	n->next = freeMissileNodes;
	freeMissileNodes = n;
}
	
//UPDATES THE MISSILES CORDINATES
void move_missile(struct missile *l, float x){
	for ( ; l ; l = l->next){
		l->p.x += l->v.x * x;
		l->p.y += l->v.y * x;
		
		if (l->p.y >= 272) 	l->ttl = 0;
		if (l->p.y < 0) 			l->ttl = 0;
		if (l->p.x >= ScreenX) 		l->ttl = 0;
		if (l->p.x < 0) 			l->ttl = 0;
		
		l->ttl -= Dt;	
	}	
}


// DETECTS IF A MISSILE HITS THE ASTEROID 
 void missileHit(struct missile *l, struct rock *r){
	for(;l;l=l->next){
		for( ; r ; r =r->next){
		float r1=1;
		float r2=20;
		float dx = l->p.x - r->p.x;
		float dy = l->p.y- r->p.y;
		float distance=sqrt(dx*dx+dy*dy);
				if(distance<(r1+r2)){
						l->ttl=0;
						r->ttl=0;
					score=score+5;
			}
		}
	}
}

		 
		// code to take away lives in crash  
	void takeDamage(){
	if(sheilds!=0){
		sheilds=sheilds-1;
		}
	else {

		lives=lives-1;
	}
	}
	
	
	//detects if a ship hits asteroid 
 void shipCrash(struct rock *l){ 
		for( ; l ; l =l->next){ 
			// rough cordinates for astroifd and ship to gerate hitbox
 			float r1 = 10;  
			float r2 = 20; 
			float dx = l->p.x - player.p.x; float dy = l->p.y - player.p.y; 
			float distance = sqrt(dx*dx + dy*dy); 
 			if (distance < (r1 + r2)){
					l->ttl = 0;
				takeDamage();
			} 
 		} 
 
 } 
 	 






/////////////////////////////////////////////ship

 // changes ships heading
void LeftTurn(){
	player.heading +=30;
	
}

void RightTurn (){
	player.heading -=30;
	
}
void Forwards(){
	float sfx, sfy;
	sfx=sin(radians(player.heading));
	sfy=cos(radians(player.heading));
	player.v.x +=sfx;
	player.v.y -=sfy;
	
	
}

void Backwards(){
	player.v.x=	player.v.x*.5;
	player.v.y=	player.v.y*.5;
}


// updates the ships cordinates
void moveShip(ship){
	player.p.x=player.v.x+player.p.x;
	player.p.y=player.v.y+player.p.y;
	
	// graduly slows hipd down my multiplying by <1
	player.v.x=	player.v.x*.998;
	player.v.y=	player.v.y*.998;
			
	// if croidnates out of bounds s
	if (player.p.x <0){
		player.p.x=ScreenX;
	}
		if (player.p.x >ScreenX){
		player.p.x=0;
	}
		if (player.p.y <0){
			
		player.p.y=gameScreenY;// game screen y = screen-display box
	}
		if (player.p.y >ScreenY){
		player.p.y=gameScreenY;
	}
}


	
	