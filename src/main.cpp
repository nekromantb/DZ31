#include <iostream>
#include <assert.h>
#include "task1.h"
#include "task2.h"

int main() {
    std::cout << "Choose task (1, 2):"<< std::endl;
    int i;
    std::cin >>i;
    assert(i >= 1 && i <=2);
    switch (i)
    {
        case 1:
            task1();
            break;
        case 2:
            task2();
            break;
    }
    return 0;
}
