/**
 * @file
 * @brief Test suite
 * @author James Humphrey (humphreyj@somnisoft.com)
 *
 * This software has been placed into the public domain using CC0.
 */

#include <sys/types.h>
#include <sys/wait.h>
#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "test.h"

/**
 * Generate the output of the command to this file.
 */
#define PATH_LOGNAME_OUT "build/test-logname.txt"

/**
 * Expected logname output.
 */
#define PATH_LOGNAME_EXPECT "build/test-logname-expect.txt"

/**
 * Call @ref logname_main and check the results.
 *
 * @param[in] extra_arg          Add another argument.
 * @param[in] expect_exit_status Expected exit code from @ref logname_main.
 * @param[in] expect_logname     Expected login name printed to STDOUT. Set
 *                               to NULL if not expecting a login name
 *                               to print.
 */
static void
test_logname_main(const bool extra_arg,
                  const int expect_exit_status,
                  const char *const expect_logname){
  int exit_status;
  int status;
  pid_t pid;
  FILE *new_stdout;
  int i;
  int argc;
  char *argv[10];
  char cmd[1000];

  argc = 0;
  argv[argc++] = strdup("logname");
  assert(argv[0]);
  if(extra_arg){
    argv[argc++] = strdup("-a");
    assert(argv[1]);
  }

  pid = fork();
  assert(pid >= 0);
  if(pid == 0){
    new_stdout = freopen(PATH_LOGNAME_OUT, "w", stdout);
    assert(new_stdout);
    exit_status = logname_main(argc, argv);
    exit(exit_status);
  }
  assert(waitpid(pid, &status, 0) == pid);
  assert(WIFEXITED(status));
  assert(WEXITSTATUS(status) == expect_exit_status);

  if(expect_logname){
    sprintf(cmd, "echo \'%s\' > \'%s\'", expect_logname, PATH_LOGNAME_EXPECT);
    assert(system(cmd) == 0);
  }
  else{
    sprintf(cmd, "cat /dev/null > \'%s\'", PATH_LOGNAME_EXPECT);
    assert(system(cmd) == 0);
  }

  sprintf(cmd, "cmp \'%s\' \'%s\'", PATH_LOGNAME_OUT, PATH_LOGNAME_EXPECT);
  assert(system(cmd) == 0);

  for(i = 0; i < argc; i++){
    free(argv[i]);
  }
}

/**
 * Run all test cases for logname.
 */
static void
test_logname_all(void){
  const char *env_logname;

  env_logname = getenv("LOGNAME");
  assert(env_logname);

  /* Successful call. */
  test_logname_main(false, EXIT_SUCCESS, env_logname);

  /* Too many arguments. */
  test_logname_main(true, EXIT_FAILURE, NULL);

  /* getlogin returns no result. */
  g_test_seam_err_force_errno = 0;
  g_test_seam_err_ctr_getlogin = 0;
  test_logname_main(false, EXIT_FAILURE, NULL);
  g_test_seam_err_force_errno = 0;
  g_test_seam_err_ctr_getlogin = -1;

  /* getlogin fails with error. */
  g_test_seam_err_force_errno = EMFILE;
  g_test_seam_err_ctr_getlogin = 0;
  test_logname_main(false, EXIT_FAILURE, NULL);
  g_test_seam_err_force_errno = 0;
  g_test_seam_err_ctr_getlogin = -1;

  /* Fail to write logname to stdout. */
  g_test_seam_err_ctr_puts = 0;
  test_logname_main(false, EXIT_FAILURE, NULL);
  g_test_seam_err_ctr_puts = -1;
}

/**
 * Test logname utility.
 *
 * Usage: test
 *
 * @retval 0 All tests passed.
 */
int
main(void){
  test_logname_all();
  return 0;
}

