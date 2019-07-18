/*  File:         unittest_helper.c
 *  Author:       Stephen Thomas
 *  Date:         7/21/19
 *  Description:
*/

#include "unittest_helper.h"
#include <stdio.h>
#include <string.h>

void assertTrue(int expression, char* testVar, int result, int expected){
  if (expression) {
    printf("Assert Test PASSED\n");
  } else {
    printf("Assert Test FAILED\n");
  }

  printf("\t%s = %d, expected = %d\n", test, result, expected);
}
