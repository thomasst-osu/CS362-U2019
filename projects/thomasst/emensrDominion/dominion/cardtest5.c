/*  File Name:    cardtest5.c
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

# define TESTCARD "drawCard"

/* BUSINESS REQUIREMENTS:  drawCard ********************************
1)  Checks to see if the deck is empty
2)  If empty, moves all discarded cards into the deck, shuffles them,
    and then adds the last deck card to the player's hand
3)  If discard is empty, does nothing
4)  If not empty, moves the last deck card into player's hand
********************************************************************/

int main(){
  /* Intialize testing and game variables */
  int numPlayers = 2;
  int thisPlayer = 0;
  int seed = 1000;
  int k[10] = {adventurer, gardens, village, minion, mine, cutpurse,
                sea_hag, tribute, smithy, council_room};

  int discarded = 0, newCards = 1;
  struct gameState state, testState;
  initializeGame(numPlayers, k, seed, &state);

  // int drawCard(int player, struct gameState *state);


  printf("---------------- TESTING FUNCTION:  %s ----------------\n", TESTCARD);



  /* ******************************* TEST 1 ******************************** */
  /* Action:  Draw card with 5 cards in hand and 5 in deck.
   * Expected Response:  6 cards in hand, 4 in deck, 0 in discard.
   * Variables:  -none-
   * Setup:  Initialize game, call drawCard for player 1
   */
  printf("\nTest 1:  Draw one card with 5 in hand and 5 in deck.\n");

  /* Scenario setup */
  discarded = 0;
  newCards = 1;

  // Make a copy for comparison
  memcpy(&testState, &state, sizeof(struct gameState));
  drawCard(thisPlayer, &testState);

  // Test discard pile
  assertTrue(testState.discardCount[thisPlayer] == state.discardCount[thisPlayer] + discarded,
    "Player Discard Count", testState.discardCount[thisPlayer],
    state.discardCount[thisPlayer] + discarded);

  // Test hand count
  assertTrue(testState.handCount[thisPlayer] == state.handCount[thisPlayer] + newCards,
    "Player Hand Count", testState.handCount[thisPlayer],
    state.handCount[thisPlayer] + newCards);

  // Test deck count
  assertTrue(testState.deckCount[thisPlayer] == state.deckCount[thisPlayer] - newCards,
    "Player Deck Count", testState.deckCount[thisPlayer],
    state.deckCount[thisPlayer] - newCards);

  // Test specific card picked up
  assertTrue(testState.hand[thisPlayer][testState.handCount[thisPlayer] - 1] == state.deck[thisPlayer][state.deckCount[thisPlayer] - 1],
    "Card Drawn", testState.hand[thisPlayer][testState.handCount[thisPlayer] - 1],
    state.deck[thisPlayer][state.deckCount[thisPlayer] - 1]);



  /* ******************************* TEST 2 ******************************** */
  /* Action:  Draw card with 0 cards in deck, 5 cards in discard pile, 5 cards in hand
  * Expected Response:  4 cards in deck, 6 cards in hand, last card in hand = ambassador
  * Variables: -none-
  * Setup:  Force 5 ambassadors into discard pile, set deck count to 0
  */
  printf("\nTest 2:  Draw one card with 5 in hand, 0 in deck, and 5 in discard.\n");

  /* Scenario setup */
  discarded = 0;
  newCards = 1;
  state.deckCount[thisPlayer] = 0;
  state.discardCount[thisPlayer] = 5;
  for (int i = 0; i < 5; i++){
    state.discard[thisPlayer][i] = ambassador;
  }

  // Make a copy for comparison
  memcpy(&testState, &state, sizeof(struct gameState));
  drawCard(thisPlayer, &testState);

  // Test discard pile
  assertTrue(testState.discardCount[thisPlayer] == 0,
    "Player Discard Count", testState.discardCount[thisPlayer], 0);

  // Test hand count
  assertTrue(testState.handCount[thisPlayer] == state.handCount[thisPlayer] + newCards,
    "Player Hand Count", testState.handCount[thisPlayer],
    state.handCount[thisPlayer] + newCards);

  // Test deck count
  assertTrue(testState.deckCount[thisPlayer] == 4,
    "Player Deck Count", testState.deckCount[thisPlayer], 4);

  // Test specific card picked up
  assertTrue(testState.hand[thisPlayer][testState.handCount[thisPlayer] - 1] == ambassador,
    "Card Drawn", testState.hand[thisPlayer][testState.handCount[thisPlayer] - 1], ambassador);



  /* ******************************* TEST 3 ******************************** */
  /* Action:  Draw card with 0 cards in deck and 0 cards in discard
   * Expected Response:  No state change, return value -1
   * Variables:  -none-
   * Setup:  Set deck count to 0, set discard count to 0
   */
   printf("\nTest 3:  Attempt to draw a card with 0 in discard and 0 in deck.\n");

   /* Scenario setup */
   discarded = 0;
   newCards = 0;
   state.discardCount[thisPlayer] = 0;


   // Make a copy for comparison
   memcpy(&testState, &state, sizeof(struct gameState));
   int success = drawCard(thisPlayer, &testState);

   // Test discard pile
   assertTrue(testState.discardCount[thisPlayer] == 0,
     "Player Discard Count", testState.discardCount[thisPlayer], 0);

   // Test hand count
   assertTrue(testState.handCount[thisPlayer] == state.handCount[thisPlayer] + newCards,
     "Player Hand Count", testState.handCount[thisPlayer],
     state.handCount[thisPlayer] + newCards);

   // Test deck count
   assertTrue(testState.deckCount[thisPlayer] == 0,
     "Player Deck Count", testState.deckCount[thisPlayer], 0);
   
   // Test return value
   assertTrue(success == -1, "Function Return Value", success, -1);




  printf("\n################## TESTING COMPLETE:  %s ##################\n\n\n", TESTCARD);

  return 0;
}
