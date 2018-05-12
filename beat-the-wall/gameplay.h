#include "movement.h"

/**
 * Check for the ball colliding with the paddle
 *
 *@return true if there is collision, false otherwise
 */
bool paddleBallCollision(Ball *ball, Paddle *paddle);

/**
 * Calculate direction of ball after collisions
 */
void ballCollision(GameData *gameData, Uint32 delta_ms);

/**
 * Play movements and check for collisions
 */
void play(GameData *gameData, Uint32 now_ms);

/**
 * Loop gameplay functions
 */
void gameMainWhile(Game *game);
