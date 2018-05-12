#include "setup.h"

/**
 * Render paddle on the screen
 */
void renderPaddle(SDL_Renderer *renderer, int x, int y);

/**
 * Render wall on the screen
 */
void renderWall(SDL_Renderer *renderer, int x, int y);

/**
 * Render ball on the screen
 */
void renderBall(SDL_Renderer *renderer, Ball *ball);

/**
 * Render all gameplay components on the screen
 */
void renderGame(SDL_Renderer *renderer, GameData *gameData);
