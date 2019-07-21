/*  File Name:    unittest4.c
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

# define TESTCARD "tribute"

/* BUSINESS REQUIREMENTS:  TRIBUTE *********************************
1)  Player after current player must reveal top 2 cards from hand
2)  Action cards are worth +2 actions
3)  Treasure cards are worth +2 coins
4)  Victory cards are worth +2 cards
5)  To get both bonuses, the cards must be different
6)  If a particular card has multiple types, the player receives
    both bonuses for that card.
********************************************************************/


int main(){
  /* Initialize testing and game variables */
  int newCards = 0;
  int revealedCards = 0;
  int extraCoins = 0;

  int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
  int seed = 1000;
  int numPlayers = 2;
  int thisPlayer = 0;
  int nextPlayer = thisPlayer + 1;

  struct gameState state, testState;
  int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
                sea_hag, tribute, smithy, council_room};

  /* Initialize game state and player cards */
  initializeGame(numPlayers, k, seed, &state);

  printf("---------------- TESTING CARD:  %s ----------------\n", TESTCARD);

  /* ******************************* TEST 1 ******************************** */
  /* Action:  Next player reveals 2 copper cards.
   * Expected Response:  Player receives 2 bonus coins.
   * Variables:  -none-
   * State Setup:  2 coppers at the top of the next player's deck
   */
  printf("\nTest 1:  Next player reveals 2 copper cards on top of his deck.  Current player receives 2 bonus coins.\n");

  /* Scenario setup */
  state.deck[nextPlayer][state.deckCount[nextPlayer] - 1] = copper;
  state.deck[nextPlayer][state.deckCount[nextPlayer] - 2] = copper;
  revealedCards = 2;
  extraCoins = 2;

  // Copy original game state to a test state
  memcpy(&testState, &state, sizeof(struct gameState));
  cardEffect(tribute, choice1, choice2, choice3, &testState, handpos, &bonus);

  // Verify next player revealed 2 cards
  assertTrue(testState.deckCount[nextPlayer] == state.deckCount[nextPlayer] - revealedCards,
            "Next Player Deck Count", testState.deckCount[nextPlayer],
            state.deckCount[nextPlayer] - revealedCards);

  // Verify next player discarded 2 cards
  assertTrue(testState.discardCount[nextPlayer] == state.discardCount[nextPlayer] + revealedCards,
            "Next Player Discard Count", testState.discardCount[nextPlayer],
            state.discardCount[nextPlayer] + revealedCards);

  // Verify current player received 2 bonus coins
  assertTrue(testState.coins == state.coins + extraCoins, "Coins", testState.coins,
            state.coins + extraCoins);

  // Verify current player did not pick up any cards
  assertTrue(testState.handCount[thisPlayer] == state.handCount[thisPlayer],
            "Player Hand Count", testState.handCount[thisPlayer], state.handCount[thisPlayer]);

  // Verify current player did not receive any additional actions
  assertTrue(testState.numActions == state.numActions, "Player Action Count",
            testState.numActions, state.numActions);



  /* ******************************* TEST 2 ******************************** */
  /* Action:  Next player reveals 1 copper and 1 estate.
   * Expected Response:  Player receives 2 bonus coins and 2 cards from deck.
   * Variables:  -none-
   * State Setup:  Place estate 2nd from top of next player's deck
   */
  printf("\nTest 2:  Next player reveals 1 copper and 1 estate.  Current player receives 2 bonus coins and 2 additional cards from deck.\n");

  /* Scenario setup */
  state.deck[nextPlayer][state.deckCount[nextPlayer] - 2] = estate;
  newCards = 2;
  revealedCards = 2;
  extraCoins = 2;

  // Copy original game state to a test state
  memcpy(&testState, &state, sizeof(struct gameState));
  cardEffect(tribute, choice1, choice2, choice3, &testState, handpos, &bonus);


  // Verify next player revealed 2 cards
  assertTrue(testState.deckCount[nextPlayer] == state.deckCount[nextPlayer] - revealedCards,
            "Next Player Deck Count", testState.deckCount[nextPlayer],
            state.deckCount[nextPlayer] - revealedCards);

  // Verify next player discarded 2 cards
  assertTrue(testState.discardCount[nextPlayer] == state.discardCount[nextPlayer] + revealedCards,
            "Next Player Discard Count", testState.discardCount[nextPlayer],
            state.discardCount[nextPlayer] + revealedCards);

  // Verify current player received 2 bonus coins
  assertTrue(testState.coins == state.coins + extraCoins, "Coins", testState.coins,
            state.coins + extraCoins);

  // Verify current player drew 2 additional cards
  assertTrue(testState.handCount[thisPlayer] == state.handCount[thisPlayer] + newCards,
          "Player Hand Count", testState.handCount[thisPlayer],
          state.handCount[thisPlayer] + newCards);

  // Verify current player did not receive any additional actions
  assertTrue(testState.numActions == state.numActions, "Player Action Count",
            testState.numActions, state.numActions);



  /* ******************************* TEST 3 ******************************** */
  /* Action:  Next player reveals 1 copper and 1 tribute.
   * Expected Response:  Player receives 2 bonus coins and 2 actions
   * Variables: -none-
   * State Setup:  Place tribute (action) 2nd from top of next player's deck
   */
  printf("\nTest 3:  Next player reveals 1 copper and 1 tribute.  Current player receives 2 bonus coins and 2 additional actions.\n");

  /* Scenario setup */
  state.deck[nextPlayer][state.deckCount[nextPlayer] - 2] = tribute;
  revealedCards = 2;
  extraCoins = 2;

  // Copy original game state to a test state
  memcpy(&testState, &state, sizeof(struct gameState));
  cardEffect(tribute, choice1, choice2, choice3, &testState, handpos, &bonus);


  // Verify next player revealed 2 cards
  assertTrue(testState.deckCount[nextPlayer] == state.deckCount[nextPlayer] - revealedCards,
            "Next Player Deck Count", testState.deckCount[nextPlayer],
            state.deckCount[nextPlayer] - revealedCards);

  // Verify next player discarded 2 cards
  assertTrue(testState.discardCount[nextPlayer] == state.discardCount[nextPlayer] + revealedCards,
            "Next Player Discard Count", testState.discardCount[nextPlayer],
            state.discardCount[nextPlayer] + revealedCards);

  // Verify current player received 2 bonus coins
  assertTrue(testState.coins == state.coins + extraCoins, "Coins", testState.coins,
            state.coins + extraCoins);

  // Verify current player did not pick up any new cards
  assertTrue(testState.handCount[thisPlayer] == state.handCount[thisPlayer],
          "Player Hand Count", testState.handCount[thisPlayer],
          state.handCount[thisPlayer]);

  // Verify current player received 2 additional actions
  assertTrue(testState.numActions == state.numActions + 2, "Player Action Count",
            testState.numActions, state.numActions + 2);


  /* ******************************* TEST 4 ******************************** */
  /* Action:  Next player only has 1 card to reveal (copper).
   * Expected Response:  Player receives 2 bonus coins.
   * Variables:  -none-
   * State Setup:  Change next player's deck count to 1 and place a copper there.
   */
  printf("\nTest 4:  Next player only has 1 copper to reveal.  Current player receives 2 bonus coins.\n");

  /* Scenario setup */
  state.deckCount[nextPlayer] = 1;
  state.deck[nextPlayer][state.deckCount[nextPlayer] - 1] = copper;
  revealedCards = 1;
  extraCoins = 2;

  // Copy original game state to a test state
  memcpy(&testState, &state, sizeof(struct gameState));
  cardEffect(tribute, choice1, choice2, choice3, &testState, handpos, &bonus);

  // Verify next player revealed 1 card
  assertTrue(testState.deckCount[nextPlayer] == state.deckCount[nextPlayer] - revealedCards,
            "Next Player Deck Count", testState.deckCount[nextPlayer],
            state.deckCount[nextPlayer] - revealedCards);

  // Verify next player discarded 1 card
  assertTrue(testState.discardCount[nextPlayer] == state.discardCount[nextPlayer] + revealedCards,
            "Next Player Discard Count", testState.discardCount[nextPlayer],
            state.discardCount[nextPlayer] + revealedCards);

  // Verify current player received 2 bonus coins
  assertTrue(testState.coins == state.coins + extraCoins, "Coins", testState.coins,
            state.coins + extraCoins);

  // Verify current player did not pick up additional cards
  assertTrue(testState.handCount[thisPlayer] == state.handCount[thisPlayer],
          "Player Hand Count", testState.handCount[thisPlayer],
          state.handCount[thisPlayer]);

  // Verify current player did not receive any additional actions
  assertTrue(testState.numActions == state.numActions, "Player Action Count",
            testState.numActions, state.numActions);


  /* ******************************* TEST 5 ******************************** */
  /* Action:  Next player has no cards to reveal.
   * Expected Response:  No change in state
   * Variables:  -none-
   * State Setup:  Change next player's deck count to 0.
   */
  // Next player has no cards to reveal.  Nothing happens.
  printf("\nTest 5:  Next player has no cards to reveal.  Nothing happens.\n");

  /* Scenario setup */
  state.deckCount[nextPlayer] = 0;    // Discard pile is already 0
  revealedCards = 0;
  extraCoins = 0;
  newCards = 0;

  // Copy original game state to a test state
  memcpy(&testState, &state, sizeof(struct gameState));
  cardEffect(tribute, choice1, choice2, choice3, &testState, handpos, &bonus);

  // Verify next player revealed 0 cards
  assertTrue(testState.deckCount[nextPlayer] == state.deckCount[nextPlayer] - revealedCards,
            "Next Player Deck Count", testState.deckCount[nextPlayer],
            state.deckCount[nextPlayer] - revealedCards);

  // Verify next player discarded 0 cards
  assertTrue(testState.discardCount[nextPlayer] == state.discardCount[nextPlayer] + revealedCards,
            "Next Player Discard Count", testState.discardCount[nextPlayer],
            state.discardCount[nextPlayer] + revealedCards);

  // Verify current player received 0 bonus coins
  assertTrue(testState.coins == state.coins + extraCoins, "Coins", testState.coins,
            state.coins + extraCoins);

  // Verify current player did not take additional cards
  assertTrue(testState.handCount[thisPlayer] == state.handCount[thisPlayer] + newCards,
          "Player Hand Count", testState.handCount[thisPlayer],
          state.handCount[thisPlayer] + newCards);

  // Verify current player did not receive any additional actions
  assertTrue(testState.numActions == state.numActions, "Player Action Count",
            testState.numActions, state.numActions);



  printf("\n################## TESTING COMPLETE:  %s ##################\n", TESTCARD);

  return 0;

}
