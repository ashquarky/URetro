/* Most of this is nicked from HBL */

/* Read a 32-bit word with kernel permissions */
unsigned int __attribute__ ((noinline)) kern_read(const void *addr)
{
    unsigned int result;
    asm volatile (
        "li 3,1\n"
        "li 4,0\n"
        "li 5,0\n"
        "li 6,0\n"
        "li 7,0\n"
        "lis 8,1\n"
        "mr 9,%1\n"
        "li 0,0x3400\n"
        "mr %0,1\n"
        "sc\n"
        "nop\n"
        "mr 1,%0\n"
        "mr %0,3\n"
        :	"=r"(result)
        :	"b"(addr)
        :	"memory", "ctr", "lr", "0", "3", "4", "5", "6", "7", "8", "9", "10",
            "11", "12"
    );

    return result;
}

/* Write a 32-bit word with kernel permissions */
void __attribute__ ((noinline)) kern_write(void *addr, unsigned int value)
{
    asm volatile (
        "li 3,1\n"
        "li 4,0\n"
        "mr 5,%1\n"
        "li 6,0\n"
        "li 7,0\n"
        "lis 8,1\n"
        "mr 9,%0\n"
        "mr %1,1\n"
        "li 0,0x3500\n"
        "sc\n"
        "nop\n"
        "mr 1,%1\n"
        :
        :	"r"(addr), "r"(value)
        :	"memory", "ctr", "lr", "0", "3", "4", "5", "6", "7", "8", "9", "10",
            "11", "12"
        );
}

#if ( (VER == 532) || (VER == 540) )
    #define KERN_SYSCALL_TBL_2                          0xFFE85070 // works with games
#elif ( (VER == 500) || (VER == 510) )
    #define KERN_SYSCALL_TBL_2                          0xFFE85070 // works with games
#elif (VER == 550)
    #define KERN_SYSCALL_TBL_2                          0xFFE85070 // works with games
#elif (VER == 410)
    #define KERN_SYSCALL_TBL_2                          0xFFE85090
#elif (VER == 400)
    #define KERN_SYSCALL_TBL_2                          0xFFE85090
#elif ( (VER == 300) || (VER == 310) )
    #define KERN_SYSCALL_TBL_2                          0xFFE85150
#else
    #error Please define valid values for firmware.
#endif

static void InjectSyscall36(unsigned int address)
{
    // Add syscall #0x36
    kern_write((void*)(KERN_SYSCALL_TBL_2 + (0x36 * 4)), address);
}

//bat.S
extern void RunSyscall36();
extern void injectBAT();
extern void clearBAT();