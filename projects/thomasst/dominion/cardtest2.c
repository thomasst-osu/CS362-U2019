/*  File Name:    cardtest2.c
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

# define TESTCARD "shuffle"

/* BUSINESS REQUIREMENTS:  shuffle *********************************
1)  Takes in a player request and gameState struct
2)  Verifies there are cards in the requested deck
3)  Sorts the cards in the deck by qsort
4)  Uses the random number generator to randomly pick out cards from
    the sorted deck and place them in a temporary placeholder deck.
5)  Places the cards back in the real deck after all have been "shuffled"
********************************************************************/

int main(){
  /* Intialize testing and game variables */
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
  /* Action:  Normal shuffle test with full deck.
   * Expected Response:  Random re-ordering of cards.
   * Variables:  -none-
   * Setup:  Initialize a new 2 player game, force 10 different cards into hand,
      call shuffle.
	*NOTE:  Using nextPlayer because thisPlayer only has 5 cards in deck vs 10
   */
  printf("\nTest 1:  Initialize game and shuffle a full deck for player 2.\n");

  /* Scenario setup */
  for (int i = 0; i < state.deckCount[nextPlayer]; i++){
   state.deck[nextPlayer][i] = i + adventurer;    // load all cards in deck with different card
  }

  // Make a copy for comparison
  memcpy(&testState, &state, sizeof(struct gameState));
  int success = shuffle(nextPlayer, &testState);

  int matches = 0;

  // Compare decks
  for (int i = 0; i < testState.deckCount[nextPlayer]; i++){
   if (testState.deck[nextPlayer][i] == state.deck[nextPlayer][i]){
     matches++;
   }
  }
  if (matches == testState.deckCount[nextPlayer]){
   printf("Assert Test FAILED\n\tAll cards matched original deck after shuffle.\n");
  } else if (matches == 0){
   printf("Assert Test PASSED\n\tNo cards matched original deck after shuffle.\n");
  } else {
   printf("Assert Test PASSED\n\t%d out of %d cards matched original deck after shuffle.\n", matches, testState.deckCount[nextPlayer]);
  }

  // Compare return value
  assertTrue(success == 0, "Shuffle Function Return Value", success, 0);



  /* ******************************* TEST 2 ******************************** */
  /* Action:  Shuffle call with 0 cards in deck
  * Expected Response:  No deck response.  Return value of -1
  * Variables:  -none-
  * Setup:  Reduce deckCount variable to 0, run shuffle function.
  */
  printf("\nTest 2:  Attempt to run shuffle function on deck with 0 cards.\n");

  /* Scenario setup */
  state.deckCount[thisPlayer] = 0;
  success = 0;

  /* Make a copy for comparison */
  memcpy(&testState, &state, sizeof(struct gameState));
  success = shuffle(thisPlayer, &testState);

  /* Ensure deck count remained at 0 */
  assertTrue(testState.deckCount[thisPlayer] == state.deckCount[thisPlayer], "Player Deck Count",
    testState.deckCount[thisPlayer], state.deckCount[thisPlayer]);

  /* Compare return value */
  assertTrue(success == -1, "Shuffle Function Return Value", success, -1);



  printf("\n################## TESTING COMPLETE:  %s ##################\n", TESTCARD);

  return 0;
}
