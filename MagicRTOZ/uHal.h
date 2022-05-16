#pragma once

#ifdef __cplusplus
  extern "C" {
#endif

typedef enum
{
    UHAL_STATUS_OK,
    UHAL_STATUS_ERROR
} uhal_status_t;

#include "./gpio.h"

#ifdef __cplusplus
  }
#endif