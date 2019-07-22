/**
 * @file
 * @brief Test seams
 * @author James Humphrey (humphreyj@somnisoft.com)
 *
 * This software has been placed into the public domain using CC0.
 */
#ifndef LOGNAME_TEST_SEAMS_H
#define LOGNAME_TEST_SEAMS_H

#include "test.h"

/*
 * Redefine these functions to internal test seams.
 */
#undef getlogin
#undef puts

/**
 * Inject a test seam to replace getlogin().
 */
#define getlogin test_seam_getlogin

/**
 * Inject a test seam to replace puts().
 */
#define puts test_seam_puts

#endif /* LOGNAME_TEST_SEAMS_H */

