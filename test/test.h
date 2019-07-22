/**
 * @file
 * @brief Test suite
 * @author James Humphrey (humphreyj@somnisoft.com)
 *
 * This software has been placed into the public domain using CC0.
 */
#ifndef LOGNAME_TEST_H
#define LOGNAME_TEST_H

int
logname_main(int argc,
             char *argv[]);

char *
test_seam_getlogin(void);

int
test_seam_puts(const char *s);

extern int g_test_seam_err_force_errno;
extern int g_test_seam_err_ctr_getlogin;
extern int g_test_seam_err_ctr_puts;

#endif /* LOGNAME_TEST_H */

