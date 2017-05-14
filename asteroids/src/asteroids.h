/* Game state */

extern float elapsed_time; /* time this ship has been active */
extern int   score;        /* total score so far */
extern int   lives;        /* lives remaining */
extern int 	ScreenX;			/* screen width*/
extern int ScreenY;				/* screen height */
extern int sheilds;				/* player */
extern bool paused;				
extern bool gameOver;
extern int gameScreenY;
extern struct ship player;
extern struct rock *asteroids; /* array of rocks / pointer to linked-list */
extern struct missile *shots;  /* array of missiles / pointer to linked-list */
extern const float Dt; /* Time step for physics, needed for consistent motion */
