/*! Static list API
 *  \brief      Linked lists with priority classification that not use memory allocations API.
 *  \details    This class is used to demonstrate a number of section commands.
 *  \example    Inserting and getting list data.
 *  \code{.c}
 *  int main(){
 *      static int myData1 = 10;
 *      static selement_t myDataElement1 = LIST_ELEMENT_NEW(
 *          &myData1,               // Data pointer
 *          1                       // Priority
 *      );
 *
 *      static int myData2 = -5;
 *      static selement_t myDataElement2 = LIST_ELEMENT_NEW(&myData2, 0);
 *
 *      slist_t myList = LIST_NEW();
 *      int position1 = slist_selement_insert(&myList, &myDataElement1);
 *      int position2 = slist_selement_insert(&myList, &myDataElement2);
 *
 *      LIST_FOREACH_INIT(
 *          myList,                 // List
 *          buffer,                 // Data element pointer buffer
 *          int                     // Data element type
 *      )
 *      {
 *          printf("Position %d: %d\n", LIST_FOREACH_POSITION, *buffer);
 *      }
 *      LIST_FOREACH_END
 *
 *      return 0;
 *  }
 *  \endcode
 *  \author     Jorge Henrique Moreira Santana
 *  \version    0.1a
 *  \date       02/10/2021
 *  \bug        This API needs more testing to find bugs.
 *  \warning    Be very careful when using Macros. Variables and functions starting with the '$' character are private and must not be modified.
 *  \copyright  MIT License.
 */

#pragma once

#ifdef __cplusplus
    extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

/*! \def LIST_NEW()
    \brief List constructor.

    \return List object.
*/
#define LIST_NEW()\
{\
    .$size = 0,\
    .$first_selement = NULL,\
}

/*! \def LIST_ELEMENT_NEW(variable, priority)
    \brief Element constructor.

    \param variable_pointer Static or global variable pointer.
    \param priority Element priority.

    \return Element object.
*/
#define LIST_ELEMENT_NEW(variable_pointer, priority)\
{\
    .$priority = (priority),\
    .$data = (const void*) (variable_pointer),\
    .$slist = NULL,\
    .$next = NULL,\
}

/*! \def LIST_FOREACH_INIT(slist, selement, selement_type)
    \brief Init of for-each list loop structure.

    \param slist Static list.
    \param selement Element buffer.
    \param selement_type Element type.
*/
#define LIST_FOREACH_INIT(slist, selement, selement_type)\
{\
    for (uint8_t $counter = 0 ; $counter < slist_size_get(&slist) ; $counter++)\
    {\
        uint8_t LIST_FOREACH_POSITION = $counter;\
        selement_type* selement = (selement_type*) slist_position_dataGet(&slist, $counter);

/*! \def LIST_FOREACH_END
    \brief End of for-each list loop structure.
*/
#define LIST_FOREACH_END\
    }\
}

/*! \struct selement_t
 *  \brief Static list element type.
 */
typedef struct selement_t
{
    uint8_t $priority;                      //!< Priority value.
    const void* $data;                      //!< Data pointer.
    struct slist_t* $slist;                 //!< List where the element is contained.
    struct selement_t* $next;               //!< Link to next element in the list.
}
selement_t;

/*! \struct slist_t
 *  \brief Static list type.
 */
typedef struct slist_t
{
    uint8_t $size;                          //!< Size of list.
    selement_t* $first_selement;            //!< First element pointer.
}
slist_t;

/*! \fn int8_t slist_selement_insert(slist_t* slist, selement_t* selement)
    \brief Inserts an element on a list.

    \param slist List pointer.
    \param selement Element pointer.

    \return List element position or -1 if an error happens.
*/
int8_t slist_selement_insert(slist_t* slist, selement_t* selement);

/*! \fn int8_t slist_selement_delete(selement_t* selement)
    \brief Deletes an element of a list.

    \param selement Element pointer.

    \return List size or -1 if an error happens.
*/
int8_t slist_selement_delete(selement_t* selement);

/*! \fn int8_t slist_selement_move(selement_t* selement, slist_t* slist)
    \brief Moves an element on a list.

    \param slist List pointer.
    \param selement Element pointer.

    \return List element position or -1 if an error happens.
*/
int8_t slist_selement_move(selement_t* selement, slist_t* slist);

/*! \fn selement_t* slist_selement_get(slist_t* slist, uint8_t position)
    \brief Gets an element of a list.

    \param slist List pointer.
    \param position List element position.

    \return List element pointer or NULL if an error happens.
*/
selement_t* slist_selement_get(slist_t* slist, uint8_t position);

/*! \fn const void* slist_selement_dataGet(selement_t* selement)
    \brief Gets a data element of a list.

    \param selement Element pointer.

    \return List element pointer or NULL if an error happens.
*/
const void* slist_selement_dataGet(selement_t* selement);

/*! \fn int8_t slist_position_delete(slist_t* slist, uint8_t position)
    \brief Deletes an element of a list.

    \param slist List pointer.
    \param position List element position.

    \return List size or -1 if an error happens.
*/
int8_t slist_position_delete(slist_t* slist, uint8_t position);

/*! \fn int8_t slist_position_move(slist_t* slistSender, uint8_t position, slist_t* slistReceiver)
    \brief Transfers an element from one list to another.

    \param slistSender Sender list pointer.
    \param position List element position.
    \param slistReceiver Receiver list pointer.

    \return List element position or -1 if an error happens.
*/
int8_t slist_position_move(slist_t* slistSender, uint8_t position, slist_t* slistReceiver);

/*! \fn const void* slist_position_dataGet(slist_t* slist, uint8_t position)
    \brief Gets a data element of a list.

    \param slist List pointer.
    \param position List element position.

    \return List element pointer or NULL if an error happens.
*/
const void* slist_position_dataGet(slist_t* slist, uint8_t position);

/*! \fn uint8_t slist_size_get(slist_t* slist)
    \brief Gets list size.

    \param slist List pointer.

    \return List size.
*/
uint8_t slist_size_get(slist_t* slist);

#ifdef __cplusplus
    }
#endif
