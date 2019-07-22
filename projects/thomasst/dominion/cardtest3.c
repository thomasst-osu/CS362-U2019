/*  File Name:    cardtest3.c
**  Author:       Stephen Thomas
**  Date:         7/21/19
**  Description:
*/

#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include "unittest_helper.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

# define TESTCARD "endTurn"

/* BUSINESS REQUIREMENTS:  endTurn *********************************
1)  Discards all cards in player's hands directly to the discard pile
    (not using discardCard).
2)  Player's hand count is reset to 0
3)  Next player is placed into state's whoseTurn variable
4)  Other state variables are reset for next player
5)  5 cards are drawn for the new player
6)  New player's coins are updated using the updateCoins function
********************************************************************/
// int endTurn(struct gameState *state);

int main(){
  /* Initialize testing and game variables */
  int numPlayers = 2;
  int thisPlayer = 0;
  int nextPlayer = thisPlayer + 1;
  int seed = 1000;
  int k[10] = {adventurer, gardens, village, minion, mine, cutpurse,
                sea_hag, tribute, smithy, council_room};
  struct gameState state, testState;
  initializeGame(numPlayers, k, seed, &state);

  printf("---------------- TESTING FUNCTION:  %s ----------------\n", TESTCARD);

  /* ******************************* TEST 1 ******************************** */
  /* Action:  End player 1's turn.
   * Expected Response:  5 cards in discard, 0 cards in hand, switch to player 2's turn,
      player 2 has 5 cards in hand, 5 cards in deck, coins updated based on player 2's
      cards in hand.
   * Variables:  state, testState for testing state variables
   * Setup:  Initialize 2 player game and call endTurn function
   */
  printf("\nTest 1:  Initialize game and end player 1's turn.\n");

  /* Make a copy for comparison */
  memcpy(&testState, &state, sizeof(struct gameState));
  endTurn(&testState);

  // Test player 1's hand count
  assertTrue(testState.handCount[thisPlayer] == state.handCount[thisPlayer] - 5,
    "Player 1 Hand Count", testState.handCount[thisPlayer], state.handCount[thisPlayer] - 5);

  // Test player 1's discard count
  assertTrue(testState.discardCount[thisPlayer] == state.discardCount[thisPlayer] + 5,
    "Player 1 Discard Count", testState.discardCount[thisPlayer],
    state.discardCount[thisPlayer] + 5);

  // Test contents of player 1's discard
  for (int i = 0; i < testState.discardCount[thisPlayer]; i++){
    assertTrue(testState.discard[thisPlayer][i] == state.hand[thisPlayer][i],
      "Player 1 Discard Contents", testState.discard[thisPlayer][i],
      state.hand[thisPlayer][i]);
  }

  // Test player 2's hand count
  assertTrue(testState.handCount[nextPlayer] == state.handCount[nextPlayer] + 5,
    "Player 2 Hand Count", testState.handCount[nextPlayer], state.handCount[nextPlayer] + 5);

  // Test player 2's deck count
  assertTrue(testState.deckCount[nextPlayer] == state.deckCount[nextPlayer] - 5,
    "Player 2 Deck Count", testState.deckCount[nextPlayer], state.deckCount[nextPlayer] - 5);

  //Test player 2's coin count (only coppers so far)
  int coinCount = 0;
  for (int i = 0; i < testState.handCount[nextPlayer]; i++){
    if (testState.hand[nextPlayer][i] == copper){
      coinCount++;
    }
  }
  assertTrue(coinCount == testState.coins, "Player 2 Coins", coinCount, testState.coins);

  // Test whose turn it is
  assertTrue(testState.whoseTurn == nextPlayer, "Whose Turn", testState.whoseTurn, nextPlayer);



  /* ******************************* TEST 2 ******************************** */
  /* Action:  End player 1 and player 2's turns returning play back to player 1.
  * Expected Response:  Player 1 5 cards in hand, 0 cards in deck, 5 cards in
      discard. Player 2 0 cards in hand, 5 cards in deck, 5 cards in discard.
  * Variables:
  * Setup:
  */
  printf("\nTest 2:  Initialize game, end player 1's turn, end player 2's turn.\n");

  /* Make a copy for comparison */
  memcpy(&testState, &state, sizeof(struct gameState));
  endTurn(&testState);
  endTurn(&testState);

  // Test player 1's hand count
  assertTrue(testState.handCount[thisPlayer] == state.handCount[thisPlayer],
    "Player 1 Hand Count", testState.handCount[thisPlayer], state.handCount[thisPlayer]);

  // Test player 1's discard count
  assertTrue(testState.discardCount[thisPlayer] == state.discardCount[thisPlayer] + 5,
    "Player 1 Discard Count", testState.discardCount[thisPlayer],
    state.discardCount[thisPlayer] + 5);

  // Test player 1's deck count
  assertTrue(testState.deckCount[thisPlayer] == state.deckCount[thisPlayer] - 10,
    "Player 1 Deck Count", testState.deckCount[thisPlayer], state.deckCount[thisPlayer] - 10);

  //Test player 1's coin count (only coppers so far)
  coinCount = 0;
  for (int i = 0; i < testState.handCount[thisPlayer]; i++){
    if (testState.hand[thisPlayer][i] == copper){
      coinCount++;
    }
  }
  assertTrue(coinCount == testState.coins, "Player 1 Coins", coinCount, testState.coins);

  // Test player 2's hand count
  assertTrue(testState.handCount[nextPlayer] == state.handCount[nextPlayer],
    "Player 2 Hand Count", testState.handCount[nextPlayer], state.handCount[nextPlayer]);

  // Test player 2's discard count
  assertTrue(testState.discardCount[nextPlayer] == state.discardCount[nextPlayer] + 5,
    "Player 2 Discard Count", testState.discardCount[nextPlayer],
    state.discardCount[nextPlayer] + 5);

  // Test player 2's deck count
  assertTrue(testState.deckCount[nextPlayer] == state.deckCount[nextPlayer] - 5,
    "Player 2 Deck Count", testState.deckCount[nextPlayer], state.deckCount[nextPlayer] - 5);

  // Test whose turn it is
  assertTrue(testState.whoseTurn == thisPlayer, "Whose Turn", testState.whoseTurn, thisPlayer);





  printf("\n################## TESTING COMPLETE:  %s ##################\n", TESTCARD);

  return 0;
}
// Test with current player = 0
// Test with current player = last player
