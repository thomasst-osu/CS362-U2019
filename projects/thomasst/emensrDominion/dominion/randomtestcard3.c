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
#include <math.h>

#define TESTCARD "Tribute"
#define MAX_TESTS 25000

/* BUSINESS REQUIREMENTS:  TRIBUTE *********************************
1)  Player after current player must reveal top 2 cards from hand
2)  Action cards are worth +2 actions
3)  Treasure cards are worth +2 coins
4)  Victory cards are worth +2 cards
5)  To get both bonuses, the cards must be different
6)  If a particular card has multiple types, the player receives
    both bonuses for that card.
********************************************************************/
int printParams(int player, int nextPlayer, struct gameState *before, struct gameState *after){
  // Coins
  if (after->coins != before->coins){
    printf("\tCoins = %d, expected %d\n", after->coins, before->coins);
  }

  // NumActions
  if (after->numActions != before->numActions){
    printf("\tNumber of Actions = %d, expected %d\n", after->numActions, before->numActions);
  }

  // Cards in Hand
  if (after->handCount[player] != before->handCount[player]){
    printf("\tPlayer Hand Count = %d, expected %d\n", after->handCount[player], before->handCount[player]);
  }

  // Other player deck count
  if (after->deckCount[nextPlayer] != before->deckCount[nextPlayer]){
    printf("\tNext Player Deck Count = %d, expected %d\n", after->deckCount[nextPlayer], before->deckCount[nextPlayer]);
  }

  // Other player discard count
  if (after->discardCount[nextPlayer] != before->discardCount[nextPlayer]){
    printf("\tNext Player Discard Count = %d, expected %d\n", after->discardCount[nextPlayer], before->discardCount[nextPlayer]);
  }

  return 0;
}

int testCard(int player, struct gameState *after){
  int choice1 = 0, choice2 = 0, choice3 = 0, handpos = 0, bonus = 0;
  struct gameState before, beforeCopy;
  memcpy(&before, after, sizeof(struct gameState));
  memcpy(&beforeCopy, after, sizeof(struct gameState));

  cardEffect(tribute, choice1, choice2, choice3, after, handpos, &bonus);

  // Mimick anticipated behavior in before game state
  int nextPlayer = player + 1;
  int tributeCards[2] = {-1, -1};

  if (nextPlayer >= before.numPlayers){
    nextPlayer = 0;
  }

  if (before.deckCount[nextPlayer] >= 2){
    tributeCards[0] = before.deck[nextPlayer][before.deckCount[nextPlayer] - 1];
    before.deck[nextPlayer][before.deckCount[nextPlayer] - 1] = -1;
    before.deckCount[nextPlayer]--;
    tributeCards[1] = before.deck[nextPlayer][before.deckCount[nextPlayer] - 1];
    before.deck[nextPlayer][before.deckCount[nextPlayer] - 1] = -1;
    before.deckCount[nextPlayer]--;
    before.discard[nextPlayer][before.discardCount[nextPlayer]] = tributeCards[0];
    before.discardCount[nextPlayer]++;
    before.discard[nextPlayer][before.discardCount[nextPlayer]] = tributeCards[1];
    before.discardCount[nextPlayer]++;
  } else if (before.deckCount[nextPlayer] == 1){
    tributeCards[0] = before.deck[nextPlayer][before.deckCount[nextPlayer] - 1];
    if (before.discardCount[nextPlayer] > 0){
      tributeCards[1] = after->discard[nextPlayer][0];
      for (int i = 0; i < before.discardCount[nextPlayer]; i++){
        before.discard[nextPlayer][i] = -1;
      }
      before.deckCount[nextPlayer] = before.discardCount[nextPlayer] - 1;
      before.discardCount[nextPlayer] = 0;
    }

  } else if (before.deckCount[nextPlayer] <= 0){
    if (before.discardCount[nextPlayer] >= 2){
      tributeCards[0] = after->discard[nextPlayer][0];
      tributeCards[1] = after->discard[nextPlayer][1];
      before.deckCount[nextPlayer] = before.discardCount[nextPlayer] - 2;
      for (int i = 0; i < before.discardCount[nextPlayer]; i++){
        before.discard[nextPlayer][i] = -1;
      }
      before.discardCount[nextPlayer] = 0;
    } else if (before.discardCount[nextPlayer] == 1){
      tributeCards[0] = after->discard[nextPlayer][0];
    }
  }

  if (tributeCards[0] == tributeCards[1]){
    before.playedCards[before.playedCardCount] = tributeCards[1];
    before.playedCardCount++;
    tributeCards[1] = -1;
  }

  for (int i = 0; i < 2; i++){
    if (tributeCards[i] == copper || tributeCards[i] == silver || tributeCards[i] == gold){
      before.coins = before.coins + 2;
    }
    else if (tributeCards[i] == estate || tributeCards[i] == duchy || tributeCards[i] == province || tributeCards[i] == gardens || tributeCards[i] == great_hall){
      before.handCount[player] = before.handCount[player] + 2;
      before.deckCount[player] = before.deckCount[player] - 2;
    }
    else if (tributeCards[i] >= adventurer && tributeCards[i] <= treasure_map){
      before.numActions = before.numActions + 2;
    }
  }

  if (memcmp(&before, after, sizeof(struct gameState)) == 0){
    printf("Random Test PASSED\n");
  } else {
    printf("Random Test FAILED (next player deck = %d, next player discard = %d)\n", beforeCopy.deckCount[nextPlayer], beforeCopy.discardCount[nextPlayer]);
    printParams(player, nextPlayer, &before, after);
  }

  return 0;
}

int main(){
  /* Parameters used in Tribute:
   * 1)  valid current player
   * 2)  valid next player
   * 3)  2 integer array for revealed tribute cards
   * 4)  valid game state

   * Variables changed within function:
   * 1)  next player deck count/content
   * 2)  next player discard count/content
   * 3)  playedCards / playedCardCount
   * 4)  player discard count/content
   * 5)  coins
   * 6)  numActions
  */

  printf("---------------- TESTING CARD:  %s ----------------\n", TESTCARD);

  struct gameState state;
  int player = 0;
  SelectStream(2);
  PutSeed(500);

  for (int i = 0; i < MAX_TESTS; i++){
    // Put random values into the game state
    for (int j = 0; j < sizeof(struct gameState); j++){
      ((char*)&state)[j] = floor(Random() * 256);
    }
    state.numPlayers = floor((Random() * 3) + 2);
    player = floor(Random() * state.numPlayers);
    state.whoseTurn = player;
    state.numActions = floor(Random() * 10);
    state.coins = floor(Random() * 10);
	state.playedCardCount = 0;

    // Populate all players' hands
		for (int k = 0; k < state.numPlayers; k++){
			state.deckCount[k] = floor(Random() * MAX_DECK);
			state.handCount[k] = floor(Random() * MAX_HAND);
			state.discardCount[k] = floor(Random() * MAX_DECK);

			for (int x = 0; x < state.deckCount[k]; x++){
				state.deck[k][x] = floor(Random() * (treasure_map + 1));
			}
			for (int x = 0; x < state.handCount[k]; x++){
				state.hand[k][x] = floor(Random() * (treasure_map + 1));
			}
			for (int x = 0; x < state.discardCount[k]; x++){
				state.discard[k][x] = floor(Random() * (treasure_map + 1));
			}
		}

    testCard(player, &state);
  }

  return 0;
}
