#include "./slist.h"

int8_t slist_create(slist_t* slist)
{
    /*
     * Checking if it's safe to instantiate the object.
     */
    if (slist != NULL && slist->_first_selement == NULL)
    {
        slist->_size = 0;
        return 1;
    }
    return -1;
}

int8_t slist_selement_create(selement_t* selement, const void* data, uint8_t priority)
{
    /*
     * Checking if it's safe to instantiate the object.
     */
    if (data != NULL && selement != NULL && selement->_slist == NULL && selement->_next == NULL)
    {
        selement->_data = data;
        selement->_priority = priority;
        selement->_slist = NULL;
        return 1;
    }
    return -1;
}

int8_t slist_selement_insert(slist_t* slist, selement_t* selement)
{
    selement_t* buffer = slist->_first_selement;
    uint8_t position = 2;

    /*
     * Checking errors in function parameters.
     */
    if (selement == NULL || selement->_slist != NULL)
    {
        return -1;
    }

    /*
     * First insertion
     */
    selement->_slist = (struct slist_t*) slist;
    slist->_size++;
    if (slist->_first_selement == NULL)
    {
        selement->_slist = slist;
        slist->_first_selement = selement;
        selement->_next = NULL;
        return 0;
    }

    /*
     * Insertion in first element position.
     */
    if (slist->_first_selement->_priority > selement->_priority)
    {
        selement->_slist = slist;
        selement->_next = slist->_first_selement;
        slist->_first_selement = selement;
        return 1;
    }

    /*
     * Insertion in other positions.
     */
    while(buffer->_next != NULL && buffer->_next->_priority <= selement->_priority)
    {
        buffer = buffer->_next;
        position++;
    }
    selement->_slist = slist;
    selement->_next = buffer->_next;
    buffer->_next = selement;
    return position;
}

int8_t slist_selement_delete(selement_t* selement)
{
    slist_t* slist = selement->_slist;
    selement_t* buffer = slist->_first_selement;

    /*
     * Checking errors in function parameters.
     */
    if (selement == NULL || selement->_slist == NULL)
    {
        return -1;
    }

    if (slist->_first_selement == selement)
    {
        slist->_first_selement = selement->_next;
    }
    else
    {
        while (buffer->_next != selement)
        {
            buffer = buffer->_next;
            if (buffer->_next == NULL)
            {
                return -1;
            }
        }
        buffer->_next = selement->_next;
    }

    selement->_slist = NULL;
    selement->_next = NULL;
    slist->_size--;
    return slist->_size;
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
     * Checking errors in function parameters.
     */
    if (slist == NULL || position >= slist->_size)
    {
        return -1;
    }

    selement_t* buffer = slist->_first_selement;
    buffer->_slist = NULL;
    slist->_size--;

    if (position == 0)
    {
        slist->_first_selement->_slist = NULL;
        slist->_first_selement = buffer->_next;
    }
    else
    {

        /*
         * Searching position element.
         */
        for (uint8_t counter = 0 ; counter < position - 1 ; counter ++)
        {
            buffer = buffer->_next;
            if (counter == position)
            {
                break;
            }
        }
        buffer->_next->_slist = NULL;
        buffer->_next = buffer->_next->_next;
    }

    return slist->_size;
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
    selement_t* buffer = slist->_first_selement;

    /*
     * Checking errors in function parameters.
     */
    if (slist == NULL || position >= slist->_size)
    {
        return NULL;
    }

    /*
     * Searching position element.
     */
    for (uint8_t counter = 0 ; counter < position ; counter ++)
    {
        buffer = buffer->_next;
        if (counter == position)
        {
            break;
        }
    }

    return buffer;
}

uint8_t slist_size_get(slist_t* slist){
    return slist->_size;
}

const void* slist_selement_dataGet(selement_t* selement)
{
    if (selement != NULL)
    {
        return selement->_data;
    }
    return NULL;
}

const void* slist_position_dataGet(slist_t* slist, uint8_t position)
{
    if (slist != NULL)
    {
        return slist_selement_get(slist, position)->_data;
    }
    return NULL;
}
