/*  File Name:    cardtest4.c
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

# define TESTCARD "getWinners"

/* BUSINESS REQUIREMENTS:  getWinners ******************************
1)  Takes in an array of players and a gameState struct
2)  Gets a score for each player using the scoreFor function
3)  Adds 1 point to every player that hasn't had a turn yet
4)  Determines which player has the highest score
5)  Sets winner to 1 in the player array and the rest to 0
********************************************************************/

/* Helper function to empty player array */
void emptyArray(int players[MAX_PLAYERS]){
  for (int i = 0; i < MAX_PLAYERS; i++){
    players[i] = 0;
  }
}

int main(){
  /* Initialize testing and game variables */
  int numPlayers = 2;
  int thisPlayer = 0;
  int nextPlayer = thisPlayer + 1;
  int winner = -1;
  int seed = 1000;
  int k[10] = {adventurer, gardens, village, minion, mine, cutpurse,
                sea_hag, tribute, smithy, council_room};
  int players[MAX_PLAYERS];
  struct gameState state, testState;

  initializeGame(numPlayers, k, seed, &state);

  // int getWinners(int players[MAX_PLAYERS], struct gameState *state)

  printf("---------------- TESTING FUNCTION:  %s ----------------\n", TESTCARD);

  /* ******************************* TEST 1 ******************************** */
  /* Action:  Test 2 player game where players have equal victory points and
      it is player 1's turn
   * Expected Response:  Player 2 wins because they have had fewer turns
   * Variables:  players[MAX_PLAYERS] array to hold player scores, game state
   * Setup:  Initialize game and immediately call getWinners
   */
  printf("\nTest 1:  2 player game ends on player 1's turn.\n");

  /* Make a copy for comparison */
  memcpy(&testState, &state, sizeof(struct gameState));
  getWinners(players, &testState);

  // Verify player's turn
  assertTrue(testState.whoseTurn == thisPlayer, "Whose Turn",
  testState.whoseTurn, thisPlayer);

  // Test contents of player array
  for (int i = 0; i < MAX_PLAYERS; i++){
    if (players[i] == 1){
     winner = i;
    }
  }

  // Verify winner
  assertTrue(winner == nextPlayer, "Winning Player", winner, nextPlayer);



   /* ******************************* TEST 2 ******************************** */
   /* Action:  Test 2 player game where players have equal victory points and
       it is player 2's turn
    * Expected Response:  Tie
    * Variables:  players[MAX_PLAYERS] array to hold player scores, game state
    * Setup:  Initialize game and immediately call getWinners
    */
    printf("\nTest 2:  2 player game ends on player 2's turn.\n");

    /* Scenario setup */
    winner = -1;
    emptyArray(players);

    /* Make a copy for comparison */
    memcpy(&testState, &state, sizeof(struct gameState));
    endTurn(&testState);
    getWinners(players, &testState);

    // Test whose turn
    assertTrue(testState.whoseTurn == nextPlayer, "Whose Turn",
      testState.whoseTurn, nextPlayer);

    // Test contents of player array
    for (int i = 0; i < MAX_PLAYERS; i++){
      if (players[i] == 1){
        winner = i;
      }
    }

    assertTrue(winner == -1, "Winning Player", winner, -1);



  /* ******************************* TEST 3 ******************************** */
  /* Action:  Test 2 player game where player 2 has more victory points
  * Expected Response: Player 2 wins
  * Variables: players[MAX_PLAYERS] array to hold player scores
  * Setup:  Add a province to player 2's deck
  */
  printf("\nTest 3:  2 player game where player 2 has more victory points.\n");

  /* Scenario setup */
  winner = -1;
  emptyArray(players);

  /* Make a copy for comparison */
  memcpy(&testState, &state, sizeof(struct gameState));
  testState.deck[nextPlayer][testState.deckCount[nextPlayer] - 1] = province;
  getWinners(players, &testState);

  int p1 = 0;
  int p2 = 0;
  for (int i = 0; i < testState.handCount[thisPlayer]; i++){
	if (testState.hand[thisPlayer][i] == estate){
	  p1++;
	} else if (testState.hand[thisPlayer][i] == province){
	  p1 = p1 + 6;
	}
  }
  for (int i = 0; i < testState.deckCount[thisPlayer]; i++){
	if (testState.deck[thisPlayer][i] == estate){
	  p1++;
	} else if (testState.deck[thisPlayer][i] == province){
	  p1 = p1 + 6;
	}
  }
  for (int i = 0; i < testState.deckCount[nextPlayer]; i++){
	if (testState.deck[nextPlayer][i] == estate){
	  p2++;
	} else if (testState.deck[nextPlayer][i] == province){
	  p2 = p2 + 6;
	}
  }
  printf("Player 1 score:  %d\nPlayer 2 score:  %d\n\n", p1, p2);

  // Test contents of player array
  for (int i = 0; i < MAX_PLAYERS; i++){
    if (players[i] == 1){
      winner = i;
    }
  }
  assertTrue(winner == nextPlayer, "Winning Player", winner, nextPlayer);



  /* ******************************* TEST 4 ******************************** */
  /* Action:  Test 2 player game where player 1 has more victory points
   * Expected Response:  Player 1 wins
   * Variables: players[MAX_PLAYERS] array to hold player scores
   * Setup:  Add a province to player 1's deck
   */
  printf("\nTest 4:  2 player game where player 1 has more victory points.\n");

  /* Scenario setup */
  winner = -1;
  emptyArray(players);

  /* Make a copy for comparison */
  memcpy(&testState, &state, sizeof(struct gameState));
  testState.deck[thisPlayer][testState.deckCount[thisPlayer] - 1] = province;
  getWinners(players, &testState);

  /* Test contents of player array */
  for (int i = 0; i < MAX_PLAYERS; i++){
    if (players[i] == 1){
      winner = i;
    }
  }
  assertTrue(winner == thisPlayer, "Winning Player", winner, thisPlayer);


  printf("\n################## TESTING COMPLETE:  %s ##################\n", TESTCARD);

  return 0;
}
