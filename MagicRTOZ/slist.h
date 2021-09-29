#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#define LIST_NEW()\
{\
    .$size = 0,\
    .$first_selement = NULL,\
}

#define LIST_ELEMENT_NEW(variable, priority)\
{\
    .$priority = (priority),\
    .$data = (const void*) (variable),\
    .$slist = NULL,\
    .$next = NULL,\
}

#define LIST_FOREACH_INIT(slist, selement, selement_type)\
{\
    for (uint8_t counter = 0 ; counter < slist_size_get(&slist) ; counter++)\
    {\
        selement_type* selement = (selement_type*) slist_position_dataGet(&slist, counter);

#define LIST_FOREACH_END\
    }\
}

/*
    List selement type.
*/
typedef struct selement_t
{
    uint8_t $priority;
    const void* $data;
    struct slist_t* $slist;
    struct selement_t* $next;
}
selement_t;

/*
    List type.
*/
typedef struct slist_t
{
    uint8_t $size;
    selement_t* $first_selement;
}
slist_t;

int8_t slist_selement_insert(slist_t* slist, selement_t* selement);
int8_t slist_selement_delete(selement_t* selement);
int8_t slist_selement_move(selement_t* selement, slist_t* slist);
selement_t* slist_selement_get(slist_t* slist, uint8_t position);
const void* slist_selement_dataGet(selement_t* selement);

int8_t slist_position_delete(slist_t* slist, uint8_t position);
int8_t slist_position_move(slist_t* slistSender, uint8_t position, slist_t* slistReceiver);
const void* slist_position_dataGet(slist_t* slist, uint8_t position);

uint8_t slist_size_get(slist_t* slist);
