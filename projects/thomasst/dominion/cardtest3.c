/*  File Name:    cardtest3.c
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

# define TESTCARD "endTurn"

/* BUSINESS REQUIREMENTS:  endTurn *********************************
1)  Discards all cards in player's hands directly to the discard pile
    (not using discardCard).
2)  Player's hand count is reset to 0
3)  Next player is placed into state's whoseTurn variable
4)  Other state variables are reset for next player
5)  5 cards are drawn for the new player
6)  New player's coins are updated using the updateCoins function
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
// Test with current player = 0
// Test with current player = last player
