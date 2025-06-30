#include "test_common.h"

// Error Tests
void test_error_functionality() {
  TEST_START("Error Functionality");
  
  // Test 1: Valid error messages
  TEST_CASE("Valid error messages");
  const char *msg = orionpp_strerr(ORIONPP_ERROR_GOOD);
  TEST_ASSERT(msg != NULL && strlen(msg) > 0, "Good error should have message");
  
  msg = orionpp_strerr(ORIONPP_ERROR_INVALID_ARG);
  TEST_ASSERT(msg != NULL && strlen(msg) > 0, "Invalid arg error should have message");
  
  msg = orionpp_strerr(ORIONPP_ERROR_OUT_OF_MEMORY);
  TEST_ASSERT(msg != NULL && strlen(msg) > 0, "Out of memory error should have message");
  
  // Test 2: Invalid error codes
  TEST_CASE("Invalid error codes");
  msg = orionpp_strerr(-1);
  TEST_ASSERT(msg != NULL && strlen(msg) > 0, "Invalid error code should return unknown message");
  
  msg = orionpp_strerr(999);
  TEST_ASSERT(msg != NULL && strlen(msg) > 0, "Large error code should return unknown message");
  
  // Test 3: All defined error codes
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
}

// Header Tests
void test_header_functionality() {
  TEST_START("Header Functionality");
  
  orionpp_header_t header;
  orionpp_error_t err;
  
  // Test 1: Header initialization
  TEST_CASE("Header initialization");
  err = orionpp_header_init(&header);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Header initialization should succeed");
  
  // Test 2: Header initialization with NULL
  TEST_CASE("Header initialization with NULL");
  err = orionpp_header_init(NULL);
  TEST_ASSERT(err == ORIONPP_ERROR_INVALID_ARG, "Should fail with NULL header");
  
  // Test 3: Header validation
  TEST_CASE("Header validation");
  err = orionpp_header_validate(&header);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Valid header should pass validation");
  
  // Test 4: Header validation with NULL
  TEST_CASE("Header validation with NULL");
  err = orionpp_header_validate(NULL);
  TEST_ASSERT(err == ORIONPP_ERROR_INVALID_ARG, "Should fail with NULL header");
  
  // Test 5: Magic number validation
  TEST_CASE("Invalid magic number");
  orionpp_header_t bad_header = header;
  bad_header.magic0 = 0xFF;
  err = orionpp_header_validate(&bad_header);
  TEST_ASSERT(err == ORIONPP_ERROR_INVALID_MAGIC, "Should fail with invalid magic");
  
  // Test 6: Version validation (future version)
  TEST_CASE("Future version validation");
  bad_header = header;
  bad_header.major = 99;
  err = orionpp_header_validate(&bad_header);
  TEST_ASSERT(err == ORIONPP_ERROR_INVALID_VERSION, "Should fail with future version");
  
  // Test 7: Feature checking
  TEST_CASE("Feature checking");
  int has_feature = orionpp_header_has_feature(&header, 1);
  TEST_ASSERT(has_feature == 0, "Default header should have no features");
  
  // Test 8: Feature checking with NULL
  TEST_CASE("Feature checking with NULL");
  has_feature = orionpp_header_has_feature(NULL, 1);
  TEST_ASSERT(has_feature == 0, "NULL header should return no features");
  
  // Test 9: Set and check feature
  TEST_CASE("Set and check feature");
  header.features = 0x01;
  has_feature = orionpp_header_has_feature(&header, 1);
  TEST_ASSERT(has_feature != 0, "Should detect set feature");
  
  has_feature = orionpp_header_has_feature(&header, 2);
  TEST_ASSERT(has_feature == 0, "Should not detect unset feature");
  
  // Test 10: Multiple features
  TEST_CASE("Multiple features");
  header.features = 0x05; // Features 1 and 3
  has_feature = orionpp_header_has_feature(&header, 1);
  TEST_ASSERT(has_feature != 0, "Should detect first feature");
  
  has_feature = orionpp_header_has_feature(&header, 4);
  TEST_ASSERT(has_feature != 0, "Should detect third feature");
  
  has_feature = orionpp_header_has_feature(&header, 2);
  TEST_ASSERT(has_feature == 0, "Should not detect unset middle feature");
}