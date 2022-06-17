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
  
  status_t gpio_init(gpio_t* gpio, uint8_t pin, gpio_group_t group, gpio_mode_t mode, gpio_pull_resisitor_t pull_resistor)
  {
    if (gpio == NULL)
    {
      return STATUS_ERROR;
    }
    
    if (pin > 7)
    {
      return STATUS_ERROR;
    }

    gpio->_pin = pin;
    gpio->_group = group;
    gpio->_mode = mode;
    gpio->_pull_resistor = pull_resistor;

    switch(gpio->_group)
    {
      case GPIO_GROUP_B: gpio->_deprived = (struct _gpio_t*) &PINB ; break;
      case GPIO_GROUP_C: gpio->_deprived = (struct _gpio_t*) &PINC ; break;
      case GPIO_GROUP_D: gpio->_deprived = (struct _gpio_t*) &PIND ; break;
      default: return STATUS_ERROR;
    }
    
    if (gpio->_mode == GPIO_MODE_OUTPUT)
    {
      gpio->_deprived->reg_ddr |= (1 << gpio->_pin);
    }
    else
    {
      gpio->_deprived->reg_ddr &= ~(1 << gpio->_pin);
      if (gpio->_pull_resistor == GPIO_PULL_RESISTOR_PULL_UP)
      {
        gpio->_deprived->reg_port |= (1 << gpio->_pin);
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
    if(gpio->_mode == GPIO_MODE_INPUT)
    {
      return STATUS_ERROR;
    }
    
    switch (state)
    {
      case GPIO_STATE_LOW: gpio->_deprived->reg_port &= ~(1 << gpio->_pin); break;
      case GPIO_STATE_HIGH: gpio->_deprived->reg_port |= (1 << gpio->_pin); break;
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
    if(gpio->_mode == GPIO_MODE_INPUT)
    {
      return STATUS_ERROR;
    }

    gpio->_deprived->reg_port ^= (1 << gpio->_pin);
    return STATUS_OK;
  }

  status_t gpio_read(gpio_t* const gpio, gpio_state_t* state)
  {
    if(gpio == NULL)
    {
      return STATUS_ERROR;
    }
    if(gpio->_mode == GPIO_MODE_OUTPUT)
    {
      return STATUS_ERROR;
    }
    *state = ((gpio->_deprived->reg_pin >> gpio->_pin) & 1) == 0 ? GPIO_STATE_LOW : GPIO_STATE_HIGH;
    return STATUS_OK;
  }

  #if defined(ENABLE_SYSTEM_PSEUDO_CLASSES)

    const gpio_class_t gpio_object = {
      .init = &gpio_init,
      .write = &gpio_write,
      .toggle = &gpio_toggle,
      .read = &gpio_read
    };
    
    const gpio_class_t* GPIO = &gpio_object;

  #endif

#endif