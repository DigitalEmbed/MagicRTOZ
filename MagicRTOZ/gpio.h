#pragma once

#ifdef __cplusplus
  extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

#define GPIO_FUNCTIONS_LIST\
  X(status_t, init, (gpio_t* gpio,  uint8_t pin, gpio_group_t group, gpio_mode_t mode, gpio_pull_resisitor_t pull_resistor))\
  X(status_t, write, (gpio_t* gpio, gpio_state_t state))\
  X(status_t, toggle, (gpio_t* gpio))\
  X(status_t, read, (gpio_t* const gpio, gpio_state_t* state))

struct _gpio_t;

typedef enum
{
  GPIO_GROUP_A,
  GPIO_GROUP_B,
  GPIO_GROUP_C,
  GPIO_GROUP_D,
  GPIO_GROUP_E,
  GPIO_GROUP_F,
  GPIO_GROUP_G,
  GPIO_GROUP_H,
  GPIO_GROUP_I,
  GPIO_GROUP_J,
  GPIO_GROUP_K,
  GPIO_GROUP_L,
  GPIO_GROUP_M,
  GPIO_GROUP_N,
  GPIO_GROUP_O,
  GPIO_GROUP_P,
  GPIO_GROUP_Q,
  GPIO_GROUP_R,
  GPIO_GROUP_S,
  GPIO_GROUP_T,
  GPIO_GROUP_U,
  GPIO_GROUP_V,
  GPIO_GROUP_W,
  GPIO_GROUP_X,
  GPIO_GROUP_Y,
  GPIO_GROUP_Z
} gpio_group_t;

typedef enum
{
  GPIO_MODE_INPUT,
  GPIO_MODE_OUTPUT
} gpio_mode_t;

typedef enum
{
  GPIO_PULL_RESISTOR_NONE,
  GPIO_PULL_RESISTOR_PULL_UP,
  GPIO_PULL_RESISTOR_PULL_DOWN
} gpio_pull_resisitor_t;

typedef enum
{
  GPIO_STATE_LOW,
  GPIO_STATE_HIGH
} gpio_state_t;

typedef struct 
{
  uint8_t _pin;
  gpio_group_t _group;
  gpio_mode_t _mode;
  gpio_pull_resisitor_t _pull_resistor;
  struct _gpio_t* _deprived;
} gpio_t;

#define X(RETURN, FUNCTION, ARGUMENTS)\
  RETURN gpio_##FUNCTION ARGUMENTS;

  GPIO_FUNCTIONS_LIST
#undef X

#if defined(ENABLE_SYSTEM_PSEUDO_CLASSES)
  const struct gpio_class_t
  {
    #define X(RETURN, FUNCTION, ARGUMENTS)\
      RETURN (*FUNCTION) ARGUMENTS;

      GPIO_FUNCTIONS_LIST
    #undef X
  } 
  GPIO = 
  {
    #define X(RETURN, FUNCTION, ARGUMENTS)\
      gpio_##FUNCTION,

      GPIO_FUNCTIONS_LIST
    #undef X
  };
#endif

#ifdef __cplusplus
  }
#endif