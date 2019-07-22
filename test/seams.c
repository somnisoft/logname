/**
 * @file
 * @brief Test seams
 * @author James Humphrey (humphreyj@somnisoft.com)
 *
 * This software has been placed into the public domain using CC0.
 */

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#include "test.h"

/**
 * Specify the errno value when a function fails.
 */
int g_test_seam_err_force_errno = 0;

/**
 * Error counter for @ref test_seam_getlogin.
 */
int g_test_seam_err_ctr_getlogin = -1;

/**
 * Error counter for @ref test_seam_puts.
 */
int g_test_seam_err_ctr_puts = -1;

/**
 * Decrement an error counter until it reaches -1.
 *
 * After a counter reaches -1, it will return a true response. This gets
 * used by the test suite to denote when to cause a function to fail. For
 * example, the unit test might need to cause the malloc() function to fail
 * after calling it a third time. In that case, the counter should initially
 * get set to 2 and will get decremented every time this function gets called.
 *
 * @param[in,out] err_ctr Error counter to decrement.
 * @retval        true    The counter has reached -1.
 * @retval        false   The counter has been decremented, but did not reach
 *                        -1 yet.
 */
static bool
test_seam_dec_err_ctr(int *const err_ctr){
  bool reached_end;

  reached_end = false;
  if(*err_ctr >= 0){
    *err_ctr -= 1;
    if(*err_ctr < 0){
      reached_end = true;
    }
  }
  return reached_end;
}

/**
 * Control when getlogin() fails.
 *
 * @retval char* Login name.
 * @retval NULL  Failed to get login name.
 */
char *
test_seam_getlogin(void){
  char *login;

  if(test_seam_dec_err_ctr(&g_test_seam_err_ctr_getlogin)){
    login = NULL;
    if(g_test_seam_err_force_errno){
      errno = g_test_seam_err_force_errno;
    }
  }
  else{
    login = getlogin();
  }
  return login;
}

/**
 * Control when puts() fails.
 *
 * @param[in] s   Print this string followed by a newline.
 * @retval    >0  Successfully wrote string.
 * @retval    EOF Failed to write string.
 */
int
test_seam_puts(const char *s){
  int rc;

  if(test_seam_dec_err_ctr(&g_test_seam_err_ctr_puts)){
    rc = EOF;
    errno = EAGAIN;
  }
  else{
    rc = puts(s);
  }
  return rc;
}

