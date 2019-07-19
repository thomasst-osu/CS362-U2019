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
  int newCards = 0, discarded = 0, extraCoins = 0;    // test variables

  struct gameState G, testG;
  int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
                sea_hag, tribute, smithy, council_room};

  /* Initialize game state and player cards */
  initializeGame(numPlayers, k, seed, &G);

  printf("---------------- TESTING CARD:  %s ----------------\n", TESTCARD);

  /* ******************************* TEST 1 ******************************** */
  // Test 1:  Choice 1 = 1.  Player chooses to receive +2 coins.
  printf("Test 1:  Choice 1 = 1.  Player chooses to receive +2 coins.\n");
  choice1 = 1;
  extraCoins = 2;

  // Copy original game state to a test state
  memcpy(&testState, &state, sizeof(struct gameState));
  cardEffect(baron, choice1, choice2, choice3, &testState, handpos, &bonus);


  // Verify number of actions increased by 1.
  assertTrue(testState.numActions == state.numActions + 1, "Action Count",
            testState.numActions, state.numActions + 1);

  // Verify the player did not discard anything
  assertTrue(testState.discardCount[thisPlayer] == state.discardCount[thisPlayer],
            "Discard Count", testState.discardCount[thisPlayer],
            state.discardCount[thisPlayer]);

  // Verify the player received two extra coins
  assertTrue(testState.coins == state.coins + extraCoins, "Coins",
            testState.coins, state.coins + extraCoins);

  // Verify the other players' hands did not change
  for (int i = 0; i < state.numPlayers; i++){
    if (i != currentPlayer){
      assertTrue(testState.discardCount[i] == state.discardCount[i], "Other Player Discard Count",
                testState.discardCount[i], state.discardCount[i]);
    }
  }


  /* ******************************* TEST 2 ******************************** */
  // Test 2:  Choice 1 = 0, choice 2 = 1.  Player chooses to discard hand and receive 4 cards.
  // All other players have 5 cards in their hand and have to discard and redraw 4.
  printf("Test 2:  Choice 1 = 0, choice 2 = 1.  \
        Player chooses to discard hand and receive 4 cards.  \
        All players start with 5 cards.\n");
  choice1 = 0;
  choice2 = 1;
  extraCoins = 0;
  newCards = 4;
  discarded = 5;

  // Copy original game state to a test state
  memcpy(&testState, &state, sizeof(struct gameState));
  cardEffect(baron, choice1, choice2, choice3, &testState, handpos, &bonus);

  // Verify number of actions increased by 1.
  assertTrue(testState.numActions == state.numActions + 1, "Action Count",
            testState.numActions, state.numActions + 1);

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
  // Test 3:  Choice 1 = 0, choice 2 = 1.  Player chooses to discard hand and receive 4 cards.
  // All other players have 4 cards in hand.
  printf("Test 3:  Choice 1 = 0, choice 2 = 1.  Player chooses to discard hand and receive 4 cards.  \
          All other players have 4 cards in hand.\n");
  choice1 = 0;
  choice2 = 1;
  extraCoins = 0;
  newCards = 4;
  discarded = 5;

  // Copy original game state to a test state
  memcpy(&testState, &state, sizeof(struct gameState));

  // Ensure current player has 5 cards and the rest have 4.
  for (int i = 0; i < testState.numPlayers; i++){
    if (i != thisPlayer){
      testState.handCount[i]--;   // all players start with 5 cards in hand
    }
  }

  cardEffect(baron, choice1, choice2, choice3, &testState, handpos, &bonus);


  // Verify number of actions increased by 1.
  assertTrue(testState.numActions == state.numActions + 1, "Action Count",
            testState.numActions, state.numActions + 1);

  // Verify player discarded 5 cards
  assertTrue(testState.discardCount[thisPlayer] == state.discardCount + discarded,
            "Discard Count", testState.discardCount[thisPlayer],
            state.discardCount + discarded);

  // Verify player picked up 4 cards into hand
  assertTrue(testState.handCount[thisPlayer] = state.handCount[thisPlayer] - discarded + newCards,
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
      assertTrue(testState.handCount[i] == state.handCount[i] - 1, "Other Player Hand Count",
                testState.handCount[i], state.handCount[i] - 1);
    }
  }

  // Verify player coins did not change
  assertTrue(testState.coins == state.coins, "Coins", testState.coins, state.coins);




  /* ******************************* TEST 4 ******************************** */
  // Test 4:  Choice 1 = 0, choice 2 = 1.  Player chooses to discard hand and receive 4 cards.
  // Edge case - player has 0 cards in hand.  Other players have 5.

  /* NOTE:  No need to verify other player hands...they are tested in previous cases */
  printf("Test 4:  Choice 1 = 0, choice 2 = 1.  Player chooses to discard hand and receive 4 cards.  \
          Edge case - player has 0 cards in hand.  Other players have 5.\n");
  choice1 = 0;
  choice2 = 1;
  extraCoins = 0;
  newCards = 4;
  discarded = 0;

  // Copy original game state to a test state
  memcpy(&testState, &state, sizeof(struct gameState));

  // Ensure current player has 0 cards in hand.
  testState.handCount[thisPlayer] = 0;

  cardEffect(baron, choice1, choice2, choice3, &testState, handpos, &bonus);


  // Verify number of actions increased by 1.
  assertTrue(testState.numActions == state.numActions + 1, "Action Count",
            testState.numActions, state.numActions + 1);

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
