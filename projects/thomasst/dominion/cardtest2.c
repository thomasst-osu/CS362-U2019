/*  File Name:    cardtest2.c
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

# define TESTCARD "shuffle"

/* BUSINESS REQUIREMENTS:  shuffle *********************************
1)  Takes in a player request and gameState struct
2)  Verifies there are cards in the requested deck
3)  Sorts the cards in the deck by qsort
4)  Uses the random number generator to randomly pick out cards from
    the sorted deck and place them in a temporary placeholder deck.
5)  Places the cards back in the real deck after all have been "shuffled"
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
// Test when indicated player has 0 cards in deck
// Test a full deck
