#include <iostream>
#include "PString.h"
#include <cstring>
#include <cstdlib>
#include "Sort.h"

using namespace std;
using namespace PrideShenLib;

int main()
{
    int array[] = {7, 3, 1, 6, 2, 8, 5, 4, 9};

    Sort::Quick(array, 10, true);

    for(int i=0; i<10; i++)
    {
        cout << array[i] << endl;
    }

    return 0;

}
