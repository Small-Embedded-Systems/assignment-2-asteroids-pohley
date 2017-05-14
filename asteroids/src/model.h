/* Asteroids Model */
struct point {
    float x,y;
};
typedef struct point coordinate_t;
typedef struct point vector_t;

/* Some insitial struct types if you want to usd them */
struct ship {
    coordinate_t p;
    vector_t     v;
		double heading;
	int lives;
};

/* initial struts for building linked lists */

typedef struct rock { 
 	  float heading; 
    coordinate_t p; 
 	  vector_t     v; 
 	  float ttl; 
		struct rock *next; 
 } rock_t; 

typedef struct missile {
		float heading; 
    coordinate_t p;
    struct missile *next;
		vector_t     v; 
		float ttl;
}missile_t;


void physics(void);

void LeftTurn();
void RightTurn();
void Forwards();
void Backwards();
void initialise_asteroids();
void initialise_missiles();
rock_t *allocate_rock_node(void); 
missile_t *allocate_missile_node(void);
void free_rock_node(rock_t *n); 
void free_missile_node(missile_t *n); 	
