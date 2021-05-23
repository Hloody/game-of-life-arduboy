#include <Arduboy2.h>

Arduboy2 arduboy;

const uint8_t grid_width = 128 / 4, grid_height = 64 / 4;
bool grid[ grid_width * grid_height ];
bool play = false;

void renderGrid() {
  
 	arduboy.clear();
	
	for ( uint8_t y = 0; y != grid_height; y++ ) 
		for ( uint8_t x = 0; x != grid_width; x++ )
			arduboy.fillRect( x * 4, y * 4, 4, 4, grid[ x + y * grid_width ] == 1 ? WHITE : BLACK ); 

	arduboy.display();

}

void nextGen() {
	bool nextGenGrid[ grid_width * grid_height ];

 	for ( uint8_t y = 0; y != grid_height; ++y ) {
		for ( uint8_t x = 0; x != grid_width; ++x ) {
			bool left  = x != 0;
			bool up    = y != 0;
			bool right = x != grid_width - 1;
			bool down  = y != grid_height - 1;
      
			bool currently_alive = grid[ x + y * grid_width ];
      
			// This code causes strict-overflow warnings that can be safely ignored.
			byte neighbours =
				( left  && up   && grid[ ( x - 1 ) + ( y - 1 ) * grid_width ] ? 1 : 0 ) +
				( left  &&         grid[ ( x - 1 ) +   y       * grid_width ] ? 1 : 0 ) +
				( left  && down && grid[ ( x - 1 ) + ( y + 1 ) * grid_width ] ? 1 : 0 ) +
				( up    &&         grid[   x       + ( y - 1)  * grid_width ] ? 1 : 0 ) +
				( down  &&         grid[   x       + ( y + 1)  * grid_width ] ? 1 : 0 ) +
				( right && up   && grid[ ( x + 1 ) + ( y - 1 ) * grid_width ] ? 1 : 0 ) +
				( right &&         grid[ ( x + 1 ) +   y       * grid_width ] ? 1 : 0 ) +
				( right && down && grid[ ( x + 1 ) + ( y + 1 ) * grid_width ] ? 1 : 0 );
      
			if ( currently_alive ) nextGenGrid[ x + y * grid_width ] = neighbours == 2 || neighbours == 3;
			else                   nextGenGrid[ x + y * grid_width ] = neighbours == 3;

		}
	}

	for ( int i = 0; i < grid_width * grid_height; i++ ) grid[ i ] = nextGenGrid[ i ];
}

void generateGrid() {

	for ( int i = 0; i < grid_width * grid_height; i++ ) grid[ i ] = random( 0, 3 ) == 0;

}

void setup() {
	arduboy.begin();

	arduboy.setFrameRate( 30 );
	arduboy.initRandomSeed();
	generateGrid();
}

void loop() {

	arduboy.pollButtons();

	if ( arduboy.justPressed( A_BUTTON ) ) { generateGrid(); play = false; };
	if ( arduboy.justPressed( B_BUTTON ) ) play = !play;

  if ( !play && arduboy.justPressed( RIGHT_BUTTON ) ) nextGen();

	if ( !arduboy.nextFrame() ) return;

	renderGrid();

	if ( play && arduboy.everyXFrames( 5 ) ) nextGen();
 
}
