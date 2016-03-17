/*  Nick Sweeting 2014/02/13
    MIT License
*/

#include <stdlib.h>
#include <cstdio>
#include <ctype.h>      // isdigit (parsing for tokens/ops)
#include <cstring>       // duh
#include <iostream>     // cout
#include <stdint.h>
#include <sys/mman.h>
using namespace std;

typedef int(*vfuncp)(void);

int crazy() {
    return 69;
}
void dummy() {}

char shellcode[] = {
    0xb8, 0x2a, 0x00, 0x00, 0x00, //mov    $0x2a,%eax
    0xc3                          //retq
};

int main () {
    // this works

    int test1[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int *newtest3loc = (int *) malloc(sizeof(test1));
    memcpy(newtest3loc, test1, sizeof(test1));
    cout << "memcpy'ed arrays work fine: " << newtest3loc[2] << endl;

    // this too

    uintptr_t pageSize        = 4096;
    uintptr_t shellcodeAddr   = (uintptr_t)shellcode;
    uintptr_t pageAlignedAddr = shellcodeAddr & ~(pageSize-1);
    vfuncp shellcodeFn = (vfuncp)shellcode;

    mprotect((void*)pageAlignedAddr,
             (shellcodeAddr - pageAlignedAddr) + sizeof(shellcode),
             PROT_EXEC|PROT_WRITE|PROT_READ);

    cout << "calling runtime-copied mprotect'ed assembly works fine too: " << shellcodeFn() << endl;


    // what about this?

    void *crazyfuncloc = (void*)&crazy;
    void *dummyfuncloc = (void*)&dummy;

    unsigned long crazy_func_size = (unsigned long)dummyfuncloc - (unsigned long)crazyfuncloc;

    uintptr_t crazy_addr   = (uintptr_t)crazy;
    uintptr_t page_aligned_crazy_addr = crazy_addr & ~(pageSize-1);
    vfuncp crazy_func = (vfuncp)crazy;

    mprotect((void*)page_aligned_crazy_addr,
             (crazy_addr - page_aligned_crazy_addr) + crazy_func_size,
             PROT_EXEC|PROT_WRITE|PROT_READ);

    cout << "calling a runtime-copied mprotect'ed normal function works fine: " << crazy_func() << endl;
}
