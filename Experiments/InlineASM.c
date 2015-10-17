#include <stdio.h>
#simdi
#64 bit register scheme

int output_a, output_b, input_a, input_b;

void resetvals() {
    output_a = -1;   // if you see -1 in the final program output, you know tha assembly did nothing
    output_b = -1;
    input_a = 0;
    input_b = 0;
}

void printout() {
    printf(" input_a=%i,  input_b=%i\noutput_a=%i, output_b=%i\n\n", input_a, input_b, output_a, output_b);
}

int main(void) {

    /*  movl $foo,%eax puts the address of variable foo into register %eax, but movl foo,%eax puts the contents of variable foo into register %eax.
        There are two %’s prefixed to the register name. This helps GCC to distinguish between the operands and registers. operands have a single % as prefix.

        The clobbered register %eax after the third colon tells GCC that the value of %eax is to be modified inside "asm", so GCC won’t use this register to store any other value.

        +---+--------------------+
        | r |    Register(s)     |
        +---+--------------------+
        | a |   %eax, %ax, %al   |
        | b |   %ebx, %bx, %bl   |
        | c |   %ecx, %cx, %cl   |
        | d |   %edx, %dx, %dl   |
        | S |   %esi, %si        |
        | D |   %edi, %di        |
        +---+--------------------+
    */

    resetvals();
    input_a=2, input_b=3;
    __asm__(
        "movl %0, %2;"              // move input_1 to output_1
        "movl %1, %3;"              // move input_2 to output_2

        :"=r" (output_a) , "=i" (output_b)                      /* output */
        :"r"  (input_a)  ,  "0" (input_b)                       /* input */
        :"%eax"
    );
    printout();

    resetvals();
    input_a=5, input_b=4;
    __asm__(
        "leal (%0,%0,1), %2;"

        :"=r" (output_a)                                        /* output */
        :"r"  (input_a)  ,  "r" (input_b)                       /* input */
        :"%eax"                                                 /* clobbered register */
    );
    printout();

    return 0;
}





/*

balancing the stacjk = pushing and popping in reverse the proper number of pointers

*/
