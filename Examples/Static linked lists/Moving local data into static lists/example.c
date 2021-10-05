/*! Inserting and getting list data
 *  \brief      Is used constructor functions and local variables in this example.
 *  \author     Jorge Henrique Moreira Santana
 *  \date       02/10/2021
 *  \warning    Be very careful when using Macros.
 *              Variables and functions starting with the '$' character are private and must not be modified.
 *              All data variables must be global or static.
 *  \copyright  MIT License.
 */

#include <MagicRTOZ.h>

__attribute__((weak))
int main()
{
    // All local variables must be static!
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