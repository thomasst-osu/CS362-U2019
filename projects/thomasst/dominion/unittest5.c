/*  File Name:    unittest5.c
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

# define TESTCARD "mine"

/* BUSINESS REQUIREMENTS:  MINE ************************************
1)  Player must have a treasure card in order to gain benefit from mine
2)  Player trashes (not discards) a treasure card from their hand
3)  Player gains a treasure of their choosing costing up to +3 more
    than the one trashed directly into their hand.
4)  If no treasure is available, this card does nothing.
********************************************************************/


int main(){
  /* Initialize testing and game variables */
  int newCards = 0;
  int discarded = 1;
  int trashed = 0;
  int xtraCoins = 0;
  int shuffleCards = 0;

  int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
  int seed = 1000;
  int numPlayers = 2;
  int thisPlayer = 0;

  struct gameState state, testState;
  int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
                sea_hag, tribute, smithy, council_room};

  /* Initialize game state and player cards */
  initializeGame(numPlayers, k, seed, &G);

  printf("---------------- TESTING CARD:  %s ----------------\n", TESTCARD);


  /* ******************************* TEST 1 ******************************** */
  /* Action:  Player chooses to trash a copper for a silver.
   * Expected Response:  Copper trashed, silver in hand, mine in discard
   * Variables:  choice 1 = 1 (copper), choice 2 = silver
   * State Setup: Player hand [0] = mine, [1 - 4] = copper.
   */
  // Player chooses to trash a copper for a silver
  printf("\nPlayer chooses to trash a copper for a silver.  Choice 1 = 1, choice 2 = silver.\n");

  /* Modify base state for scenario */
  state.hand[thisPlayer][0] = mine;
  state.hand[thisPlayer][1] = copper;
  state.hand[thisPlayer][2] = copper;
  state.hand[thisPlayer][3] = copper;
  state.hand[thisPlayer][4] = copper;

  choice1 = 1;
  choice2 = silver;
  newCards = 1;
  discarded = 1;
  trashed = 1;

  // Copy original game state to a test state
  memcpy(&testState, &state, sizeof(struct gameState));
  cardEffect(mine, choice1, choice2, choice3, &testState, handpos, &bonus);

  // Verify the number of cards in hand (5 - copper - mine + silver = 4)
  assertTrue(testState.handCount[thisPlayer] == state.handCount[thisPlayer] - discarded - trashed + newCards,
            "Player Hand Count", testState.handCount[thisPlayer],
            state.handCount[thisPlayer] - discarded - trashed + newCards);

  // Verify the contents of player's hand (1 silver, 3 copper)
  int copperCount = 0, silverCount = 0;
  int testCopperCount = 0, testSilverCount = 0;
  for (int i = 0; i < state.handCount[thisPlayer]; i++){
    if (state.hand[thisPlayer][i] == copper){
      copperCount++;
    } else if (state.hand[thisPlayer][i] == silver){
      silverCount++;
    }
  }

  for (int i = 0; i < testState.handCount[thisPlayer]; i++){
    if (testState.hand[thisPlayer][i] == copper){
      testCopperCount++;
    } else if (testState.hand[thisPlayer][i] == silver){
      testSilverCount++;
    }
  }

  assertTrue(testCopperCount == copperCount - trashed, "Player Copper in Hand", testCopperCount, copperCount - trashed);

  // Verify played card in discard (1 mine)
  assertTrue(testState.discardCount[thisPlayer] = state.discardCount[thisPlayer] + discarded,
            "Player Discard Count", testState.discardCount[thisPlayer], state.discardCount[thisPlayer] + discarded);

  // Verify played card is a mine
  if (testState.discardCount[thisPlayer] == 1){
    assertTrue(testState.discard[thisPlayer][testState.discardCount[thisPlayer] - 1] == mine,
              "Mine in Player Discard", testState.discard[thisPlayer][testState.discardCount[thisPlayer] - 1],
              mine);
  }



  /* ******************************* TEST 2 ******************************** */
  /* Action: Player chooses to trash a copper for a gold
   * Expected Response:  No change in state.  Action not allowed.
   * Variables:  choice 1 = 1 (copper), choice 2 = gold.
   * State Setup:  Player hand [0] = mine, [1 - 4] = copper
   */
  printf("\nPlayer tries to trash copper for a gold.  Choice 1 = 1, choice 2 = gold.\n");

  /* Scenario setup */
  // -- no changes to state needed from previous test
  choice1 = 1;
  choice2 = gold;
  discarded = 0;

  // Copy original game state to a test state
  memcpy(&testState, &state, sizeof(struct gameState));
  cardEffect(mine, choice1, choice2, choice3, &testState, handpos, &bonus);

  // Verify the number of cards in hand (5 - no cards gained/discarded)
  assertTrue(testState.handCount[thisPlayer] = state.handCount[thisPlayer], "Player Hand Count",
            testState.handCount[thisPlayer], state.handCount[thisPlayer]);

  // Verify the contents of player's hand (1 mine, 4 copper)
  copperCount = 0;
  testCopperCount = 0;
  int mineCount, testMineCount;

  for (int i = 0; i < state.handCount[thisPlayer]; i++){
    if (state.hand[thisPlayer][i] == copper){
      copperCount++;
    } else if (state.hand[thisPlayer][i] == mine){
      mineCount++;
    }
  }

  for (int i = 0; i < testState.handCount[thisPlayer]; i++){
    if (testState.hand[thisPlayer][i] == copper){
      testCopperCount++;
    } else if (testState.hand[thisPlayer][i] == mine){
      testMineCount++;
    }
  }
  assertTrue(testCopperCount == copperCount, "Player Copper in Hand", testCopperCount, copperCount);
  assertTrue(testMineCount == mineCount, "Player Mine in Hand", testMineCount, mineCount);

  // Verify no cards in discard
  assertTrue(testState.discardCount[thisPlayer] == state.discardCount[thisPlayer] + discarded,
            "Player Discard Count", testState.discardCount[thisPlayer],
            state.discardCount[thisPlayer] + discarded);



  /* ******************************* TEST 3 ******************************** */
  /* Action: Player tries to trade a copper for an action card
   * Expected Response:  No change in state.  Action not allowed.
   * Variables:  choice 1 = 1 (copper), choice 2 = adventurer
   * State Setup:  Player hand [0] = mine, [1 - 4] = copper
   */
  printf("\nPlayer tries to trade a copper for an action card (adventurer).  \
            Choice 1 = 1, choice 2 = adventurer.\n");

  /* Scenario setup */
  // -- no changes to state needed from previous state
  choice1 = 1;
  choice2 = adventurer;

  // Copy original game state to a test state
  memcpy(&testState, &state, sizeof(struct gameState));
  cardEffect(mine, choice1, choice2, choice3, &testState, handpos, &bonus);


  // Verify the number of cards in hand (5 - no cards gained/discarded)
  assertTrue(testState.handCount[thisPlayer] = state.handCount[thisPlayer], "Player Hand Count",
            testState.handCount[thisPlayer], state.handCount[thisPlayer]);

  // Verify the contents of player's hand (1 mine, 4 copper)
  copperCount = 0;
  testCopperCount = 0;
  int mineCount, testMineCount;

  for (int i = 0; i < state.handCount[thisPlayer]; i++){
    if (state.hand[thisPlayer][i] == copper){
      copperCount++;
    } else if (state.hand[thisPlayer][i] == mine){
      mineCount++;
    }
  }

  for (int i = 0; i < testState.handCount[thisPlayer]; i++){
    if (testState.hand[thisPlayer][i] == copper){
      testCopperCount++;
    } else if (testState.hand[thisPlayer][i] == mine){
      testMineCount++;
    }
  }
  assertTrue(testCopperCount == copperCount, "Player Copper in Hand", testCopperCount, copperCount);
  assertTrue(testMineCount == mineCount, "Player Mine in Hand", testMineCount, mineCount);

  // Verify no cards in discard
  assertTrue(testState.discardCount[thisPlayer] == state.discardCount[thisPlayer] + discarded,
            "Player Discard Count", testState.discardCount[thisPlayer],
            state.discardCount[thisPlayer] + discarded);



  /* ******************************* TEST 4 ******************************** */
  /* Action: Player tries to trade a non-treasure card for a silver
   * Expected Response:  No change in state.  Action not allowed.
   * Variables: choice 1 = 1 (province), choice 2 = silver
   * State Setup:  Player hand [0] = mine, [1] = province, [2 - 4] = copper
   */
  printf("\nPlayer tries to trade a non-treasure card for a silver.  \
            Choice 1 = 1 (province), choice 2 = silver.\n");

  /* Scenario setup */
  state.hand[thisPlayer][1] = province;

  choice1 = 1;
  choice2 = silver;

  // Copy original game state to a test state
  memcpy(&testState, &state, sizeof(struct gameState));
  cardEffect(mine, choice1, choice2, choice3, &testState, handpos, &bonus);

  // Verify the number of cards in hand (5 - no cards gained/discarded)
  assertTrue(testState.handCount[thisPlayer] = state.handCount[thisPlayer], "Player Hand Count",
            testState.handCount[thisPlayer], state.handCount[thisPlayer]);

  // Verify the contents of player's hand (1 mine, 1 province, 3 copper)
  copperCount = 0;  testCopperCount = 0;
  mineCount = 0;  testMineCount = 0;
  int provinceCount = 0, testProvinceCount = 0;

  for (int i = 0; i < state.handCount[thisPlayer]; i++){
    if (state.hand[thisPlayer][i] == copper){
      copperCount++;
    } else if (state.hand[thisPlayer][i] == mine){
      mineCount++;
    } else if (state.hand[thisPlayer][i] == province){
      provinceCount++;
    }
  }

  for (int i = 0; i < testState.handCount[thisPlayer]; i++){
    if (testState.hand[thisPlayer][i] == copper){
      testCopperCount++;
    } else if (testState.hand[thisPlayer][i] == mine){
      testMineCount++;
    } else if (testState.hand[thisPlayer][i] == province){
      testProvinceCount++;
    }
  }
  assertTrue(testCopperCount == copperCount, "Player Copper in Hand", testCopperCount, copperCount);
  assertTrue(testMineCount == mineCount, "Player Mine in Hand", testMineCount, mineCount);
  assertTrue(testProvinceCount == provinceCount, "Player Province in Hand", testProvinceCount, provinceCount);

  // Verify no cards in discard
  assertTrue(testState.discardCount[thisPlayer] == state.discardCount[thisPlayer] + discarded,
            "Player Discard Count", testState.discardCount[thisPlayer],
            state.discardCount[thisPlayer] + discarded);



  printf("################## TESTING COMPLETE:  %s ##################\n", TESTCARD);

  return 0;

}
