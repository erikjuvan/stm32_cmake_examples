.section .rodata @progbits
    .global CanOPEN_zipedEDS_File$$Base, @type   CanOPEN_zipedEDS_File$$Base, @object
    .balign 4

CanOPEN_zipedEDS_File$$Base:
#if defined(MOTOR_GMCC_75)
    #if defined(DEBUG)
        .incbin "../../src/Middlewares/Protocol/CANopen/CO_Dictionary/GMCC_75_491/gen_objdict.zip"
    #else
        .incbin "../../src/Middlewares/Protocol/CANopen/CO_Dictionary/GMCC_75/gen_objdict.zip"
    #endif
#elif defined(MOTOR_GMCC_66)
    #if defined(DEBUG)
        .incbin "../../src/Middlewares/Protocol/CANopen/CO_Dictionary/GMCC_66_491/gen_objdict.zip"
    #else
        .incbin "../../src/Middlewares/Protocol/CANopen/CO_Dictionary/GMCC_66/gen_objdict.zip"
    #endif
#elif defined(MOTOR_RECHI_81)
    #if defined(DEBUG)
        .incbin "../../src/Middlewares/Protocol/CANopen/CO_Dictionary/RECHI_81_491/gen_objdict.zip"
    #else
        .incbin "../../src/Middlewares/Protocol/CANopen/CO_Dictionary/RECHI_81/gen_objdict.zip"
    #endif
#else
    #error
#endif

CanOPEN_zipedEDS_File$$Base_end:
    .global CanOPEN_zipedEDS_File$$End, @type   CanOPEN_zipedEDS_File$$End, @object
    .balign 4
CanOPEN_zipedEDS_File$$End:
    .int    CanOPEN_zipedEDS_File$$Base_end - CanOPEN_zipedEDS_File$$Base
