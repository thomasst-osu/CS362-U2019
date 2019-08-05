/*  File Name:    unittest3.c
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

# define TESTCARD "ambassador"

/* BUSINESS REQUIREMENTS:  AMBASSADOR ******************************
1)  Player reveals a card from hand
2)  Player elects how many copies (0, 1, or 2) of the card to return to the supply
    * NOTE:  the copies must come from the player's hand directly
3)  Each other player takes a copy of the card from the supply
4)  If there are not enough in the supply for everyone, the cards are dealt
    in play order (clockwise)
********************************************************************/


int main(){
  /* Initialize testing and game variables */
  int discarded = 0;

  int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
  int seed = 1000;
  int numPlayers = 3;
  int thisPlayer = 0;

  struct gameState state, testState;
  int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
                sea_hag, tribute, smithy, council_room};

  /* Initialize game state and player cards */
  initializeGame(numPlayers, k, seed, &state);

  /* Cards in player's hand must be controlled for this unit test */
  state.hand[thisPlayer][0] = ambassador;
  state.hand[thisPlayer][1] = copper;
  state.hand[thisPlayer][2] = copper;
  state.hand[thisPlayer][3] = copper;
  state.hand[thisPlayer][4] = copper;


  printf("---------------- TESTING CARD:  %s ----------------\n", TESTCARD);



  /* ******************************* TEST 1 ******************************** */
  /* Action:  Player reveals a copper, but does contribute any to the supply.
   * Expected Response:  Other player(s) pick up a copper from supply
   * Variables:  Choice 1 = 1 (copper hand position), choice 2 = 0 (0 copies)
   * State Setup:  -none...setup above-
   */
  printf("\nTest 1:  Player reveal a copper, but does not add any to the supply.  Choice 1 = 1 (copper hand position), choice 2 = 0 (0 copies).\n");

  /* Scenario setup */
  choice1 = 1;
  choice2 = 0;

  // Copy original game state to a test state
  memcpy(&testState, &state, sizeof(struct gameState));
  cardEffect(ambassador, choice1, choice2, choice3, &testState, handpos, &bonus);

  // Verify the player did not get rid of any coppers
  int stateCopperCount = 0;
  int testStateCopperCount = 0;
  for (int i = 0; i < state.handCount[thisPlayer]; i++){
    if (state.hand[thisPlayer][i] == copper){
      stateCopperCount++;
    }
  }

  for (int i = 0; i < testState.handCount[thisPlayer]; i++){
    if (testState.hand[thisPlayer][i] == copper){
      testStateCopperCount++;
    }
  }
  assertTrue(testStateCopperCount == stateCopperCount, "Copper in Hand",
            testStateCopperCount, stateCopperCount);

  // Verify other players picked up (in discard pile) a copper
  for (int i = 0; i < state.numPlayers; i++){
    if (i != thisPlayer){
      assertTrue(testState.discardCount[i] == 1, "Other Player Discard",
                testState.discardCount[i], 1);
    }
  }

  // Verify the supply for coppers
  assertTrue(testState.supplyCount[copper] == state.supplyCount[copper] - (numPlayers - 1),
            "Copper Supply", testState.supplyCount[copper],
            state.supplyCount[copper] - (numPlayers - 1));



  /* ******************************* TEST 2 ******************************** */
  /* Action:  Player reveals a copper and contributes 2 back to the supply.
   * Expected Response:  Other player(s) pick up a copper from supply.
   * Variables:  Choice 1 = 1 (copper hand position), choice 2 = 2 (2 copies)
   * State Setup:  -none-
   */

  // NOTE:  Player has 2 coppers in hand.
  printf("\nTest 2:  Player reveals a copper and contributes 2 back to the supply.  Choice 1 = 1 (copper hand position), choice 2 = 2 (2 copies).\n");
  choice1 = 1;
  choice2 = 2;
  discarded = 2;

  // Copy original game state to a test state
  memcpy(&testState, &state, sizeof(struct gameState));
  cardEffect(ambassador, choice1, choice2, choice3, &testState, handpos, &bonus);

  // Verify the player got rid of 2 cards
  assertTrue(testState.handCount[thisPlayer] == state.handCount[thisPlayer] - discarded,
            "Player Hand Count", testState.handCount[thisPlayer],
            state.handCount[thisPlayer] - discarded);

  // Verify the player got rid of copper cards only
  stateCopperCount = 0;
  testStateCopperCount = 0;
  for (int i = 0; i < state.handCount[thisPlayer]; i++){
    if (state.hand[thisPlayer][i] == copper){
      stateCopperCount++;
    }
  }

  for (int i = 0; i < testState.handCount[thisPlayer]; i++){
    if (testState.hand[thisPlayer][i] == copper){
      testStateCopperCount++;
    }
  }

  assertTrue(testStateCopperCount == stateCopperCount - discarded, "Player Copper Count",
            testStateCopperCount, stateCopperCount - discarded);

  // Verify the player only discarded the played card
  assertTrue(testState.discardCount[thisPlayer] == state.discardCount[thisPlayer] + 1,
            "Player Discard Count", testState.discardCount[thisPlayer],
            state.discardCount[thisPlayer] + 1);

  // Verify the other players picked up (in discard pile) a copper
  for (int i = 0; i < state.numPlayers; i++){
    if (i != thisPlayer){
      assertTrue(testState.discardCount[i] == state.discardCount[i] + 1,
              "Other Player Discard Count", testState.discardCount[i],
              state.discardCount[i] + 1);
    }
  }

  // Verify the supply count for coppers (should have stayed the same)
  assertTrue(testState.supplyCount[copper] == state.supplyCount[copper],
            "Copper Supply", testState.supplyCount[copper], state.supplyCount[copper]);



  /* ******************************* TEST 3 ******************************** */
  /* Action:  Player reveals a copper and requests to put 2 in the supply pile
      while only holding one.
   * Expected Response:  State remains unchanged.  Action not allowed.
   * Variables: Choice 1 = 1 (copper hand position), choice 2 = 2 (2 copies)
   * State Setup:  Change player's hand [2 - 4] to silver.  Should leave player
      with only 1 copper.
   */
  printf("\nTest 3:  Player reveals a copper and requests to put 2 in the supply pile, but only holds one.  Choice 1 = 1 (copper hand position), choice 2 = 2 (2 copies).\n");

  /* Scenario setup (ambassador already in position 0, copper in 1)*/
  state.hand[thisPlayer][2] = silver;
  state.hand[thisPlayer][3] = silver;
  state.hand[thisPlayer][4] = silver;
  choice1 = 1;
  choice2 = 2;

  // Copy original game state to a test state
  memcpy(&testState, &state, sizeof(struct gameState));
  cardEffect(ambassador, choice1, choice2, choice3, &testState, handpos, &bonus);

  // Verify state remained unchanged due to insufficient number of coppers
  assertTrue(testState.handCount[thisPlayer] == state.handCount[thisPlayer],
            "Player Hand Count", testState.handCount[thisPlayer],
            state.handCount[thisPlayer]);

  assertTrue(testState.discardCount[thisPlayer] == state.discardCount[thisPlayer],
            "Player Discard Count", testState.discardCount[thisPlayer],
            state.discardCount[thisPlayer]);

  for (int i = 0; i < state.numPlayers; i++){
    if (i != thisPlayer){
      assertTrue(testState.discardCount[i] == state.discardCount[i],
                "Other Player Discard Count", testState.discardCount[i],
                state.discardCount[i]);
    }
  }

  assertTrue(testState.supplyCount[copper] == state.supplyCount[copper],
            "Copper Supply", testState.supplyCount[copper],
            state.supplyCount[copper]);



  /* ******************************* TEST 4 ******************************** */
  /* Action:  Player reveals a copper, does not add to supply pile, no coppers in supply.
   * Expected Response:  No state change as other players cannot draw from supply.
   * Variables: Choice 1 = 1 (copper hand position), choice 2 = 0 (0 copies)
   * State Setup:  Change copper supply to 0
   */
  printf("\nTest 4:  Player reveals a copper, but does not add any to the supply pile.  The supply pile is empty.  Choice 1 = 1 (copper hand position), choice 2 = 0 (0 copies).\n");

  /* Scenario setup */
  state.supplyCount[copper] = 0;
  choice1 = 1;
  choice2 = 0;
  discarded = 1;

  // Copy original game state to a test state
  memcpy(&testState, &state, sizeof(struct gameState));
  cardEffect(ambassador, choice1, choice2, choice3, &testState, handpos, &bonus);

  // Verify state remained unchanged due to insufficient number of coppers in supply
  // NOTE:  The card is still played and, as such, the player should have a discard.
  assertTrue(testState.handCount[thisPlayer] == state.handCount[thisPlayer] - discarded,
          "Player Hand Count", testState.handCount[thisPlayer],
          state.handCount[thisPlayer] - discarded);

  assertTrue(testState.discardCount[thisPlayer] == state.discardCount[thisPlayer] + discarded,
          "Player Discard Count", testState.discardCount[thisPlayer],
          state.discardCount[thisPlayer] + discarded);

  for (int i = 0; i < state.numPlayers; i++){
    if (i != thisPlayer){
      assertTrue(testState.discardCount[i] == state.discardCount[i],
                "Other Player Discard Count", testState.discardCount[i],
                state.discardCount[i]);
    }
  }

  assertTrue(testState.supplyCount[copper] == state.supplyCount[copper],
          "Copper Supply", testState.supplyCount[copper], state.supplyCount[copper]);



  /* ******************************* TEST 5 ******************************** */
  /* Action:  Player reveals copper and contributes 1 to an empty supply pile.
   * Expected Response:  Only next player in order gets a copy of copper.
   * Variables: Choice 1 = 1 (copper hand position), choice 2 = 1 (1 copy)
   * State Setup:  -none, carryover from previous test-
   * NOTE:  There are 3 players in these unit tests
   */
  printf("\nTest 5:  Player reveals a copper, contributes one to an empty supply pile.  Choice 1 = 1 (copper hand position), choice 2 = 1 (1 copy).\n");

  /* Scenario setup */
  choice1 = 1;
  choice2 = 1;

  // Copy original game state to a test state
  memcpy(&testState, &state, sizeof(struct gameState));
  cardEffect(ambassador, choice1, choice2, choice3, &testState, handpos, &bonus);

  // Verify player's hand count
  assertTrue(testState.handCount[thisPlayer] == state.handCount[thisPlayer] - 1 - 1,
          "Player Hand Count", testState.handCount[thisPlayer],
          state.handCount[thisPlayer] - 1 - 1);

  // Verify player did not discard copper
  stateCopperCount = 0;
  testStateCopperCount = 0;

  for (int i = 0; i < state.discardCount[thisPlayer]; i++){
    if (state.discard[thisPlayer][i] == copper){
      stateCopperCount++;
    }
  }
  for (int i = 0; i < testState.discardCount[thisPlayer]; i++){
    if (testState.discard[thisPlayer][i] == copper){
      testStateCopperCount++;
    }
  }

  assertTrue(testStateCopperCount == stateCopperCount, "Discarded Copper",
          testStateCopperCount, stateCopperCount);

  // Verify player didn't retain any coppers
  stateCopperCount = 0;
  testStateCopperCount = 0;

  for (int i = 0; i < state.handCount[thisPlayer]; i++){
    if (state.hand[thisPlayer][i] == copper){
      stateCopperCount++;
    }
  }

  for (int i = 0; i < testState.handCount[thisPlayer]; i++){
    if (testState.hand[thisPlayer][i] == copper){
      testStateCopperCount++;
    }
  }

  assertTrue(testStateCopperCount == stateCopperCount - 1, "Copper in Hand",
          testStateCopperCount, stateCopperCount);

  // Verify next player received a copper
  assertTrue(testState.discardCount[thisPlayer + 1] == 1, "2nd Player Discard Count",
          testState.discardCount[thisPlayer + 1], 1);

  if (testState.discardCount[thisPlayer + 1] > 0){
    assertTrue(testState.discard[thisPlayer + 1][0] == copper, "2nd Player Copper in Discard",
              testState.discard[thisPlayer + 1][0], copper);
  }
  // Verify 3rd player did not get a copper
  assertTrue(testState.discardCount[testState.numPlayers - 1] == state.discardCount[state.numPlayers - 1],
            "3rd Player Discard Count", testState.discardCount[testState.numPlayers - 1],
            state.discardCount[state.numPlayers - 1]);

  // Verify supply count
  assertTrue(testState.supplyCount[copper] == state.supplyCount[copper],
          "Copper Supply", testState.supplyCount[copper], state.supplyCount[copper]);



  printf("\n################## TESTING COMPLETE:  %s ##################\n\n\n", TESTCARD);

  return 0;

}