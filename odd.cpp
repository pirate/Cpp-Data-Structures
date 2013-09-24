
#include <stdlib.h>
#include <iostream>

using namespace std;

int main () {
    int ints[10] = {0,1,2,3,4,5,6,7,8,9};
    cout << 7[ints] << endl;
    cout << ints[8] << endl;
    // ints[7] is shorthand for *(ints+7)
    // so 7[ints] works because *(7+ints) is the same as *(ints+7)
}
