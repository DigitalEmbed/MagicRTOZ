/*! Inserting and getting list data
 *  \brief      Is used constructor macros and global variables in this example.
 *  \author     Jorge Henrique Moreira Santana
 *  \date       02/10/2021
 *  \warning    Be very careful when using Macros.
 *              Variables and functions starting with the '_' character are private and must not be modified.
 *              All data variables must be global or static.
 *  \copyright  MIT License.
 */

#include <MagicRTOZ.h>

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

__attribute__((weak))
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
        printf("Position %d: %d\n", SLIST_FOREACH_POSITION, *buffer);
    }
    SLIST_FOREACH_END
    return 0;
}