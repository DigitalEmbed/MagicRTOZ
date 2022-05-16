#include "./atmega328.h"
#if ((defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__)) && (ENABLE_GPIO == 1))

  #include "./MagicRTOZ.h"
  #include <avr/io.h>
  #include <stdio.h>

  struct _gpio_t
  {
    volatile uint8_t reg_pin;
    volatile uint8_t reg_ddr;
    volatile uint8_t reg_port;
  };

  status_t gpio_init(gpio_t* gpio)
  {
    if (gpio == NULL)
    {
      return STATUS_ERROR;
    }
    if (gpio->pin > 7)
    {
      return STATUS_ERROR;
    }

    switch(gpio->group)
    {
      case GPIO_GROUP_B: gpio->deprived = (struct _gpio_t*) &PINB ; break;
      case GPIO_GROUP_C: gpio->deprived = (struct _gpio_t*) &PINC ; break;
      case GPIO_GROUP_D: gpio->deprived = (struct _gpio_t*) &PIND ; break;
      default: return STATUS_ERROR;
    }
    
    if (gpio->mode == GPIO_MODE_OUTPUT)
    {
      gpio->deprived->reg_ddr |= (1 << gpio->pin);
    }
    else
    {
      gpio->deprived->reg_ddr &= ~(1 << gpio->pin);
      if (gpio->pull_resistor == GPIO_PULL_RESISTOR_PULL_UP)
      {
        gpio->deprived->reg_port |= (1 << gpio->pin);
      }
    }
    return STATUS_OK; 
  }

  status_t gpio_write(gpio_t* gpio, gpio_state_t state)
  {
    if(gpio == NULL)
    {
      return STATUS_ERROR;
    }
    if(gpio->mode == GPIO_MODE_INPUT)
    {
      return STATUS_ERROR;
    }
    
    switch (state)
    {
      case GPIO_STATE_LOW: gpio->deprived->reg_port &= ~(1 << gpio->pin); break;
      case GPIO_STATE_HIGH: gpio->deprived->reg_port |= (1 << gpio->pin); break;
      default: return STATUS_ERROR;
    }

    return STATUS_OK;
  }

  status_t gpio_toggle(gpio_t* gpio)
  {
    if(gpio == NULL)
    {
      return STATUS_ERROR;
    }
    if(gpio->mode == GPIO_MODE_INPUT)
    {
      return STATUS_ERROR;
    }

    gpio->deprived->reg_port ^= (1 << gpio->pin);
    return STATUS_OK;
  }

  status_t gpio_read(gpio_t* gpio, gpio_state_t* state)
  {
    if(gpio == NULL)
    {
      return STATUS_ERROR;
    }
    if(gpio->mode == GPIO_MODE_OUTPUT)
    {
      return STATUS_ERROR;
    }
    *state = ((gpio->deprived->reg_pin >> gpio->pin) & 1) == 0 ? GPIO_STATE_LOW : GPIO_STATE_HIGH;
    return STATUS_OK;
  }

#endif