#include "./attiny85.h"
#if (defined(__AVR_ATtiny85__) && (ENABLE_GPIO == 1))

  #include "./uhal.h"
  #include <avr/io.h>
  #include <stdio.h>

  struct _uhal_gpio_t
  {
    volatile uint8_t reg_pin;
    volatile uint8_t reg_ddr;
    volatile uint8_t reg_port;
  };

  uhal_status_t uhal_gpio_init(uhal_gpio_t* gpio)
  {
    if (gpio == NULL)
    {
      return UHAL_STATUS_ERROR;
    }
    if (gpio->pin > 5)
    {
      return UHAL_STATUS_ERROR;
    }

    switch(gpio->group)
    {
      case UHAL_GPIO_GROUP_B: gpio->deprived = (struct _uhal_gpio_t*) &PINB ; break;
      default: return UHAL_STATUS_ERROR;
    }
    
    if (gpio->mode == UHAL_GPIO_MODE_OUTPUT)
    {
      gpio->deprived->reg_ddr |= (1 << gpio->pin);
    }
    else
    {
      gpio->deprived->reg_ddr &= ~(1 << gpio->pin);
      if (gpio->pull_resistor == UHAL_GPIO_PULL_RESISTOR_PULL_UP)
      {
        gpio->deprived->reg_port |= (1 << gpio->pin);
      }
    }
    return UHAL_STATUS_OK; 
  }

  uhal_status_t uhal_gpio_write(uhal_gpio_t* gpio, uhal_gpio_state_t state)
  {
    if(gpio == NULL)
    {
      return UHAL_STATUS_ERROR;
    }
    if(gpio->mode == UHAL_GPIO_MODE_INPUT)
    {
      return UHAL_STATUS_ERROR;
    }
    
    switch (state)
    {
      case UHAL_GPIO_STATE_LOW: gpio->deprived->reg_port &= ~(1 << gpio->pin); break;
      case UHAL_GPIO_STATE_HIGH: gpio->deprived->reg_port |= (1 << gpio->pin); break;
      default: return UHAL_STATUS_ERROR;
    }

    return UHAL_STATUS_OK;
  }

  uhal_status_t uhal_gpio_toggle(uhal_gpio_t* gpio)
  {
    if(gpio == NULL)
    {
      return UHAL_STATUS_ERROR;
    }
    if(gpio->mode == UHAL_GPIO_MODE_INPUT)
    {
      return UHAL_STATUS_ERROR;
    }

    gpio->deprived->reg_port ^= (1 << gpio->pin);
    return UHAL_STATUS_OK;
  }

  uhal_status_t uhal_gpio_read(uhal_gpio_t* gpio, uhal_gpio_state_t* state)
  {
    if(gpio == NULL)
    {
      return UHAL_STATUS_ERROR;
    }
    if(gpio->mode == UHAL_GPIO_MODE_OUTPUT)
    {
      return UHAL_STATUS_ERROR;
    }
    *state = ((gpio->deprived->reg_pin >> gpio->pin) & 1) == 0 ? UHAL_GPIO_STATE_LOW : UHAL_GPIO_STATE_HIGH;
    return UHAL_STATUS_OK;
  }

#endif