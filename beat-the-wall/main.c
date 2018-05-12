#include "gameplay.h"

// Variable declarations
time_t startTime;
time_t endTime;
int hits;
char score[10];
double wallWidth = -SCREEN_WIDTH/100;

int main(int argc, char **argv)
{
  Game game;

  initialiseWindow(&game.gameVideo);
  initialisePlay(&game.gameData);
  gameMainWhile(&game);
  endGame(&game);

  return 0;
}
