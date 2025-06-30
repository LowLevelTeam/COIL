#include "test_framework.h"
#include "orionpp/header.h"

/**
 * @file test_header.c
 * @brief Header validation tests
 * 
 * Tests header initialization, validation, and feature management.
 */

void test_header_core() {
  TEST_START("Header Core Operations");
  
  orionpp_header_t header;
  orionpp_error_t err;
  
  TEST_CASE("Header initialization");
  err = orionpp_header_init(&header);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Init should succeed");
  
  TEST_CASE("Header validation");
  err = orionpp_header_validate(&header);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Valid header should pass validation");
  
  TEST_CASE("Magic number check");
  TEST_ASSERT(header.magic0 == 'O' && header.magic1 == 'P' && 
              header.magic2 == 'P' && header.magic3 == 0xD4, 
              "Magic number should be set correctly");
  
  TEST_CASE("Version check");
  TEST_ASSERT(header.major == 1 && header.minor == 0 && header.patch == 0,
              "Version should be 1.0.0");
}

void test_header_validation() {
  TEST_START("Header Validation");
  
  orionpp_header_t header, bad_header;
  orionpp_error_t err;
  
  orionpp_header_init(&header);
  
  TEST_CASE("Invalid magic number");
  bad_header = header;
  bad_header.magic0 = 0xFF;
  err = orionpp_header_validate(&bad_header);
  TEST_ASSERT(err == ORIONPP_ERROR_INVALID_MAGIC, "Should fail with invalid magic");
  
  TEST_CASE("Future version validation");
  bad_header = header;
  bad_header.major = 99;
  err = orionpp_header_validate(&bad_header);
  TEST_ASSERT(err == ORIONPP_ERROR_INVALID_VERSION, "Should fail with future version");
  
  bad_header = header;
  bad_header.minor = 99;
  err = orionpp_header_validate(&bad_header);
  TEST_ASSERT(err == ORIONPP_ERROR_INVALID_VERSION, "Should fail with future minor version");
  
  TEST_CASE("Patch version tolerance");
  bad_header = header;
  bad_header.patch = 99;
  err = orionpp_header_validate(&bad_header);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Should accept future patch version");
}

void test_header_features() {
  TEST_START("Header Feature Management");
  
  orionpp_header_t header;
  orionpp_header_init(&header);
  
  TEST_CASE("Default features");
  int has_feature = orionpp_header_has_feature(&header, ORIONPP_FEATURE_ABI);
  TEST_ASSERT(has_feature == 0, "Default header should have no features");
  
  TEST_CASE("Set and check feature");
  header.features |= (1 << ORIONPP_FEATURE_ABI);
  has_feature = orionpp_header_has_feature(&header, ORIONPP_FEATURE_ABI);
  TEST_ASSERT(has_feature != 0, "Should detect set feature");
  
  has_feature = orionpp_header_has_feature(&header, ORIONPP_FEATURE_STL);
  TEST_ASSERT(has_feature == 0, "Should not detect unset feature");
  
  TEST_CASE("Multiple features");
  header.features |= (1 << ORIONPP_FEATURE_STL);
  has_feature = orionpp_header_has_feature(&header, ORIONPP_FEATURE_ABI);
  TEST_ASSERT(has_feature != 0, "Should still detect first feature");
  
  has_feature = orionpp_header_has_feature(&header, ORIONPP_FEATURE_STL);
  TEST_ASSERT(has_feature != 0, "Should detect second feature");
  
  has_feature = orionpp_header_has_feature(&header, ORIONPP_FEATURE_CSTL);
  TEST_ASSERT(has_feature == 0, "Should not detect unset feature");
}

void test_header_errors() {
  TEST_START("Header Error Conditions");
  
  orionpp_error_t err;
  
  TEST_CASE("NULL pointer handling");
  err = orionpp_header_init(NULL);
  TEST_ASSERT(err == ORIONPP_ERROR_INVALID_ARG, "Init with NULL should fail");
  
  err = orionpp_header_validate(NULL);
  TEST_ASSERT(err == ORIONPP_ERROR_INVALID_ARG, "Validate with NULL should fail");
  
  int has_feature = orionpp_header_has_feature(NULL, ORIONPP_FEATURE_ABI);
  TEST_ASSERT(has_feature == 0, "Feature check with NULL should return 0");
}