

#include "common.h"

#define BLDR_INIT_SECTION      __attribute__ ((section(".bldrinit")))
#define BOOTLOADER_SECTION     __attribute__ ((section (".bootloader")))

#define XJMP  "jmp"

/* Bootloader Vektoren und Bootloader Startup Code
 */
BLDR_INIT_SECTION __ATTR_NAKED __ATTR_USED
static void bldr_boot(void) {    
    __asm__ __volatile__ (
        ".macro bvector name\n"
        ".weak   \\name\n"
        ".set    \\name, __bldr_bad_interrupt\n" // default da weak gebunden
        XJMP " \\name\n" // springe zu Vector
        ".endm\n"
     "__bldr_vectors:\n"
        XJMP " __bldr_init\n"
        "bvector __vector_1\n"
        "bvector __vector_2\n"
        "bvector __vector_3\n"
        "bvector __vector_4\n"
        "bvector __vector_5\n"
        "bvector __vector_6\n"
        "bvector __vector_7\n"
        "bvector __vector_8\n"
        "bvector __vector_9\n"
        "bvector __vector_10\n"
        "bvector __vector_11\n"
        "bvector __vector_12\n"
        "bvector __vector_13\n"
        "bvector __vector_14\n"
        "bvector __vector_15\n"
        "bvector __vector_16\n"
        "bvector __vector_17\n"
        "bvector __vector_18\n"
        "bvector __vector_19\n"
        "bvector __vector_20\n"
        "bvector __vector_21\n"
        "bvector __vector_22\n"
        "bvector __vector_23\n"
        "bvector __vector_24\n"
        "bvector __vector_25\n"
        "bvector __vector_26\n"
        "bvector __vector_27\n"
        "bvector __vector_28\n"
        "bvector __vector_29\n"
        "bvector __vector_30\n"
        "bvector __vector_31\n"
        "bvector __vector_32\n"
        "bvector __vector_33\n"
        "bvector __vector_34\n"
        "bvector __vector_35\n"
        "bvector __vector_36\n"
        "bvector __vector_37\n"
        "bvector __vector_38\n"
        "bvector __vector_39\n"
        "bvector __vector_40\n"
        "bvector __vector_41\n"
        "bvector __vector_42\n"
        "bvector __vector_43\n"
        "bvector __vector_44\n"
        "bvector __vector_45\n"
        "bvector __vector_46\n"
        "bvector __vector_47\n"
        "bvector __vector_48\n"
        "bvector __vector_49\n"
        "bvector __vector_50\n"
        "bvector __vector_51\n"
        "bvector __vector_52\n"
        "bvector __vector_53\n"
        "bvector __vector_54\n"
        "bvector __vector_55\n"
        "bvector __vector_56\n"
        "bvector __vector_57\n"
        "bvector __vector_58\n"
        "bvector __vector_59\n"
        "bvector __vector_60\n"
        "bvector __vector_61\n"
        "bvector __vector_62\n"
        "bvector __vector_63\n"
        "bvector __vector_64\n"
        "bvector __vector_65\n"
        "bvector __vector_66\n"
        "bvector __vector_67\n"
        "bvector __vector_68\n"
        "bvector __vector_69\n"
        "bvector __vector_70\n"
        "bvector __vector_71\n"
        "bvector __vector_72\n"
        "bvector __vector_73\n"
        "bvector __vector_74\n"
        "bvector __vector_75\n"
        "bvector __vector_76\n"
        "bvector __vector_77\n"
        "bvector __vector_78\n"
        "bvector __vector_79\n"
        "bvector __vector_80\n"
        "bvector __vector_81\n"
        "bvector __vector_82\n"
        "bvector __vector_83\n"
        "bvector __vector_84\n"
        "bvector __vector_85\n"
        "bvector __vector_86\n"
        "bvector __vector_87\n"
        "bvector __vector_88\n"
        "bvector __vector_89\n"
        "bvector __vector_90\n"
        "bvector __vector_91\n"
        "bvector __vector_92\n"
        "bvector __vector_93\n"
        "bvector __vector_94\n"
    "__bldr_bad_interrupt:\n"
        "reti\n"
    ".globl __bldr_init\n"
    "__bldr_init:\n"
        "clr   __zero_reg__\n" // __zero_reg__ to 0
       : : : "memory"
    );

    // cli Interrupts löschen
    SREG = 0;     // cli
    // Stackpointer Initialiseren
    SP = BLDR_STACK_TOP; // INTERNAL_SRAM_END - sizeof(struct bldr_ram) - 1
    
    // RAM löschen (und somit Stackpointer und bldr_ram)
    uint8_t* p = (uint8_t*)INTERNAL_SRAM_START;
    do {
        *p++ = 0;
    } while (p != (uint8_t*)INTERNAL_SRAM_END);

    // springe zu bldr_start()
    __asm__ __volatile__ (
        XJMP " bldr_start\n"
    );
}
