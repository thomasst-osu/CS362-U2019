/*  File Name:
**  Author:       Stephen Thomas
**  Date:         7/28/19
**  Description:  This file contains the random testing procedures for the
**    baron card for the game of dominion.  It establishes important parameters
**    for testing the card and then randomly assigns values to these parameters
**    for testing.
*/

#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define TESTCARD "baron"
#define MAX_TESTS 5000

/* BUSINESS REQUIREMENTS:  BARON ***********************************
1)  If no estates are currently in hand, player draws an estate
2)  If player has an estate, they can discard in exchange for +4 coins
3)  Player can elect to draw an estate instead of +4 coins and retain
    current estate in hand.
4)  Player receives an extra buy.
********************************************************************/
int printParams(){

}

int isEstateInHand(){

}

int testCard(int player, int choice1, struct gameState *after){
  // cardEffect(int card, int choice1, int choice2, int choice3, struct gameState *state, int handpos, int *bonus)
  // playBaronCard(int choice1, int currentPlayer, struct gameState *state)
}

int main(){
  /* Parameters used in Baron:
   * 1) Choice1 (logic pivots around 0.  Try to balance calls around 0.)
   * 2) Valid player choice (cap at max players)
   * 3) Valid game state (see next section for specific state required)

   * Variables changed within function:
   * 1) numBuys
   * 2) coins
   * 3) handCount / hand (estate card in hand changes logic)
   * 4) discard / discardCount
   * 5) deck / deckCount (utilized in gainCard call)
   * 6) supply count for estate cards
  */

  printf("---------------- TESTING CARD:  %s ----------------\n", TESTCARD);

  struct gameState state;
  int player = 0, choice1 = 0;
  SelectStream(2);
  PutSeed(500);

  for (int i = 0; i < MAX_TESTS; i++){
    // Put random values into the game state
    for (int j = 0; j < sizeof(struct gameState); j++){
      ((char*)&state)[j] = floor(Random() * 256);
    }
    player = floor(Random() * MAX_PLAYERS);
    choice1 = floor(Random() * 11) - 5;   // -5 to 5 range
    state.deckCount[player] = floor(Random() * MAX_DECK);
    state.handCount[player] = floor(Random() * MAX_HAND);
    state.discardCount[player] = floor(Random() * MAX_DECK);

    testCard(player, choice1, &state);
  }


  printf("\n################## TESTING COMPLETE:  %s ##################\n\n\n", TESTCARD);
  return 0;
}
