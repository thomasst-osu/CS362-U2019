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

int main(){

  printf("---------------- TESTING FUNCTION:  %s ----------------\n", TESTCARD);

  /* ******************************* TEST 1 ******************************** */
  /* Action:
   * Expected Response:
   * Variables:
   * Setup:
   */



  /* ******************************* TEST 2 ******************************** */
  /* Action:
  * Expected Response:
  * Variables:
  * Setup:
  */



  /* ******************************* TEST 3 ******************************** */
  /* Action:
   * Expected Response:
   * Variables:
   * Setup:
   */


  /* ******************************* TEST 4 ******************************** */
  /* Action:
  * Expected Response:
  * Variables:
  * Setup:
  */

  printf("\n################## TESTING COMPLETE:  %s ##################\n", TESTCARD);

  return 0;
}
// Test with less than max players
// Test with non-first player winning
