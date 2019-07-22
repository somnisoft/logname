/**
 * @file
 * @brief logname
 * @author James Humphrey (humphreyj@somnisoft.com)
 *
 * Print user login name.
 *
 * This software has been placed into the public domain using CC0.
 */

#include <err.h>
#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifdef TEST
/**
 * Declare some functions with extern linkage, allowing the test suite to call
 * those functions.
 */
# define LINKAGE extern
# include "../test/seams.h"
#else /* !(TEST) */
/**
 * Define all functions as static when not testing.
 */
# define LINKAGE static
#endif /* TEST */

/**
 * logname utility context.
 */
struct logname{
  /**
   * Exit status set to one of the following values.
   *   - EXIT_SUCCESS
   *   - EXIT_FAILURE
   */
  int status_code;
};

/**
 * Print an error message to STDERR and set an error status code.
 *
 * @param[in,out] logname   See @ref logname.
 * @param[in]     errno_msg Include a standard message describing errno.
 * @param[in]     fmt       Format string arguments used by vwarn.
 */
static void
logname_warn(struct logname *const logname,
             const bool errno_msg,
             const char *const fmt, ...){
  va_list ap;

  logname->status_code = EXIT_FAILURE;
  va_start(ap, fmt);
  if(errno_msg){
    vwarn(fmt, ap);
  }
  else{
    vwarnx(fmt, ap);
  }
  va_end(ap);
}

/**
 * Main entry point for logname utility.
 *
 * @param[in]     argc         Number of arguments in @p argv.
 * @param[in,out] argv         Argument list.
 * @retval        EXIT_SUCCESS Successful.
 * @retval        EXIT_FAILURE Error occurred.
 */
LINKAGE int
logname_main(int argc,
             char *argv[]){
  struct logname logname;
  char *login_name;

  (void)argv;
  memset(&logname, 0, sizeof(logname));
  if(argc > 1){
    logname_warn(&logname, false, "too many arguments");
  }
  else{
    errno = 0;
    login_name = getlogin();
    if(login_name == NULL){
      if(errno){
        logname_warn(&logname, true, "getlogin");
      }
      else{
        logname_warn(&logname, false, "getlogin: failed to get login name");
      }
    }
    else{
      if(puts(login_name) == EOF){
        logname_warn(&logname, true, "puts: %s", login_name);
      }
    }
  }
  return logname.status_code;
}

#ifndef TEST
/**
 * Main program entry point.
 *
 * @param[in]     argc See @ref logname_main.
 * @param[in,out] argv See @ref logname_main.
 * @return             See @ref logname_main.
 */
int
main(int argc,
     char *argv[]){
  return logname_main(argc, argv);
}
#endif /* TEST */

