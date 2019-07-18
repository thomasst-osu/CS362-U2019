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
1)
2)
3)
********************************************************************/


int main(){
  /* Initialize testing and game variables */
  int newCards = 0;
  int discarded = 1;
  int xtraCoins = 0;
  int shuffleCards = 0;

  int i, j, m;
  int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
  int remove1, remove2;
  int seed = 1000;
  int numPlayers = 2;
  int thisPlayer = 0;

  struct gameState G, testG;
  int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
                sea_hag, tribute, smithy, council_room};

  /* Initialize game state and player cards */
  initializeGame(numPlayers, k, seed, &G);

  printf("---------------- TESTING CARD:  %s ----------------\n", TESTCARD);

  /* *********************** TEST 1 *********************** */


  /* *********************** TEST 2 *********************** */


  printf("################## TESTING COMPLETE:  %s ##################\n", TESTCARD);

  return 0;

}
