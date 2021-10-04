/*! Static list API
 *  \brief      Linked lists with priority classification API.
 *  \details    The linked lists generated with this API not use memory allocations.
 *              This method it's advisable if you have arrays that don't have a certain size, but that won't change their size during the program.
 *  \example    1 - Inserting and getting list data:
 *  \code{.c}
    int myData1 = 10;

    // This macro is used when you need to initialize this
    // object outside of function scopes.
    selement_t myDataElement1 = SLIST_ELEMENT_NEW(
        &myData1,                                       // Data pointer
        1                                               // Priority
    );

    int myData2 = -5;
    selement_t myDataElement2 = SLIST_ELEMENT_NEW(&myData2, 0);

    // This macro is used when you need to initialize this
    // object outside of function scopes.
    slist_t myList = SLIST_NEW();

    int main(){
        int position1 = slist_selement_insert(&myList, &myDataElement1);
        int position2 = slist_selement_insert(&myList, &myDataElement2);

        // For-each slist macro.
        SLIST_FOREACH_INIT(
            myList,                 // List
            buffer,                 // Data element pointer buffer
            int                     // Data element type
        )
        {
            printf("Position %d: %d\n", LIST_FOREACH_POSITION, *buffer);
        }
        SLIST_FOREACH_END
        return 0;
    }
 *  \endcode
 *  \example    2 - Moving local data into static lists.
 *  \code{.c}
 *  int main()
    {
        // All variables must be global or static!
        static slist_t listA;
        static slist_t listB;
        static float pi = 3.14;
        static float euler = 2.71;
        static selement_t pi_element;
        static selement_t euler_element;

        // List constructor.
        if
        (
            slist_create(&listA) == -1 ||
            slist_create(&listB) == -1
        )
        {
            printf("Error in slist instance!\n");
            return -1;
        }

        // Element constructor.
        if
        (
            slist_selement_create(&pi_element, &pi, 0) == -1 ||
            slist_selement_create(&euler_element, &euler, 0) == -1
        )
        {
            printf("Error in selement instance!\n");
            return -1;
        }

        slist_selement_insert(&listA, &pi_element);
        slist_selement_insert(&listA, &euler_element);

        // Printing all listA elements.
        if (slist_size_get(&listA) > 0)
        {
            printf("listA:\n");
            SLIST_FOREACH_INIT(listA, buffer, float)
            {
                printf
                (
                    " [%d] - %f\n",
                    LIST_FOREACH_POSITION,
                    (float) *buffer
                );
            }
            SLIST_FOREACH_END
        }
        else
        {
            printf("listA: None\n");
        }

        // Printing all listB elements.
        if (slist_size_get(&listB) > 0)
        {
            printf("\nlistB:\n");
            SLIST_FOREACH_INIT(listB, buffer, float)
            {
                printf
                (
                    " [%d] - %f\n",
                    LIST_FOREACH_POSITION,
                    (float) *buffer
                );
            }
            SLIST_FOREACH_END
        }
        else
        {
            printf("\nlistB: None\n");
        }

        printf("\nMoving listA data to listB...");

        // Moving an element to another list by selement pointer.
        slist_selement_move(&pi_element, &listB);

        // Moving an element to another list by position.
        slist_position_move(&listA, 0, &listB);

        printf(" OK!\n");

        // Printing all listA elements.
        if (slist_size_get(&listA) > 0)
        {
            printf("\nlistA:\n");
            SLIST_FOREACH_INIT(listA, buffer, float)
            {
                printf
                (
                    " [%d] - %f\n",
                    LIST_FOREACH_POSITION,
                    (float) *buffer
                );
            }
            SLIST_FOREACH_END
        }
        else
        {
            printf("\nlistA: None\n");
        }

        // Printing all listB elements.
        if (slist_size_get(&listB) > 0)
        {
            printf("\nlistB:\n");
            SLIST_FOREACH_INIT(listB, buffer, float)
            {
                printf
                (
                    " [%d] - %f\n",
                    LIST_FOREACH_POSITION,
                    (float) *buffer
                );
            }
            SLIST_FOREACH_END
        }
        else
        {
            printf("\nlistB: None\n");
        }
    }
 *  \endcode
 *  \author     Jorge Henrique Moreira Santana
 *  \version    0.1a
 *  \date       02/10/2021
 *  \bug        This API needs more testing to find bugs.
 *  \warning    Be very careful when using Macros.
 *              Variables and functions starting with the '$' character are private and must not be modified.
 *              All data variables must be global or static.
 *  \copyright  MIT License.
 */

#pragma once

#ifdef __cplusplus
    extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

/*! \def SLIST_NEW()
    \brief List constructor.

    \return List object.
*/
#define SLIST_NEW()\
{\
    .$size = 0,\
    .$first_selement = NULL,\
}

/*! \def SLIST_ELEMENT_NEW(variable, priority)
    \brief Element constructor.

    \param data_pointer Static or global variable pointer.
    \param priority Element priority.

    \return Element object.
*/
#define SLIST_ELEMENT_NEW(data_pointer, priority)\
{\
    .$priority = (priority),\
    .$data = (const void*) (data_pointer),\
    .$slist = NULL,\
    .$next = NULL,\
}

/*! \def SLIST_FOREACH_INIT(slist, selement, selement_type)
    \brief Init of for-each list loop structure.

    \param slist Static list.
    \param selement Element buffer.
    \param selement_type Element type.
*/
#define SLIST_FOREACH_INIT(slist, selement, selement_type)\
{\
    for (uint8_t $counter = 0 ; $counter < slist_size_get(&slist) ; $counter++)\
    {\
        uint8_t LIST_FOREACH_POSITION = $counter;\
        selement_type* selement = (selement_type*) slist_position_dataGet(&slist, $counter);

/*! \def LIST_FOREACH_END
    \brief End of for-each list loop structure.
*/
#define SLIST_FOREACH_END\
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

/*! \def int8_t slist_create(slist_t* slist)
    \brief List constructor.

    \param slist List pointer.

    \return 1 in successful case or -1 in error cases.
*/
int8_t slist_create(slist_t* slist);

/*! \def int8_t slist_selement_create(selement_t* selement, void* data, uint8_t priority)
    \brief Element constructor.

    \param selement Element pointer.
    \param data Static or global data pointer.
    \param priority Element priority.

    \return Element object.
*/
int8_t slist_selement_create(selement_t* selement, const void* data, uint8_t priority);

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
