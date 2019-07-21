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
  int discarded = 0, newCards = 0, trashed = 0, addBuys = 0, extraCoins = 0;

  struct gameState state, testState;
  int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
                sea_hag, tribute, smithy, council_room};

  /* Initialize game state and player cards */
  initializeGame(numPlayers, k, seed, &state);

  printf("---------------- TESTING CARD:  %s ----------------\n", TESTCARD);


  /* ******************************* TEST 1 ******************************** */
  /* Action:  Player chooses to draw an estate card
   * Expected Response:  Player picks up estate card to discard pile, discards baron,
      and gets an additional buy.
   * Variables:  Choice 1 = 0.
   * State Setup:  -none...leave as defined by initializeGame-
   */
  printf("\nTest 1:  Player elects to draw an estate card regardless of hand content.  Choice 1 = 0.\n");
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
  /* Action: Player attempts to gain +4 coins while NOT having an estate card.
   * Expected Response:  No extra coins, player picks up an estate, discards baron,
      and gets an additional buy.
   * Variables:  Choice 1 = 1
   * State Setup:  Player hand [0 - 4] = copper to ensure no estates in hand.
   */
  printf("\nTest 2:  Player elects to gain +4 coins while NOT having an estate card.  Choice 1 = 1.\n");
  choice1 = 1;
  newCards = 1;
  discarded = 1;
  addBuys = 1;

  /* Scenario setup */
  for (int i = 0; i < state.handCount[thisPlayer]; i++){
    state.hand[thisPlayer][i] = copper;
  }

  // Copy original game state to a test state
  memcpy(&testState, &state, sizeof(struct gameState));
  cardEffect(baron, choice1, choice2, choice3, &testState, handpos, &bonus);

  // Verify that an estate card was gained (in discard pile) because the player did not
  // have one in their hand.
  assertTrue(testState.discard[thisPlayer][testState.discardCount[thisPlayer] - 1] == estate,
            "New Estate Card", testState.discard[thisPlayer][testState.discardCount[thisPlayer] - 1],
            estate);

  // Verify that only one card was gained (to discard pile)
  assertTrue(testState.discardCount[thisPlayer] == state.discardCount[thisPlayer] + discarded,
            "Discard Count", testState.discardCount[thisPlayer], state.discardCount[thisPlayer] + discarded);

  // Verify that the state supply count has decreased by 1.
  assertTrue(testState.supplyCount[estate] == state.supplyCount[estate] - newCards,
            "Estate Supply", testState.supplyCount[estate], state.supplyCount[estate] - newCards);

  // Verify that the number of coins available hasn't changed
  assertTrue(testState.coins == state.coins, "Coins", testState.coins, state.coins);

  // Verify number of buys increased by 1
  assertTrue(testState.numBuys == state.numBuys + addBuys, "Number of Buys", testState.numBuys, state.numBuys + 1);



  /* ******************************* TEST 3 ******************************** */
  /* Action: Player discards an estate for +4 coins
   * Expected Response:  Player discards estate, discards baron, receives +4 coins
      and +1 buy.
   * Variables:  Choice 1 = 1
   * State Setup:  Player hand [0] = estate, [1 - 4] = copper
   */
  // Choice 1 = 1.  Player elects to gain +4 coins while having an estate card.
  printf("\nTest 3:  Player elects to gain +4 coins while having an estate card.  Choice 1 = 1.\n");

  /* Scenario setup */
  state.hand[thisPlayer][0] = estate;
  choice1 = 1;
  discarded = 2;
  newCards = 0;
  extraCoins = 4;
  addBuys = 1;


  // Copy original game state to a test state
  memcpy(&testState, &state, sizeof(struct gameState));
  cardEffect(baron, choice1, choice2, choice3, &testState, handpos, &bonus);

  // Verify that an estate card was discarded.
  int discardedEstate = 0;
  for (int i = 0; i < testState.discardCount[thisPlayer]; i++){
    if (testState.discard[thisPlayer][i] == estate){
      discardedEstate++;
    }
  }
  assertTrue(discardedEstate == 1, "Discarded Estate Card", discardedEstate, 1);

  // Verify that the estate and baron card were discarded
  assertTrue(testState.discardCount[thisPlayer] == state.discardCount[thisPlayer] + discarded,
            "Discard Count", testState.discardCount[thisPlayer], state.discardCount[thisPlayer] + discarded);

  // Verify that the supply count was not modified for estates.
  assertTrue(testState.supplyCount[estate] == state.supplyCount[estate],
            "Estate Supply", testState.supplyCount[estate], state.supplyCount[estate]);

  // Verify that the player received 4 extra coins.
  assertTrue(testState.coins == state.coins + extraCoins, "Coins", testState.coins, state.coins + extraCoins);

  // Verify that the card count in the player's hand decreased by 2.
  assertTrue(testState.handCount[thisPlayer] == state.handCount[thisPlayer] - discarded,
            "Cards in Player's Hand", testState.handCount[thisPlayer],
            state.handCount[thisPlayer] - discarded);

  // Verify number of buys increased by 1
  assertTrue(testState.numBuys == state.numBuys + addBuys, "Number of Buys", testState.numBuys, state.numBuys + addBuys);




  /* ******************************* TEST 4 ******************************** */
  /* Action:  Player attempts to draw an estate card when the estate pile is empty.
   * Expected Response:  Player discards baron, receives extra buy.
   * Variables:  Choice 1 = 0
   * State Setup:  Estate supply count = 0
   */
  // Choice 1 = 0.  Player elects to draw an estate card when estate pile is empty.
  printf("\nTest 4:  Player elects to draw an estate card when estate pile is empty.  Choice 1 = 0.\n");

  /* Scenario Setup */
  state.supplyCount[estate] = 0;
  choice1 = 0;
  newCards = 0;
  discarded = 1;
  addBuys = 1;

  // Copy original game state to a test state
  memcpy(&testState, &state, sizeof(struct gameState));
  cardEffect(baron, choice1, choice2, choice3, &testState, handpos, &bonus);

  // Verify that no estate card was gained (into discard pile)
  int estateCount = 0;
  for (int i = 0; i < testState.discardCount[thisPlayer]; i++){
    if (testState.discard[thisPlayer][i] == estate){
      estateCount++;
    }
  }

  assertTrue(estateCount == newCards, "Discarded Estate Card", estateCount, newCards);

  // Verify the estate supply
  assertTrue(testState.supplyCount[estate] == 0, "Estate Supply", testState.supplyCount[estate], 0);

  // Verify the player's hand count
  assertTrue(testState.handCount[thisPlayer] == state.handCount[thisPlayer] - discarded,
            "Cards in Player's Hand", testState.handCount[thisPlayer],
            state.handCount[thisPlayer] - discarded);

  // Verify the player's coin count
  assertTrue(testState.coins == state.coins, "Coins", testState.coins, state.coins);

  // Verify number of buys increased by 1
  assertTrue(testState.numBuys == state.numBuys + addBuys, "Number of Buys", testState.numBuys, state.numBuys + addBuys);



  printf("\n################## %s TESTING COMPLETE ##################\n", TESTCARD);

  return 0;

}
