#include "render.h"

/**
 * Check for key presses and act accordingly
 */
void handleKey(GameData *gameData, SDL_Event *event);

/**
 * Clamp x to top or bottom of the screen
 */
float edgeLimit(float x, float maxY);

/**
 * Move paddle according to its speed and screen boundaries
 */
void movePaddle(Paddle *paddle, Uint32 delta_ms);

/**
 * Move ball according to its speed and screen boundaries
 */
void moveBall(Ball *ball, Uint32 delta_ms);

/**
 * Move all elements simultaneously
 */
void playMovements(GameData *gameData, Uint32 delta_ms);
