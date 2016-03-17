int main(void) {

    int a = 10;
    int b;
            asm ("movl %1, %%eax;
                  movl %%eax, %0;"
                 :"=r"(b)           /* output */
                 :"r"(a)            /* input */
                 :"%eax"            /* clobbered register */
                 );


    return 0;
}
