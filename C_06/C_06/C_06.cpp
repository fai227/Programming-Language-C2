#include <iostream>
#include "Struct.h"


int main()
{
    
    struct Player players[] = {
        { "Tom", 1, 5 },
        { "John", 2, 10},
        { "Sam", 3, 7 }
    };

    Team american = { "Team American", sum(players, 3), players };

    fprintf_s(stdout, "Team Point is %d", american.point);
    
}