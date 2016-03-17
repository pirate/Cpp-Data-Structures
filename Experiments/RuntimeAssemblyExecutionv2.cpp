/*  Nick Sweeting 2014/09/26
    Runtime function copying and execution
    MIT License
*/

#include <stdlib.h>     // duh
#include <iostream>     // cout
#include <sys/mman.h>   // memcpy, mprotect
using namespace std;

const unsigned long page_size = 4096;

int func(int a, int b) {return a+b;}
void dummy() {}
// assuming functions are sequential in memory: func_size = &dummy - &func

typedef int(*funcp)(int, int);

int main () {
    unsigned long func_size = (unsigned long)&dummy - (unsigned long)&func;     // calculate func size at runtime
    
    unsigned long newmem_addr = (unsigned long) malloc(func_size);              // allocate new memory to copy function into
    unsigned long page_aligned_newmem_addr = newmem_addr & ~(page_size-1);      // calculate page_aligned address (memcpy takes normal addrs, but mprotect takes page_aligned addrs)

    memcpy((void*)newmem_addr,                  // to   addr                    // copy the function to the new malloc'ed memory block
           (void*)&func,                        // from addr
           func_size);                                                          cout << "1. memcpy'ed " << func_size << " byte function from @" << (unsigned long)&func << " to @" << newmem_addr << endl;

    funcp copied_func = (funcp)newmem_addr;                                     // create a func pointer to the function in the new location

    mprotect((void*)page_aligned_newmem_addr,   // from addr                    // add execute access to the malloc'ed memory block holding the copied function
             func_size,                         // # of bytes
             PROT_EXEC);                                                        cout << "2. mprotect +PROT_EXEC'ed " << func_size << " bytes of memory from @" << newmem_addr << " through @" << newmem_addr+func_size << endl;

    int result = (*copied_func)(40,2);                                          cout << "3. Result of executing copied function in new location: " << result << endl;
}

    // I dont know the difference, but this and the version above both work:

    // mprotect((void*)page_aligned_newmem_addr,
    //          (newmem_addr-page_aligned_newmem_addr)+func_size,
    //          PROT_EXEC);
