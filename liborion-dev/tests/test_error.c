#include "test_framework.h"
#include "orionpp/error.h"

/**
 * @file test_error.c
 * @brief Error handling tests
 * 
 * Tests error message retrieval and validation.
 */

void test_error_messages() {
  TEST_START("Error Message Handling");
  
  const char *msg;
  
  TEST_CASE("Valid error messages");
  msg = orionpp_strerr(ORIONPP_ERROR_GOOD);
  TEST_ASSERT(msg != NULL && strlen(msg) > 0, "Success error should have message");
  
  msg = orionpp_strerr(ORIONPP_ERROR_INVALID_ARG);
  TEST_ASSERT(msg != NULL && strlen(msg) > 0, "Invalid arg error should have message");
  
  msg = orionpp_strerr(ORIONPP_ERROR_OUT_OF_MEMORY);
  TEST_ASSERT(msg != NULL && strlen(msg) > 0, "Out of memory error should have message");
  
  msg = orionpp_strerr(ORIONPP_ERROR_ARENA_FULL);
  TEST_ASSERT(msg != NULL && strlen(msg) > 0, "Arena full error should have message");
  
  TEST_CASE("Invalid error codes");
  msg = orionpp_strerr(-1);
  TEST_ASSERT(msg != NULL && strlen(msg) > 0, "Invalid error code should return unknown message");
  
  msg = orionpp_strerr(999);
  TEST_ASSERT(msg != NULL && strlen(msg) > 0, "Large error code should return unknown message");
  
  TEST_CASE("All defined error codes");
  int all_valid = 1;
  for (int i = ORIONPP_ERROR_GOOD; i <= ORIONPP_ERROR_UNKNOWN; i++) {
    msg = orionpp_strerr(i);
    if (msg == NULL || strlen(msg) == 0) {
      all_valid = 0;
      break;
    }
  }
  TEST_ASSERT(all_valid, "All defined error codes should have valid messages");
  
  TEST_CASE("Error message consistency");
  const char *good_msg = orionpp_strerr(ORIONPP_ERROR_GOOD);
  const char *unknown_msg = orionpp_strerr(ORIONPP_ERROR_UNKNOWN);
  const char *invalid_msg = orionpp_strerr(-999);
  
  TEST_ASSERT(strcmp(unknown_msg, invalid_msg) == 0, 
              "Unknown and invalid error codes should return same message");
  TEST_ASSERT(strcmp(good_msg, unknown_msg) != 0,
              "Success and unknown messages should be different");
}