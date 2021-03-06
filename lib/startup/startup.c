/// \file
/// \brief code to initialize the c library when using gcc
/// NOTE: This code does not properly call _start() from newlib,
/// instead it is modeled off the example from code composer studio.
/// This thread has more information https://e2e.ti.com/support/microcontrollers/other/f/908/t/479812?CCS-GNU-C-Compiler-and-Linker-Setup
/// This likely means the code won't work with c++
#include<stddef.h>

// defined in linker script indicating location of various segments
extern unsigned int  __data_load__;
extern unsigned int __data_start__;
extern unsigned int __data_end__;
extern unsigned int __bss_start__;
extern unsigned int __bss_end__;
extern unsigned int __STACK_END;

// the main function
extern void main(void);

// called on a reset. Do not emit prologue in assembly since the stack is not
// yet valid
__attribute__((naked, used)) void _c_int00(void)
{
    // set up the stack
    __asm__ volatile ("MSR msp, %0\n" : : "r" (&__STACK_END));

    unsigned int *source = &__data_load__ ;
    unsigned int *destination = &__data_start__;

    // copy the statically initialized data from flash to RAM
    while(destination != &__data_end__)
    {
        *destination++ = *source++;
    }

    // Zero fill the bss segment with 0's
    destination = &__bss_start__;
    while(destination != &__bss_end__)
    {
        *destination++ = 0;
    }

// from ACLE (arm c library extentions) only enable floating point if
// float support is enabled by the compiler
#if defined(__ARM_FP)    
    // Enable the floating-point unit.  This must be done here to handle the
    // case where main() uses floating-point and the function prologue saves
    // floating-point registers (which will fault if floating-point is not
    // enabled).
    // See 3.1.5.7 in the datasheet for example code and explanation
    // basically it's FPUEnable() from tivaware

    // the Coprocessor Access Control Register (CPAC)
    volatile unsigned int * CPAC = (unsigned int *)0xE000ED88;

    // set the CP11 and CP10 bits to all 1's to enable the FPU. leave other bits unchanged
    *CPAC = (*CPAC & ~0x00F00000) | 0x00F00000;

    // reset the pipeline with some barrier/synchronization instructions (see 3.1.5.7 on the FPU).
    // Might be unnecessary, but can't hurt
    __asm__ volatile ("DSB\n"
                      "ISB");
#endif
    main(); // call main
}
