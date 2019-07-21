/*  File Name:    cardtest5.c
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

# define TESTCARD "drawCard"

/* BUSINESS REQUIREMENTS:  drawCard ********************************
1)  Checks to see if the deck is empty
2)  If empty, moves all discarded cards into the deck, shuffles them,
    and then adds the last deck card to the player's hand
3)  If discard is empty, does nothing
4)  If not empty, moves the last deck card into player's hand
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
// Test with empty deck
// Test with empty deck and empty discard pile
// Test with non-empty deck
