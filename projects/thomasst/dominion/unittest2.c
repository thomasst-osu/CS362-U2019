/*  File Name:    unittest2.c
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

# define TESTCARD "minion"

/* BUSINESS REQUIREMENTS:  MINION **********************************
1)  Player receives an extra "Action" play
2)  Player can choose to receive +2 coins with no other game effects
3)  Player can alternatively choose to discard hand, draw 4 cards,
    and force all other players with 5 cards in their hand to discard
    and draw 4 cards.
********************************************************************/


int main(){
  /* Initialize testing and game variables */
  int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
  int seed = 1000;
  int numPlayers = 2;
  int thisPlayer = 0;
  int newCards = 0, discarded = 0, extraCoins = 0, addActions = 1;    // test variables

  struct gameState state, testState;
  int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
                sea_hag, tribute, smithy, council_room};

  /* Initialize game state and player cards */
  initializeGame(numPlayers, k, seed, &state);

  printf("---------------- TESTING CARD:  %s ----------------\n", TESTCARD);

  /* ******************************* TEST 1 ******************************** */
  /* Action:  Player chooses to receive 2 coins.
   * Expected Response:  Player discards minion card, receives 2 coins, receives 1 action
   * Variables:  Choice 1 = 1
   * State Setup:  - none -
   */
  printf("\nTest 1:  Player chooses to receive +2 coins.  Choice 1 = 1.\n");

  /* Scenario setup */
  choice1 = 1;
  discarded = 1;
  extraCoins = 2;

  // Copy original game state to a test state
  memcpy(&testState, &state, sizeof(struct gameState));
  cardEffect(minion, choice1, choice2, choice3, &testState, handpos, &bonus);


  // Verify number of actions increased by 1.
  assertTrue(testState.numActions == state.numActions + addActions, "Action Count",
            testState.numActions, state.numActions + addActions);

  // Verify the player discarded minion card
  assertTrue(testState.discardCount[thisPlayer] == state.discardCount[thisPlayer] + discarded,
            "Discard Count", testState.discardCount[thisPlayer],
            state.discardCount[thisPlayer] + discarded);

  // Verify the player received two extra coins
  assertTrue(testState.coins == state.coins + extraCoins, "Coins",
            testState.coins, state.coins + extraCoins);

  // Verify the other players' hands did not change
  for (int i = 0; i < state.numPlayers; i++){
    if (i != thisPlayer){
      assertTrue(testState.discardCount[i] == state.discardCount[i], "Other Player Discard Count",
                testState.discardCount[i], state.discardCount[i]);
    }
  }


  /* ******************************* TEST 2 ******************************** */
  /* Action:  Player chooses to discard hand.
   * Expected Response:  Player discards hand, draws 4 cards, other players
      discard hands (all players have 5 in hand) and draw 4 from deck.
   * Variables:  Choice 1 = 0, choice 2 = 1
   * State Setup: -none-
   */
  printf("\nTest 2:  Player chooses to discard hand and receive 4 cards.  All players start with 5 cards.  Choice 1 = 0, choice 2 = 1.\n");

  /* Scenario setup */
  choice1 = 0;
  choice2 = 1;
  extraCoins = 0;
  newCards = 4;
  discarded = 5;

  // Copy original game state to a test state
  memcpy(&testState, &state, sizeof(struct gameState));
  cardEffect(minion, choice1, choice2, choice3, &testState, handpos, &bonus);

  // Verify number of actions increased by 1.
  assertTrue(testState.numActions == state.numActions + addActions, "Action Count",
            testState.numActions, state.numActions + addActions);

  // Verify player discarded 5 cards from hand
  assertTrue(testState.discardCount[thisPlayer] == state.discardCount[thisPlayer] + discarded,
            "Discard Count", testState.discardCount[thisPlayer],
            state.discardCount[thisPlayer] + discarded);

  // Verify player picked up 4 cards into hand
  assertTrue(testState.handCount[thisPlayer] == state.handCount[thisPlayer] + newCards - discarded,
            "New Cards in Hand", testState.handCount[thisPlayer],
            state.handCount[thisPlayer] + newCards - discarded);

  // Verify other players discarded 5 cards from hand
  for (int i = 0; i < state.numPlayers; i++){
    if (i != thisPlayer){
      assertTrue(testState.discardCount[i] == state.discardCount[i] + discarded,
                "Other Player Discard Count", testState.discardCount[i],
                state.discardCount[i] + discarded);
    }
  }

  // Verify other players picked up 4 cards into hand
  for (int i = 0; i < state.numPlayers; i++){
    if (i != thisPlayer){
      assertTrue(testState.handCount[i] == state.handCount[i] + newCards - discarded,
                "Other Player Hand Count", testState.handCount[i],
                state.handCount[i] + newCards - discarded);
    }
  }

  // Verify player coins remained the same
  assertTrue(testState.coins == state.coins, "Coins", testState.coins, state.coins);


  /* ******************************* TEST 3 ******************************** */
  /* Action:  Player chooses to discard hand while other players only have 4 cards in hand.
   * Expected Response:  Player discards hand, draws 4 new cards, and receives
      one additional buy.  Other players remain the same.
   * Variables:  Choice 1 = 0, choice 2 = 1
   * State Setup: Reduce cards in other players' hands to 4.
   */
  printf("\nTest 3:  Player chooses to discard hand and receive 4 cards.  All other players have 4 cards in hand.  Choice 1 = 0, choice 2 = 1.\n");

  /* Scenario setup */
  for (int i = 0; i < state.numPlayers; i++){
    if (i != thisPlayer){
      state.handCount[i] = 4;
    }
  }
  choice1 = 0;
  choice2 = 1;
  extraCoins = 0;
  newCards = 4;
  discarded = 5;

  // Copy original game state to a test state
  memcpy(&testState, &state, sizeof(struct gameState));
  cardEffect(minion, choice1, choice2, choice3, &testState, handpos, &bonus);


  // Verify number of actions increased by 1.
  assertTrue(testState.numActions == state.numActions + addActions, "Action Count",
            testState.numActions, state.numActions + addActions);

  // Verify player discarded 5 cards
  assertTrue(testState.discardCount[thisPlayer] == state.discardCount[thisPlayer] + discarded,
            "Discard Count", testState.discardCount[thisPlayer],
            state.discardCount[thisPlayer] + discarded);

  // Verify player picked up 4 cards into hand
  assertTrue(testState.handCount[thisPlayer] == state.handCount[thisPlayer] - discarded + newCards,
            "New Cards in Hand", testState.handCount[thisPlayer],
            state.handCount[thisPlayer] - discarded + newCards);

  // Verify other players did not discard anything
  for (int i = 0; i < state.numPlayers; i++){
    if (i != thisPlayer){
      assertTrue(testState.discardCount[i] == state.discardCount[i], "Other Player Discard Count",
                testState.discardCount[i], state.discardCount[i]);
    }
  }

  // Verify other players have only 4 cards in hand
  for (int i = 0; i < state.numPlayers; i++){
    if (i != thisPlayer){
      assertTrue(testState.handCount[i] == state.handCount[i], "Other Player Hand Count",
                testState.handCount[i], state.handCount[i]);
    }
  }

  // Verify player coins did not change
  assertTrue(testState.coins == state.coins, "Coins", testState.coins, state.coins);




  /* ******************************* TEST 4 ******************************** */
  /* Action:  Player chooses to discard hand and receive 4 cards when there are
      0 cards currently in their hand.
   * Expected Response:  Player receives 4 cards and one buy.
   * Variables:  Choice 1 = 0, choice 2 = 1.
   * State Setup:  Player hand count = 0
   */

  /* NOTE:  No need to verify other player hands...they are tested in previous cases */
  printf("\nTest 4:  Player chooses to discard hand and receive 4 cards.  Edge case - player has 0 cards in hand.  Other players have 5.  Choice 1 = 0, choice 2 = 1.\n");

  /* Scenario setup */
  state.handCount[thisPlayer] = 0;
  choice1 = 0;
  choice2 = 1;
  extraCoins = 0;
  newCards = 4;
  discarded = 0;

  // Copy original game state to a test state
  memcpy(&testState, &state, sizeof(struct gameState));
  cardEffect(minion, choice1, choice2, choice3, &testState, handpos, &bonus);


  // Verify number of actions increased by 1.
  assertTrue(testState.numActions == state.numActions + addActions, "Action Count",
            testState.numActions, state.numActions + addActions);

  // Verify that no cards were discarded during turn (player had 0 cards in hand).
  assertTrue(testState.discardCount[thisPlayer] == state.discardCount[thisPlayer] + discarded,
            "Discard Count", testState.discardCount[thisPlayer], state.discardCount[thisPlayer] + discarded);

  // Verify player picked up 4 cards into hand.
  assertTrue(testState.handCount[thisPlayer] == state.handCount[thisPlayer] + newCards,
            "New Cards in Hand", testState.handCount[thisPlayer],
            0 - discarded + newCards);

  // Verify player coins did not change
  assertTrue(testState.coins == state.coins, "Coins", testState.coins, state.coins);





  printf("\n################## TESTING COMPLETE:  %s ##################\n", TESTCARD);

  return 0;

}
