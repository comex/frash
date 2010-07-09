.globl ___aeabi_ldivmod
___aeabi_ldivmod:
        stmfd sp!,    {r4, r5, r6, r7, r8, lr}
        mov     r5, r2
        mov     r6, r3
        mov     r7, r0
        mov     r8, r1
        bl      ___divdi3
        mul     r2, r5, r1
        umull   r3, r4, r5, r0
        mla     ip, r0, r6, r2
        add     r4, ip, r4
        subs    r2, r7, r3
        sbc     r3, r8, r4
        ldmfd sp!,     {r4, r5, r6, r7, r8, lr}
        bx      lr

.globl ___aeabi_uldivmod
___aeabi_uldivmod:
        stmfd sp!,    {r4, r5, r6, r7, r8, lr}
        mov     r5, r2
        mov     r6, r3
        mov     r7, r0
        mov     r8, r1
        bl      ___udivdi3
        mul     r2, r5, r1
        umull   r3, r4, r5, r0
        mla     ip, r0, r6, r2
        add     r4, ip, r4
        subs    r2, r7, r3
        sbc     r3, r8, r4
        ldmfd sp!,     {r4, r5, r6, r7, r8, lr}
        bx      lr

