#include "./slist.h"

int8_t slist_selement_insert(slist_t* slist, selement_t* selement)
{
    /*
        Declarations
    */
    selement_t* buffer = slist->$first_selement;
    uint8_t position = 2;

    /*
        Returns error in NULL selement case
    */
    if (selement == NULL || selement->$slist != NULL)
    {
        return -1;
    }

    selement->$slist = (struct slist_t*) slist;
    slist->$size++;
    /*
        First insertion
    */
    if (slist->$first_selement == NULL)
    {
        slist->$first_selement = selement;
        selement->$next = NULL;
        return 0;
    }

    /*
        Insertion in first selement position
    */
    if (slist->$first_selement->$priority > selement->$priority)
    {
        selement->$next = slist->$first_selement;
        slist->$first_selement = selement;
        return 1;
    }

    /*
        Insertion in other positions
    */
    while(buffer->$next != NULL && buffer->$next->$priority <= selement->$priority)
    {
        buffer = buffer->$next;
        position++;
    }
    selement->$next = buffer->$next;
    buffer->$next = selement;
    return position;
}

int8_t slist_selement_delete(selement_t* selement)
{
    slist_t* slist = selement->$slist;
    selement_t* buffer = slist->$first_selement;

    if (selement == NULL || selement->$slist == NULL)
    {
        return -1;
    }

    if (slist->$first_selement == selement)
    {
        slist->$first_selement = selement->$next;
    }
    else
    {
        while (buffer->$next != selement)
        {
            buffer = buffer->$next;
            if (buffer->$next == NULL)
            {
                return -1;
            }
        }
        buffer->$next = selement->$next;
    }

    selement->$slist = NULL;
    slist->$size--;
    return slist->$size;
}

int8_t slist_selement_move(selement_t* selement, slist_t* slist)
{
    if (slist_selement_delete(selement) != -1)
    {
        return slist_selement_insert(slist, selement);
    }
    return -1;
}

int8_t slist_position_delete(slist_t* slist, uint8_t position)
{
    /*
        Returns error in NULL slist or invalid position cases
    */
    if (slist == NULL || position >= slist->$size)
    {
        return -1;
    }

    selement_t* buffer = slist->$first_selement;
    buffer->$slist = NULL;
    slist->$size--;

    if (position == 0)
    {
        slist->$first_selement = buffer->$next;
    }
    else
    {
        /*
            Searching position selement
        */
        for (uint8_t counter = 0 ; counter < position - 1 ; counter ++)
        {
            buffer = buffer->$next;
            if (counter == position)
            {
                break;
            }
        }
        buffer->$next = buffer->$next->$next;
    }

    return slist->$size;
}

int8_t slist_position_move(slist_t* slistSender, uint8_t position, slist_t* slistReceiver)
{
    selement_t* selement_buffer = slist_selement_get(slistSender, position);
    if (selement_buffer != NULL && slist_position_delete(slistSender, position) != -1)
    {
        return slist_selement_insert(slistReceiver, selement_buffer);
    }
    return -1;
}

selement_t* slist_selement_get(slist_t* slist, uint8_t position)
{
    /*
        Declarations
    */
    selement_t* buffer = slist->$first_selement;

    /*
        Returns error in NULL slist or invalid position cases
    */
    if (slist == NULL || position >= slist->$size)
    {
        return NULL;
    }

    /*
        Searching position selement
    */
    for (uint8_t counter = 0 ; counter < position ; counter ++)
    {
        buffer = buffer->$next;
        if (counter == position)
        {
            break;
        }
    }

    return buffer;
}

uint8_t slist_size_get(slist_t* slist){
    return slist->$size;
}

const void* slist_selement_dataGet(selement_t* selement)
{
    if (selement != NULL)
    {
        return selement->$data;
    }
    return NULL;
}

const void* slist_position_dataGet(slist_t* slist, uint8_t position)
{
    if (slist != NULL)
    {
        return slist_selement_get(slist, position)->$data;
    }
    return NULL;
}
