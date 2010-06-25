.thumb
.globl _rgba_bgra_copy
.thumb_func
_rgba_bgra_copy:
    pld [r1, #32]

    vld4.u8 {d0[0], d1[0], d2[0], d3[0]}, [r1]!
    vld4.u8 {d0[1], d1[1], d2[1], d3[1]}, [r1]!
    vld4.u8 {d0[2], d1[2], d2[2], d3[2]}, [r1]!
    vld4.u8 {d0[3], d1[3], d2[3], d3[3]}, [r1]!
    vld4.u8 {d0[4], d1[4], d2[4], d3[4]}, [r1]!
    vld4.u8 {d0[5], d1[5], d2[5], d3[5]}, [r1]!
    vld4.u8 {d0[6], d1[6], d2[6], d3[6]}, [r1]!
    vld4.u8 {d0[7], d1[7], d2[7], d3[7]}, [r1]!

    vswp d0, d2

    vst4.u8 {d0[0], d1[0], d2[0], d3[0]}, [r0]!
    vst4.u8 {d0[1], d1[1], d2[1], d3[1]}, [r0]!
    vst4.u8 {d0[2], d1[2], d2[2], d3[2]}, [r0]!
    vst4.u8 {d0[3], d1[3], d2[3], d3[3]}, [r0]!
    vst4.u8 {d0[4], d1[4], d2[4], d3[4]}, [r0]!
    vst4.u8 {d0[5], d1[5], d2[5], d3[5]}, [r0]!
    vst4.u8 {d0[6], d1[6], d2[6], d3[6]}, [r0]!
    vst4.u8 {d0[7], d1[7], d2[7], d3[7]}, [r0]!

    cmp r1, r2
    blt _rgba_bgra_copy
    bx lr

