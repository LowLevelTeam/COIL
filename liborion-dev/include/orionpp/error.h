#ifndef __ORIONPP_ERROR
#define __ORIONPP_ERROR

enum orionpp_error {
  ORIONPP_ERROR_GOOD,
};

typedef int orionpp_error_t;

const char *orionpp_strerr(orionpp_error_t err);

#endif // __ORIONPP_ERROR