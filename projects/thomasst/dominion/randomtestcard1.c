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
#include <math.h>
#include <stdbool.h>

#define TESTCARD "baron"
#define MAX_TESTS 5000

/* BUSINESS REQUIREMENTS:  BARON ***********************************
1)  If no estates are currently in hand, player draws an estate
2)  If player has an estate, they can discard in exchange for +4 coins
3)  Player can elect to draw an estate instead of +4 coins and retain
    current estate in hand.
4)  Player receives an extra buy.
********************************************************************/
int printParams(int player, struct gameState *before, struct gameState *after){
	
	// Coins
	if (after->coins != before->coins){
		printf("\tCoins = %d, expected %d\n", after->coins, before->coins);
	}
	
	// Num Buys
	if (after->numBuys != before->numBuys){
		printf("\tNumber of Buys = %d, expected %d\n", after->numBuys, before->numBuys);
	}
	
	// Discard Count
	if (after->discardCount[player] != before->discardCount[player]){
		printf("\tDiscard Count = %d, expected %d\n", after->discardCount[player], before->discardCount[player]);
	}

	// Discard Contents
	if (memcmp(after->discard[player], before->discard[player], sizeof(int) * before->discardCount[player]) != 0){
		printf("\tContents of Discard Pile do not match.\n");
	}

	// Hand Count
	if (after->handCount[player] != before->handCount[player]){
		printf("\tHand Count = %d, expected %d\n", after->handCount[player], before->handCount[player]);
	}

	// Hand Contents
	if (memcmp(after->hand[player], before->hand[player], sizeof(int) * before->handCount[player]) != 0){
		printf("\tContents of Hand do not match.\n");
	}

	// Deck Count
	if (after->deckCount[player] != before->deckCount[player]){
		printf("\tDeck Count = %d, expected %d\n", after->deckCount[player], before->deckCount[player]);
	}

	// Deck Contents
	if (memcmp(after->deck[player], before->deck[player], sizeof(int) * before->deckCount[player]) != 0){
		printf("\tContents of Deck do not match.\n");
	}

	// Estate supply
	if (after->supplyCount[estate] != before->supplyCount[estate]){
		printf("\tEstate Supply = %d, expected %d\n", after->supplyCount[estate], before->supplyCount[estate]);
	}


	return 0;
}

int isEstateInHand(int player, struct gameState *state){
	int inHand = -1;
	for (int i = 0; i < state->handCount[player]; i++){
		if (state->hand[player][i] == estate){
			inHand = i;
		}
	}

	return inHand;
}

int testCard(int player, int choice1, struct gameState *after){
  // cardEffect(int card, int choice1, int choice2, int choice3, struct gameState *state, int handpos, int *bonus)
  // playBaronCard(int choice1, int currentPlayer, struct gameState *state)

	int choice2 = 0, choice3 = 0, handpos = 0, bonus = 0;
	struct gameState before;
	memcpy(&before, after, sizeof(struct gameState));

	cardEffect(baron, choice1, choice2, choice3, after, handpos, &bonus);

	// Mimick behavior in before game state
	int estatePos = isEstateInHand(player, &before);
	int estatePosCopy = estatePos;
	before.numBuys++;

	if (choice1 > 0){
		if (estatePos != -1){
			before.coins += 4;
			before.discardCount[player]++;
			before.discard[player][before.discardCount[player] - 1] = estate;
			for (; estatePos < before.handCount[player]; estatePos++){
				before.hand[player][estatePos] = before.hand[player][estatePos + 1];
			}
			before.hand[player][before.handCount[player]] = -1;
			before.handCount[player]--;
		}
		else {
			if (before.supplyCount[estate] > 0){
				before.supplyCount[estate]--;
				before.discardCount[player]++;
				before.discard[player][before.discardCount[player] - 1] = estate;
			}
		}
	}
	else {
		if (before.supplyCount[estate] > 0){
			before.supplyCount[estate]--;
			before.discardCount[player]++;
			before.discard[player][before.discardCount[player] - 1] = estate;
		}
	}
	
	// Compare game states, if different print out details
	if (memcmp(&before, after, sizeof(struct gameState)) == 0){
		printf("Random Test PASSED\n");
	} else {
		printf("Random Test FAILED (choice1 = %d, estateInHand = %d)\n", choice1, estatePosCopy);
		printParams(player, &before, after);
	}
	return 0;
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
	state.supplyCount[estate] = floor(Random() * 10);
	state.coins = floor(Random() * 10);
	state.numBuys = floor(Random() * 10);
	state.whoseTurn = player;

	// Populate hand randomly
	for (int k = 0; k < state.handCount[player]; k++){
		state.hand[player][k] = floor(Random() * (treasure_map + 1));
	} 

	// Populate deck randomly
	for (int k = 0; k < state.deckCount[player]; k++){
		state.deck[player][k] = floor(Random() * (treasure_map + 1));
	}

	// Populate discard randomly
	for (int k = 0; k < state.discardCount[player]; k++){
		state.discard[player][k] = floor(Random() * (treasure_map + 1));
	}

    testCard(player, choice1, &state);
  }


  printf("\n################## TESTING COMPLETE:  %s ##################\n\n\n", TESTCARD);
  return 0;
}
