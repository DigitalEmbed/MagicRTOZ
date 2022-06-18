#include "./atmega328.h"
#if ((defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__)) && (ENABLE_HTIMER == 1))

    #include "./MagicRTOZ.h"
    #include <stdio.h>

    typedef enum 
    { 
        WGMn0 = 0,
        WGMn1,
        COMnC0,
        COMnC1,
        COMnB0,
        COMnB1,
        COMnA0,
        COMnA1
    } 
    tccra_bits_t;

    typedef enum
    {
        CSn0 = 0,
        CSn1,
        CSn2,
        WGMn2,
        WGMn3,
        ICESn = 6,
        ICNCn
    }
    tccrb_bits_t;

    typedef enum 
    {
        FOCnC = 5,
        FOCnB,
        FOCnA
    }
    tccrc_bits_t;

    typedef enum
    {
        TOIEn = 0,
        OCIEnA,
        OCIEnB,
        OCIEnC,
        ICIEn = 5
    }
    timsk_bits_t;

    typedef enum
    {
        TOVn = 0,
        OCFnA,
        OCFnB,
        OCFnC,
        ICFn = 5
    }
    tifr_bits_t;

    typedef enum
    {
        HTIMER_PS_TURNED_OFF = 0,
        HTIMER_PS_1,
        HTIMER_PS_8,
        HTIMER_PS_32,
        HTIMER_PS_64,
        HTIMER_PS_128,
        HTIMER_PS_256,
        HTIMER_PS_1024,
        HTIMER_EXTERNAL_CLOCK_RISING_EDGE,
        HTIMER_EXTERNAL_CLOCK_DESCENT_EDGE,
    }
    htimer_config_t;

    typedef struct
    {
        volatile uint8_t TCCRA;
        volatile uint8_t TCCRB;
        volatile uint8_t TCCRC;
        volatile uint8_t const reserved;
        
        union 
        {
            volatile uint8_t array[2];
            volatile uint16_t data;
        } 
        TCNT;
        
        union 
        {
            volatile uint8_t array[2];
            volatile uint16_t data;
        }
        ICR;
        
        union
        {
            volatile uint8_t array[2];
            volatile uint16_t data;
        }
        OCRA;
        
        union
        {
            volatile uint8_t array[2];
            volatile uint16_t data;
        }
        OCRB;

        union
        {
            volatile uint8_t array[2];
            volatile uint16_t data;
        }
        OCRC;
    } 
    _htimer_1_t;

    typedef struct
    {
        volatile uint8_t TIMSK;
        volatile uint8_t const reserved[0x6E - 0x15 - 0x01];
        volatile uint8_t TIFR;   
    }
    _htimer_2_t;

    struct _htimer_t
    {
        _htimer_1_t* registers_1;
        _htimer_2_t* registers_2;
        bool in_use;
        htimer_t* htimer;
    };

    struct _htimer_t preallocated_htimers_array[] = {
        {
            .registers_1 = (_htimer_1_t*) &TCCR0A,
            .registers_2 = (_htimer_2_t*) &TIFR0,
            .in_use = false,
            .htimer = NULL,
        },
        {
            .registers_1 = (_htimer_1_t*) &TCCR1A,
            .registers_2 = (_htimer_2_t*) &TIFR1,
            .in_use = false,
            .htimer = NULL,
        },
    };

    int8_t htimer_install(htimer_t* htimer, uint8_t priority, uint32_t time_waiting_ms, htimer_run_mode_t run_mode)
    {
    }

    void htimer_suspend(htimer_t* htimer)
    {

    }

    void htimer_resume(htimer_t* htimer)
    {

    }

    void htimer_setPriority(htimer_t* htimer, uint8_t priority)
    {

    }
    
    void htimer_setWaitingTimer(htimer_t* htimer, uint32_t time_waiting_ms)
    {

    }

    const char* htimer_getName(htimer_t* htimer)
    {
        
    }

#endif