/*  File Name:
**  Author:       Stephen Thomas
**  Date:         7/28/19
**  Description:  This file contains the random testing procedures for the
**    minion card for the game of dominion.  It establishes important parameters
**    for testing the card and then randomly assigns values to these parameters
**    for testing.
*/

#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define TESTCARD "minion"
#define MAX_TESTS 5000

/* BUSINESS REQUIREMENTS:  MINION **********************************
1)  Player receives an extra "Action" play
2)  Player can choose to receive +2 coins with no other game effects
3)  Player can alternatively choose to discard hand, draw 4 cards,
    and force all other players with 5 cards in their hand to discard
    and draw 4 cards.
********************************************************************/
int printParams(){

}

int testCard(){

}

int main(){
  /* Parameters used in Minion:
   * 1)

   * Variables changed within function:
   * 1)
  */

  printf("---------------- TESTING CARD:  %s ----------------\n", TESTCARD);

  struct gameState state;
  int player = 0, choice1 = 0, choice2 = 0, choice3 = 0, handpos = 0, bonus = 0;
  SelectStream(2);
  PutSeed(500);

  for (int i = 0; i < MAX_TESTS; i++){
    // Put random values into the game state
    for (int j = 0; j < sizeof(struct gameState); j++){
      ((char*)&state)[j] = floor(Random() * 256);
    }
    player = floor(Random() * MAX_PLAYERS);
    state.deckCount[player] = floor(Random() * MAX_DECK);
    state.handCount[player] = floor(Random() * MAX_HAND);
    state.discardCount[player] = floor(Random() * MAX_DECK);
  }

  return 0;
}
