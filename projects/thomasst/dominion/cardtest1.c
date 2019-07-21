/*  File Name:    cardtest1.c
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

# define TESTCARD "initializeGame"

/* BUSINESS REQUIREMENTS:  initializeGame **************************
1)  Sets up random number generator
2)  Validates number of players before proceeding
3)  Validates kingdom card requests are all different cards
4)  Sets supply counts of various cards based on player count
5)  Initializes each player's hand with the appropriate number of
    estates and coppers.
6)  Shuffles every player's deck
7)  Initializes various state variables for the beginning of the game
8)  Draws cards and updates coins for the first player
********************************************************************/

int main(){
  /* Initialize testing and game variables */
  int numPlayers = 1;
  int seed = 1000;
  int k[10] = {adventurer, gardens, village, minion, mine, cutpurse,
                sea_hag, tribute, smithy, council_room};


  printf("---------------- TESTING FUNCTION:  %s ----------------\n", TESTCARD);

  /* ******************************* TEST 1 ******************************** */
  /* Action:  Try to initialize game with only 1 player
   * Expected Response:  No state variables are initialized.
   * Variables:  numPlayers = 1
   * Setup:  Set number of players to 1 and call function
   */
  printf("\nTest 1:  Try to initialize game with only 1 player.\n");

  /* Scenario setup */
  struct gameState state1;

  initializeGame(numPlayers, k, seed, &state1);

  // Test number of players in state
  assertTrue(state1.numPlayers == 0, "State Number of Players",
    state1.numPlayers, 0);

  // Test supply count of curse cards
  assertTrue(state1.supplyCount[curse] == 0, "Supply Curse Cards",
    state1.supplyCount[curse], 0);



  /* ******************************* TEST 2 ******************************** */
  /* Action:  Try to initialize game with 5 players
  * Expected Response:  No state variables are initialized.  5 players is greater
      than max allowed.
  * Variables:  numPlayers = 5
  * Setup:  Set number of players to 5 and call function
  */
  printf("\nTest 2:  Try to initialize game with %d players (greater than max allowed).\n", MAX_PLAYERS + 1);

  /* Scenario setup */
  numPlayers = MAX_PLAYERS + 1;
  struct gameState state2;

  initializeGame(numPlayers, k, seed, &state2);

  // Test number of players in state
  assertTrue(state2.numPlayers == 0, "State Number of Players",
    state2.numPlayers, 0);

  // Test supply count of curse cards
  assertTrue(state2.supplyCount[curse] == 0, "Supply Curse Cards",
    state2.supplyCount[curse], 0);



/* ******************************* TEST 3 ******************************** */
  /* Action:  Try to initialize game with duplicate kingdom cards;
  * Expected Response:  No state changes.  Duplicate cards not allowed.
  * Variables:  k2[10] - kingdom cards with one duplicate
  * Setup:  2 players, kingdom cards with 1 duplicate
  */
  printf("\nTest 3:  Try to initialize game with duplicate kingdom cards.\n");

  /* Scenario setup */
  numPlayers = 2;
  struct gameState state4;
  int k2[10] = {adventurer, gardens, village, village, mine, cutpurse,
                sea_hag, tribute, smithy, council_room};

  initializeGame(numPlayers, k2, seed, &state4);

  // Test number of players in state
  assertTrue(state4.numPlayers == 0, "State Number of Players",
    state4.numPlayers, 0);

  // Test supply count of curse cards
  assertTrue(state4.supplyCount[curse] == 0, "Supply Curse Cards",
    state4.supplyCount[curse], 0);




  /* ******************************* TEST 4 ******************************** */
  /* Action:  Initialize game with allowable number of players.
   * Expected Response:  Normal state setup.
   * Variables:  numPlayers = 2 - MAX_PLAYERS
   * Setup:  Set number of players and call function
   */

  /* Scenario setup */
  struct gameState state3;

  for (int i = 2; i <= MAX_PLAYERS; i++){
    printf("\nTest 4.%d:  Initialize game with %d players.  Normal state setup.\n", i-1, i);
    initializeGame(i, k, seed, &state3);

    // Test number of players
    assertTrue(state3.numPlayers == i, "Player Count", state3.numPlayers, i);

    // Test number of curses
    assertTrue(state3.supplyCount[curse] == (i * 10) - 10, "Curse Supply Count",
      state3.supplyCount[curse], (i * 10) - 10);

    // Test number of victory cards
    int victoryCount = 0;
    if (i == 2){
      victoryCount = 8;
    } else {
      victoryCount = 12;
    }
    assertTrue(state3.supplyCount[estate] == victoryCount, "Estate Supply Count",
      state3.supplyCount[estate], victoryCount);
    assertTrue(state3.supplyCount[duchy] == victoryCount, "Duchy Supply Count",
      state3.supplyCount[duchy], victoryCount);
    assertTrue(state3.supplyCount[province] == victoryCount, "Province Supply Count",
      state3.supplyCount[province], victoryCount);

    // Test treasure cards
    assertTrue(state3.supplyCount[copper] == 60 - (7 * i), "Copper Supply Count",
      state3.supplyCount[copper], 60 - (7 * i));
    assertTrue(state3.supplyCount[silver] == 40, "Silver Supply Count",
      state3.supplyCount[silver], 40);
    assertTrue(state3.supplyCount[gold] == 30, "Gold Supply Count",
      state3.supplyCount[gold], 30);

    // Test kingdom cards (all should be 10 cards except for gardens)
    for (int j = 0 ; j < 10; j++){
      if (k[j] == gardens){
        if (i == 2){
          assertTrue(state3.supplyCount[k[j]] == 8, "Gardens Count",
            state3.supplyCount[k[j]], 8);
        } else {
          assertTrue(state3.supplyCount[k[j]] == 12, "Gardens Count",
            state3.supplyCount[k[j]], 8);
        }
      } else {
        assertTrue(state3.supplyCount[k[j]] == 10, "Kingdom Card Count",
          state3.supplyCount[k[j]], 10);
      }
    }

    // Test kingdom card count of cards not selected for game
    for (int j = adventurer; j <= treasure_map; j++){
      int selectFlag = 0;
      for (int n = 0; n < 10; n++){
        if (j == k[n]){
          selectFlag = 1;
        }
      }

      if (selectFlag == 0){
        assertTrue(state3.supplyCount[j] == -1, "Non-Playable Card Count",
          state3.supplyCount[j], -1);
      }
    }

    // Check player hands
    for (int j = 0; j < i; j++){
      // First player's hand
      if (j == 0){
        assertTrue(state3.handCount[j] == 5, "First Player Hand Count",
          state3.handCount[j], 5);
        assertTrue(state3.deckCount[j] == 5, "First Player Deck Count",
          state3.deckCount[j], 5);
      } else {
        assertTrue(state3.handCount[j] == 0, "Other Player Hand Count",
          state3.handCount[j], 0);
        assertTrue(state3.deckCount[j] ==  10, "Other Player Deck Count",
		  state3.deckCount[j], 10);
      }
    }

  }


  printf("\n################## TESTING COMPLETE:  %s ##################\n", TESTCARD);

  return 0;
}
