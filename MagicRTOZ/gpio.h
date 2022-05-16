#pragma once

#ifdef __cplusplus
  extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

struct _uhal_gpio_t;

typedef enum
{
  UHAL_GPIO_GROUP_A,
  UHAL_GPIO_GROUP_B,
  UHAL_GPIO_GROUP_C,
  UHAL_GPIO_GROUP_D,
  UHAL_GPIO_GROUP_E,
  UHAL_GPIO_GROUP_F,
  UHAL_GPIO_GROUP_G,
  UHAL_GPIO_GROUP_H,
  UHAL_GPIO_GROUP_I,
  UHAL_GPIO_GROUP_J,
  UHAL_GPIO_GROUP_K,
  UHAL_GPIO_GROUP_L,
  UHAL_GPIO_GROUP_M,
  UHAL_GPIO_GROUP_N,
  UHAL_GPIO_GROUP_O,
  UHAL_GPIO_GROUP_P,
  UHAL_GPIO_GROUP_Q,
  UHAL_GPIO_GROUP_R,
  UHAL_GPIO_GROUP_S,
  UHAL_GPIO_GROUP_T,
  UHAL_GPIO_GROUP_U,
  UHAL_GPIO_GROUP_V,
  UHAL_GPIO_GROUP_W,
  UHAL_GPIO_GROUP_X,
  UHAL_GPIO_GROUP_Y,
  UHAL_GPIO_GROUP_Z
} uhal_gpio_group_t;

typedef enum
{
  UHAL_GPIO_MODE_INPUT,
  UHAL_GPIO_MODE_OUTPUT
} uhal_gpio_mode_t;

typedef enum
{
  UHAL_GPIO_PULL_RESISTOR_NONE,
  UHAL_GPIO_PULL_RESISTOR_PULL_UP,
  UHAL_GPIO_PULL_RESISTOR_PULL_DOWN
} uhal_gpio_pull_resisitor_t;

typedef enum
{
  UHAL_GPIO_STATE_LOW,
  UHAL_GPIO_STATE_HIGH
} uhal_gpio_state_t;

typedef struct 
{
  uint8_t pin;
  uhal_gpio_group_t group;
  uhal_gpio_mode_t mode;
  uhal_gpio_pull_resisitor_t pull_resistor;
  struct _uhal_gpio_t* deprived;
} uhal_gpio_t;

uhal_status_t uhal_gpio_init(uhal_gpio_t* gpio);
uhal_status_t uhal_gpio_write(uhal_gpio_t* gpio, uhal_gpio_state_t state);
uhal_status_t uhal_gpio_toggle(uhal_gpio_t* gpio);
uhal_status_t uhal_gpio_read(uhal_gpio_t* gpio, uhal_gpio_state_t* state);

#ifdef __cplusplus
  }
#endif