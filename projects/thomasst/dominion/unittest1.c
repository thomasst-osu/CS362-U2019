/*  File Name:    unittest1.c
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

# define TESTCARD "baron"

/* BUSINESS REQUIREMENTS:  BARON ***********************************
1)  If no estates are currently in hand, player draws an estate
2)  If player has an estate, they can discard in exchange for +4 coins
3)  Player can elect to draw an estate instead of +4 coins and retain
    current estate in hand.
4)  Player receives an extra buy.
********************************************************************/


int main(){
  /* Initialize testing and game variables */
  int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
  int seed = 1000;
  int numPlayers = 2;
  int thisPlayer = 0;

  struct gameState state, testState;
  int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
                sea_hag, tribute, smithy, council_room};

  /* Initialize game state and player cards */
  initializeGame(numPlayers, k, seed, &state);

  printf("---------------- TESTING CARD:  %s ----------------\n", TESTCARD);


  /* ******************************* TEST 1 ******************************** */
  // Choice 1 = 0.  Player elects to draw an estate card regardless of hand content.
  printf("\nTest 1:  Choice 1 = 0.  Player elects to draw an estate card regardless of hand content.\n");
  choice1 = 0;

  // Copy original game state to a test state
  memcpy(&testState, &state, sizeof(struct gameState));
  cardEffect(baron, choice1, choice2, choice3, &testState, handpos, &bonus);

  // Verify that an estate card is in the discard pile
  assertTrue(testState.discard[thisPlayer][testState.discardCount[thisPlayer] - 1] == estate,
            "New Estate Card", testState.discard[thisPlayer][testState.discardCount[thisPlayer] - 1],
            estate);

  // Verify that only one card was gained (to discard pile)
  assertTrue(testState.discardCount[thisPlayer] == state.discardCount[thisPlayer] + 1,
            "Discard Count", testState.discardCount[thisPlayer], state.discardCount[thisPlayer] + 1);

  // Verify that the estate supply count has decreased by 1
  assertTrue(testState.supplyCount[estate] == state.supplyCount[estate] - 1,
            "Estate Supply", testState.supplyCount[estate], state.supplyCount[estate] - 1);

  // Verify that the number of coins available hasn't changed
  assertTrue(testState.coins == state.coins, "Coins", testState.coins, state.coins);

  // Verify number of buys increased by 1
  assertTrue(testState.numBuys == state.numBuys + 1, "Number of Buys", testState.numBuys, state.numBuys + 1);



  /* ******************************* TEST 2 ******************************** */
  // Choice 1 = 1.  Player elects to gain +4 coins while NOT having an estate card.
  printf("\nTest 2:  Choice 1 = 1.  Player elects to gain +4 coins while NOT having an estate card.\n");
  choice1 = 1;

  // Copy original game state to a test state
  memcpy(&testState, &state, sizeof(struct gameState));

  // Ensure that the current player does not have any estates in their hand
  for (int i = 0; i < 10; i++){
    testState.hand[thisPlayer][i] = copper;
  }

  cardEffect(baron, choice1, choice2, choice3, &testState, handpos, &bonus);

  // Verify that an estate card was gained (in discard pile) because the player did not
  // have one in their hand.
  assertTrue(testState.discard[thisPlayer][testState.discardCount[thisPlayer] - 1] == estate,
            "New Estate Card", testState.discard[thisPlayer][testState.discardCount[thisPlayer] - 1],
            estate);

  // Verify that only one card was gained (to discard pile)
  assertTrue(testState.discardCount[thisPlayer] == state.discardCount[thisPlayer] + 1,
            "Discard Count", testState.discardCount[thisPlayer], state.discardCount[thisPlayer] + 1);

  // Verify that the state supply count has decreased by 1.
  assertTrue(testState.supplyCount[estate] == state.supplyCount[estate] - 1,
            "Estate Supply", testState.supplyCount[estate], state.supplyCount[estate] - 1);

  // Verify that the number of coins available hasn't changed
  assertTrue(testState.coins == state.coins, "Coins", testState.coins, state.coins);

  // Verify number of buys increased by 1
  assertTrue(testState.numBuys == state.numBuys + 1, "Number of Buys", testState.numBuys, state.numBuys + 1);



  /* ******************************* TEST 3 ******************************** */
  // Choice 1 = 1.  Player elects to gain +4 coins while having an estate card.
  printf("\nTest 3:  Choice 1 = 1.  Player elects to gain +4 coins while having an estate card.\n");
  choice1 = 1;

  // Copy original game state to a test state
  memcpy(&testState, &state, sizeof(struct gameState));

  // Ensure that the player has at least one estate card in their hand.
  testState.hand[thisPlayer][0] = estate;

  cardEffect(baron, choice1, choice2, choice3, &testState, handpos, &bonus);

  // Verify that an estate card was discarded.
  assertTrue(testState.discard[thisPlayer][testState.discardCount[thisPlayer] - 1] == estate,
            "Discarded Estate Card", testState.discard[thisPlayer][testState.discardCount[thisPlayer] - 1],
            estate);

  // Verify that only one card was discarded.
  assertTrue(testState.discardCount[thisPlayer] == state.discardCount[thisPlayer] + 1,
            "Discard Count", testState.discardCount[thisPlayer], state.discardCount[thisPlayer] + 1);

  // Verify that the supply count was not modified for estates.
  assertTrue(testState.supplyCount[estate] == state.supplyCount[estate],
            "Estate Supply", testState.supplyCount[estate], state.supplyCount[estate]);

  // Verify that the player received 4 extra coins.
  assertTrue(testState.coins == state.coins + 4, "Coins", testState.coins, state.coins + 4);

  // Verify that the card count in the player's hand decreased by 1.
  assertTrue(testState.handCount[thisPlayer] == state.handCount[thisPlayer] - 1,
            "Cards in Player's Hand", testState.handCount[thisPlayer],
            state.handCount[thisPlayer] - 1);

  // Verify number of buys increased by 1
  assertTrue(testState.numBuys == state.numBuys + 1, "Number of Buys", testState.numBuys, state.numBuys + 1);




  /* ******************************* TEST 4 ******************************** */
  // Choice 1 = 0.  Player elects to draw an estate card when estate pile is empty.
  printf("\nTest 4:  Choice 1 = 0.  Player elects to draw an estate card when estate pile is empty.\n");
  choice1 = 0;

  // Copy original game state to a test state
  memcpy(&testState, &state, sizeof(struct gameState));

  // Ensure that there are no more estates in the supply deck.
  testState.supplyCount[estate] = 0;

  cardEffect(baron, choice1, choice2, choice3, &testState, handpos, &bonus);

  // Verify that no estate card was gained (into discard pile)
  assertTrue(testState.discard[thisPlayer][testState.discardCount[thisPlayer] - 1] != estate,
            "Discarded Estate Card", testState.discard[thisPlayer][testState.discardCount[thisPlayer] - 1],
            0);

  // Verify the estate supply
  assertTrue(testState.supplyCount[estate] == 0, "Estate Supply", testState.supplyCount[estate], 0);

  // Verify the player's hand count
  assertTrue(testState.handCount[thisPlayer] == state.handCount[thisPlayer],
            "Cards in Player's Hand", testState.handCount[thisPlayer],
            state.handCount[thisPlayer]);

  // Verify the player's coin count
  assertTrue(testState.coins == state.coins, "Coins", testState.coins, state.coins);

  // Verify number of buys increased by 1
  assertTrue(testState.numBuys == state.numBuys + 1, "Number of Buys", testState.numBuys, state.numBuys + 1);



  printf("\n################## %s TESTING COMPLETE ##################\n", TESTCARD);

  return 0;

}
