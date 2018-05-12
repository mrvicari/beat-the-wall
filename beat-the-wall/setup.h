#include "data.h"

/**
 * Setup window, otherwise print error
 */
void initialiseWindow (GameVideo *gameVideo);

/**
 * Set the ball off in a random direction
 */
void toggle_ball (Ball *ball);

/**
 * Re-position ball in starting place
 */
void resetBall (Ball *ball);

/**
 * Prepare variables to begin game
 */
void initialisePlay(GameData *gameData);

/**
 * Close the game
 */
void endGame(Game *game);

/**
 * Restart game
 */
void resetGame(GameData *gameData);
