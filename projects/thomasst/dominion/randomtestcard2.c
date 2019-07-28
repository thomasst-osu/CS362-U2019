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
#include <math.h>

#define TESTCARD "minion"
#define MAX_TESTS 5000

/* BUSINESS REQUIREMENTS:  MINION **********************************
1)  Player receives an extra "Action" play
2)  Player can choose to receive +2 coins with no other game effects
3)  Player can alternatively choose to discard hand, draw 4 cards,
    and force all other players with 5 cards in their hand to discard
    and draw 4 cards.
********************************************************************/
int printParams(int player, struct gameState *before, struct gameState *after){
  // numActions
  if (after->numActions != before->numActions){
    printf("\tNumber of Actions = %d, expected %d\n", after->numActions, before->numActions);
  }

  // Coins
  if (after->coins != before->coins){
    printf("\tCoins = %d, expected %d\n", after->coins, before->coins);
  }

  // Player hand count
  if (after->handCount[player] != before->handCount[player]){
    printf("\tPlayer Hand Count = %d, expected %d\n", after->handCount[player], before->handCount[player]);
  }

  // Player hand content
  if (memcmp(after->hand[player], before->hand[player], sizeof(int) * before->handCount[player])){
    printf("\tContents of Player Hand do not match.\n");
  }

  // Player deck count
  if (after->deckCount[player] != before->deckCount[player]){
    printf("\tPlayer Deck Count = %d, expected %d\n", after->deckCount[player], before->deckCount[player]);
  }

  // Player deck content
  if (memcmp(after->deck[player], before->deck[player], sizeof(int) * before->deckCount[player])){
    printf("\tContents of Player Deck do not match.\n");
  }

  // Player discard count
  if (after->discardCount[player] != before->discardCount[player]){
    printf("\tPlayer Discard Count = %d, expected %d\n", after->discardCount[player], before->discardCount[player]);
  }

  // Player discard content
  if (memcmp(after->discard[player], before->discard[player], sizeof(int) * before->discardCount[player])){
    printf("\tContents of Player Discard do not match.\n");
  }

  // Other players hand count/content, deck count/content, discard count/content
  for (int i = 0; i < before.numPlayers; i++){
    if (i != player){
      // Player hand count
      if (after->handCount[i] != before->handCount[i]){
        printf("\tOther Hand Count = %d, expected %d\n", after->handCount[i], before->handCount[i]);
      }

      // Player hand content
      if (memcmp(after->hand[i], before->hand[i], sizeof(int) * before->handCount[i])){
        printf("\tContents of Other Player Hand do not match.\n");
      }

      // Player deck count
      if (after->deckCount[i] != before->deckCount[i]){
        printf("\tOther Player Deck Count = %d, expected %d\n", after->deckCount[i], before->deckCount[i]);
      }

      // Player deck content
      if (memcmp(after->deck[i], before->deck[i], sizeof(int) * before->deckCount[i])){
        printf("\tContents of Other Player Deck do not match.\n");
      }

      // Player discard count
      if (after->discardCount[i] != before->discardCount[i]){
        printf("\tOther Player Discard Count = %d, expected %d\n", after->discardCount[i], before->discardCount[i]);
      }

      // Player discard content
      if (memcmp(after->discard[i], before->discard[i], sizeof(int) * before->discardCount[i])){
        printf("\tContents of Other Player Discard do not match.\n");
    }
  }
}

int testCard(int choice1, int choice2, int player, int handpos, struct gameState *after){
  // cardEffect(int card, int choice1, int choice2, int choice3, struct gameState *state, int handpos, int *bonus)
  // playMinionCard(int choice1, int choice2, int currentPlayer, int handpos, struct gameState *state)

  int choice3 = 0, bonus = 0;
  struct gameState before;
  memcpy(&before, after, sizeof(struct gameState));

  cardEffect(minion, choice1, choice2, choice3, after, handpos, &bonus);

  // Mimick anticipated behavior in before game state
  before.numActions++;

  if (choice1){
    before.coins += 2;
  }
  else if (choice2){
    if (before.deckCount[player] >= 4){
      for (int i = 0; i < before.handCount[player]; i++){
        before.discard[player][before.discardCount[player]] = before.hand[player][i];
        before.discardCount[player]++;
        before.hand[player][i] = -1;
        before.handCount[player]--;
      }

      for (int i = 0; i < 4; i++){
        before.hand[player][before.handCount[player]] = before.deck[player][before.deckCount[player] - 1];
        before.deckCount[player]--;
      }
    }
    else {
      before.discardCount[player] = 0;
      before.handCount[player] = 4;
      memcpy(before.hand[player], after->hand[player], sizeof(int) * before.handCount[player]);
      memcpy(before.deckCount[player], after->deckCount[player], sizeof(int));
      memcpy(before.deck[player], after->deck[player], sizeof(int) * before.deckCount[player]);
    }

    for (int i = 0; i < before.numPlayers; i++){
      if (i != player){
        if (before.deckCount[i] >= 4){
          for (int j = 0; j < before.handCount[i]; j++){
            before.discard[i][before.discardCount[i]] = before.hand[i][j];
            before.discardCount[i]++;
            before.hand[i][j] = -1;
            before.handCount[i]--;
          }

          for (int j = 0; j < 4; j++){
            before.hand[i][before.handCount[i]] = before.deck[i][before.deckCount[i] - 1];
            before.deckCount[i]--;
          }
        }
        else {
          before.discardCount[i] = 0;
          before.handCount[i] = 4;
          memcpy(before.hand[i], after->hand[i], sizeof(int) * before.handCount[i]);
          memcpy(before.deckCount[i], after->deckCount[i], sizeof(int));
          memcpy(before.deck[i], after->deck[i], sizeof(int) * before.deckCount[i]);
        }
      }
    }

  }
  else {

  }

  if (memcmp(&before, after, sizeof(struct gameState)) == 0){
    printf("Random Test PASSED\n");
  } else {
    printf("Random Test FAILED (choice1 = %d, choice2 = %d)\n", choice1, choice2);
    printParams(player, &before, after);
  }

}

int main(){
  /* Parameters used in Minion:
   * 1)  choice1 - determines if player wants +2 coins
   * 2)  choice2 - determines if player wants to discard hand and redraw 4
   * 3)  valid player choice
   * 4)  valid hand position
   * 4)  valid game state

   * Variables changed within function:
   * 1)  numActions
   * 2)  coins
   * 3)  discard count/content
   * 4)  hand count/content
   * 5)  deck count/content
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
    state.whoseTurn = player;
    state.numPlayers = floor((Random() * 3) + 2);
    player = floor(Random() * state.numPlayers);
    choice1 = floor(Random() * 2);
    choice2 = floor(Random() * 2);
    state.deckCount[player] = floor(Random() * MAX_DECK);
    state.handCount[player] = floor(Random() * MAX_HAND);
    state.discardCount[player] = floor(Random() * MAX_DECK);
    state.coins = floor(Random() * 10);
    state.numActions = floor(Random() * 10);
    handpos = floor(Random() * state.handCount[player]);

    // Populate all players' hands
    for (int k = 0; k < state.numPlayers; k++){
      // Populate hand randomly
    	for (int x = 0; x < state.handCount[player]; x++){
    		state.hand[k][x] = floor(Random() * (treasure_map + 1));
    	}

    	// Populate deck randomly
    	for (int x = 0; x < state.deckCount[player]; x++){
    		state.deck[k][x] = floor(Random() * (treasure_map + 1));
    	}

    	// Populate discard randomly
    	for (int x = 0; x < state.discardCount[player]; x++){
    		state.discard[k][x] = floor(Random() * (treasure_map + 1));
    	}
    }

    testCard(choice1, choice2, player, handpos, &state);
  }

  return 0;
}
