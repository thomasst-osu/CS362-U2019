/*  File Name:
**  Author:       Stephen Thomas
**  Date:         7/28/19
**  Description:  This file contains the random testing procedures for the
**    Tribute card for the game of dominion.  It establishes important parameters
**    for testing the card and then randomly assigns values to these parameters
**    for testing.
*/

#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define TESTCARD "Tribute"
#define MAX_TESTS 5000

/* BUSINESS REQUIREMENTS:  TRIBUTE *********************************
1)  Player after current player must reveal top 2 cards from hand
2)  Action cards are worth +2 actions
3)  Treasure cards are worth +2 coins
4)  Victory cards are worth +2 cards
5)  To get both bonuses, the cards must be different
6)  If a particular card has multiple types, the player receives
    both bonuses for that card.
********************************************************************/
int printParams(){

}

int testCard(){

}

int main(){
  /* Parameters used in Tribute:
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
