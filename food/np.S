	.syntax unified
	.section __TEXT,__text,regular
	.section __TEXT,__textcoal_nt,coalesced
	.section __TEXT,__const_coal,coalesced
	.section __TEXT,__picsymbolstub4,symbol_stubs,none,16
	.text
	.align 2
	.globl _new_fucked_up_variant_object
	.thumb
	.thumb_func _new_fucked_up_variant_object
_new_fucked_up_variant_object:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	sub	sp, sp, #16
	movs	r3, #6
	@ lr needed for prologue
	str	r1, [r0, #4]
	str	r1, [r0, #8]
	str	r1, [r0, #12]
	str	r3, [r0]
	add	sp, sp, #16
	bx	lr
	.align 2
	.globl _NPN_ReleaseVariantValue
	.thumb
	.thumb_func _NPN_ReleaseVariantValue
_NPN_ReleaseVariantValue:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	@ lr needed for prologue
	bx	lr
	.align 2
	.globl _NPN_RetainObject
	.thumb
	.thumb_func _NPN_RetainObject
_NPN_RetainObject:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, [r0, #4]
	@ lr needed for prologue
	adds	r3, r3, #1
	str	r3, [r0, #4]
	bx	lr
	.align 2
	.thumb
	.thumb_func _npnTimerCB
_npnTimerCB:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r7, lr}
	add	r7, sp, #0
	mov	r3, r0
	ldr	r0, L10
	mov	r2, r1
	mov	r1, r3
L8:
	add	r0, pc
	ldr	r0, [r0]
	blx	r2
	pop	{r7, pc}
	.p2align 2
L11:
	.align 2
L10:
	.long	L_nppt$non_lazy_ptr-4-(L8)
	.align 2
	.globl _foo
	.thumb
	.thumb_func _foo
_foo:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	@ lr needed for prologue
	bx	lr
	.cstring
	.align 2
LC0:
	.ascii "---- LATER ----\0"
	.section __DATA, __cfstring
	.align 2
LC1:
	.long	___CFConstantStringClassReference
	.long	1992
	.long	LC0
	.long	15
	.text
	.align 2
	.thumb
	.thumb_func _later
_later:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r7, lr}
	add	r7, sp, #0
	ldr	r0, L18
	ldr	r2, L18+4
	movs	r1, #0
L15:
	add	r0, pc
L16:
	add	r2, pc
	ldr	r0, [r0]
	ldr	r0, [r0]
	blx	L_CFStringCreateWithFormat$stub
	blx	L_logreal_$stub
	pop	{r7, pc}
	.p2align 2
L19:
	.align 2
L18:
	.long	L_kCFAllocatorDefault$non_lazy_ptr-4-(L15)
	.long	LC1-4-(L16)
	.cstring
	.align 2
LC2:
	.ascii "%s: handling event %p\0"
	.section __DATA, __cfstring
	.align 2
LC3:
	.long	___CFConstantStringClassReference
	.long	1992
	.long	LC2
	.long	21
	.cstring
	.align 2
LC4:
	.ascii "HANDLED\0"
	.align 2
LC5:
	.ascii "IGNORED\0"
	.align 2
LC6:
	.ascii "%s: the plugin %s the event %p)\0"
	.section __DATA, __cfstring
	.align 2
LC7:
	.long	___CFConstantStringClassReference
	.long	1992
	.long	LC6
	.long	31
	.text
	.align 2
	.globl _handle_event
	.thumb
	.thumb_func _handle_event
_handle_event:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r4, r5, r7, lr}
	add	r7, sp, #8
	sub	sp, sp, #8
	ldr	r3, L36
	mov	r4, r0
	ldr	r2, L36+4
L21:
	add	r3, pc
	movs	r1, #0
	ldr	r0, [r3]
	ldr	r3, L36+8
L22:
	add	r2, pc
	str	r4, [sp]
	ldr	r5, [r0]
L23:
	add	r3, pc
	mov	r0, r5
	blx	L_CFStringCreateWithFormat$stub
	blx	L_logreal_$stub
	ldr	r3, L36+12
	ldr	r0, L36+16
	mov	r1, r4
L24:
	add	r3, pc
L25:
	add	r0, pc
	ldr	r3, [r3, #40]
	ldr	r0, [r0]
	blx	r3
	uxth	r0, r0
	cbz	r0, L26
	ldr	r1, L36+20
L28:
	add	r1, pc
	b	L29
L26:
	ldr	r1, L36+24
L30:
	add	r1, pc
L29:
	str	r1, [sp]
	ldr	r1, L36+28
	ldr	r2, L36+32
	ldr	r3, L36+36
L33:
	add	r1, pc
L31:
	add	r2, pc
	ldr	r1, [r1, #40]
L32:
	add	r3, pc
	mov	r0, r5
	str	r1, [sp, #4]
	movs	r1, #0
	blx	L_CFStringCreateWithFormat$stub
	blx	L_logreal_$stub
	sub	sp, r7, #8
	pop	{r4, r5, r7, pc}
	.p2align 2
L37:
	.align 2
L36:
	.long	L_kCFAllocatorDefault$non_lazy_ptr-4-(L21)
	.long	LC3-4-(L22)
	.long	___func__.13905-4-(L23)
	.long	_pluginfuncs-4-(L24)
	.long	L_nppt$non_lazy_ptr-4-(L25)
	.long	LC4-4-(L28)
	.long	LC5-4-(L30)
	.long	_pluginfuncs-4-(L33)
	.long	LC7-4-(L31)
	.long	___func__.13905-4-(L32)
	.cstring
	.align 2
LC8:
	.ascii "! fakeConstruct\0"
	.section __DATA, __cfstring
	.align 2
LC9:
	.long	___CFConstantStringClassReference
	.long	1992
	.long	LC8
	.long	15
	.text
	.align 2
	.globl _fakeConstruct
	.thumb
	.thumb_func _fakeConstruct
_fakeConstruct:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r7, lr}
	add	r7, sp, #0
	ldr	r0, L42
	ldr	r2, L42+4
	movs	r1, #0
L39:
	add	r0, pc
L40:
	add	r2, pc
	ldr	r0, [r0]
	ldr	r0, [r0]
	blx	L_CFStringCreateWithFormat$stub
	blx	L_logreal_$stub
	movs	r0, #0
	pop	{r7, pc}
	.p2align 2
L43:
	.align 2
L42:
	.long	L_kCFAllocatorDefault$non_lazy_ptr-4-(L39)
	.long	LC9-4-(L40)
	.cstring
	.align 2
LC10:
	.ascii "! fakeEnumeration\0"
	.section __DATA, __cfstring
	.align 2
LC11:
	.long	___CFConstantStringClassReference
	.long	1992
	.long	LC10
	.long	17
	.text
	.align 2
	.globl _fakeEnumeration
	.thumb
	.thumb_func _fakeEnumeration
_fakeEnumeration:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r7, lr}
	add	r7, sp, #0
	ldr	r0, L48
	ldr	r2, L48+4
	movs	r1, #0
L45:
	add	r0, pc
L46:
	add	r2, pc
	ldr	r0, [r0]
	ldr	r0, [r0]
	blx	L_CFStringCreateWithFormat$stub
	blx	L_logreal_$stub
	movs	r0, #0
	pop	{r7, pc}
	.p2align 2
L49:
	.align 2
L48:
	.long	L_kCFAllocatorDefault$non_lazy_ptr-4-(L45)
	.long	LC11-4-(L46)
	.cstring
	.align 2
LC12:
	.ascii "! fakeRemoveProperty: %@\0"
	.section __DATA, __cfstring
	.align 2
LC13:
	.long	___CFConstantStringClassReference
	.long	1992
	.long	LC12
	.long	24
	.text
	.align 2
	.globl _fakeRemoveProperty
	.thumb
	.thumb_func _fakeRemoveProperty
_fakeRemoveProperty:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r7, lr}
	add	r7, sp, #0
	ldr	r0, L54
	ldr	r2, L54+4
	mov	r3, r1
L51:
	add	r0, pc
L52:
	add	r2, pc
	ldr	r0, [r0]
	movs	r1, #0
	ldr	r0, [r0]
	blx	L_CFStringCreateWithFormat$stub
	blx	L_logreal_$stub
	movs	r0, #0
	pop	{r7, pc}
	.p2align 2
L55:
	.align 2
L54:
	.long	L_kCFAllocatorDefault$non_lazy_ptr-4-(L51)
	.long	LC13-4-(L52)
	.cstring
	.align 2
LC14:
	.ascii "! fakeSetProperty: %@\0"
	.section __DATA, __cfstring
	.align 2
LC15:
	.long	___CFConstantStringClassReference
	.long	1992
	.long	LC14
	.long	21
	.text
	.align 2
	.globl _fakeSetProperty
	.thumb
	.thumb_func _fakeSetProperty
_fakeSetProperty:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r7, lr}
	add	r7, sp, #0
	ldr	r0, L60
	ldr	r2, L60+4
	mov	r3, r1
L57:
	add	r0, pc
L58:
	add	r2, pc
	ldr	r0, [r0]
	movs	r1, #0
	ldr	r0, [r0]
	blx	L_CFStringCreateWithFormat$stub
	blx	L_logreal_$stub
	movs	r0, #0
	pop	{r7, pc}
	.p2align 2
L61:
	.align 2
L60:
	.long	L_kCFAllocatorDefault$non_lazy_ptr-4-(L57)
	.long	LC15-4-(L58)
	.cstring
	.align 2
LC16:
	.ascii "! fakeHasProperty: %@\0"
	.section __DATA, __cfstring
	.align 2
LC17:
	.long	___CFConstantStringClassReference
	.long	1992
	.long	LC16
	.long	21
	.text
	.align 2
	.globl _fakeHasProperty
	.thumb
	.thumb_func _fakeHasProperty
_fakeHasProperty:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r7, lr}
	add	r7, sp, #0
	ldr	r0, L66
	ldr	r2, L66+4
	mov	r3, r1
L63:
	add	r0, pc
L64:
	add	r2, pc
	ldr	r0, [r0]
	movs	r1, #0
	ldr	r0, [r0]
	blx	L_CFStringCreateWithFormat$stub
	blx	L_logreal_$stub
	movs	r0, #0
	pop	{r7, pc}
	.p2align 2
L67:
	.align 2
L66:
	.long	L_kCFAllocatorDefault$non_lazy_ptr-4-(L63)
	.long	LC17-4-(L64)
	.cstring
	.align 2
LC18:
	.ascii "! fakeInvokeDefault\0"
	.section __DATA, __cfstring
	.align 2
LC19:
	.long	___CFConstantStringClassReference
	.long	1992
	.long	LC18
	.long	19
	.text
	.align 2
	.globl _fakeInvokeDefault
	.thumb
	.thumb_func _fakeInvokeDefault
_fakeInvokeDefault:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r7, lr}
	add	r7, sp, #0
	ldr	r0, L72
	ldr	r2, L72+4
	movs	r1, #0
L69:
	add	r0, pc
L70:
	add	r2, pc
	ldr	r0, [r0]
	ldr	r0, [r0]
	blx	L_CFStringCreateWithFormat$stub
	blx	L_logreal_$stub
	movs	r0, #0
	pop	{r7, pc}
	.p2align 2
L73:
	.align 2
L72:
	.long	L_kCFAllocatorDefault$non_lazy_ptr-4-(L69)
	.long	LC19-4-(L70)
	.cstring
	.align 2
LC20:
	.ascii "! fakeInvoke: %@\0"
	.section __DATA, __cfstring
	.align 2
LC21:
	.long	___CFConstantStringClassReference
	.long	1992
	.long	LC20
	.long	16
	.text
	.align 2
	.globl _fakeInvoke
	.thumb
	.thumb_func _fakeInvoke
_fakeInvoke:
	@ args = 4, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r7, lr}
	add	r7, sp, #0
	ldr	r0, L78
	ldr	r2, L78+4
	mov	r3, r1
L75:
	add	r0, pc
L76:
	add	r2, pc
	ldr	r0, [r0]
	movs	r1, #0
	ldr	r0, [r0]
	blx	L_CFStringCreateWithFormat$stub
	blx	L_logreal_$stub
	movs	r0, #0
	pop	{r7, pc}
	.p2align 2
L79:
	.align 2
L78:
	.long	L_kCFAllocatorDefault$non_lazy_ptr-4-(L75)
	.long	LC21-4-(L76)
	.cstring
	.align 2
LC22:
	.ascii "! fakeHasMethod: %@\0"
	.section __DATA, __cfstring
	.align 2
LC23:
	.long	___CFConstantStringClassReference
	.long	1992
	.long	LC22
	.long	19
	.text
	.align 2
	.globl _fakeHasMethod
	.thumb
	.thumb_func _fakeHasMethod
_fakeHasMethod:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r7, lr}
	add	r7, sp, #0
	ldr	r0, L84
	ldr	r2, L84+4
	mov	r3, r1
L81:
	add	r0, pc
L82:
	add	r2, pc
	ldr	r0, [r0]
	movs	r1, #0
	ldr	r0, [r0]
	blx	L_CFStringCreateWithFormat$stub
	blx	L_logreal_$stub
	movs	r0, #0
	pop	{r7, pc}
	.p2align 2
L85:
	.align 2
L84:
	.long	L_kCFAllocatorDefault$non_lazy_ptr-4-(L81)
	.long	LC23-4-(L82)
	.cstring
	.align 2
LC24:
	.ascii "! fakeInvalidate\0"
	.section __DATA, __cfstring
	.align 2
LC25:
	.long	___CFConstantStringClassReference
	.long	1992
	.long	LC24
	.long	16
	.text
	.align 2
	.globl _fakeInvalidate
	.thumb
	.thumb_func _fakeInvalidate
_fakeInvalidate:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r7, lr}
	add	r7, sp, #0
	ldr	r0, L90
	ldr	r2, L90+4
	movs	r1, #0
L87:
	add	r0, pc
L88:
	add	r2, pc
	ldr	r0, [r0]
	ldr	r0, [r0]
	blx	L_CFStringCreateWithFormat$stub
	blx	L_logreal_$stub
	pop	{r7, pc}
	.p2align 2
L91:
	.align 2
L90:
	.long	L_kCFAllocatorDefault$non_lazy_ptr-4-(L87)
	.long	LC25-4-(L88)
	.cstring
	.align 2
LC26:
	.ascii "np.c\0"
	.align 2
LC27:
	.ascii "wsize >= data_size\0"
	.align 2
LC28:
	.ascii "ret == data_size\0"
	.text
	.align 2
	.thumb
	.thumb_func _func.13786
_func.13786:
	@ Nested: function declared inside another function.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, lr}
	add	r7, sp, #12
	sub	sp, sp, #4
	ldr	r0, L109
	ldr	r3, L109+4
	ldr	r1, [ip, #8]
L94:
	add	r0, pc
L93:
	add	r3, pc
	ldr	r6, [r0]
	ldr	r3, [r3, #28]
	mov	r4, ip
	mov	r0, r6
	blx	r3
	ldr	r3, [r4, #4]
	cmp	r0, r3
	bcs	L95
	ldr	r0, L109+8
	ldr	r1, L109+12
	ldr	r3, L109+16
L97:
	add	r0, pc
L98:
	add	r1, pc
L99:
	add	r3, pc
	mov	r2, #340
	b	L108
L95:
	ldr	r2, L109+20
	ldr	r0, [r4]
	ldr	r1, [r4, #8]
L100:
	add	r2, pc
	str	r0, [sp]
	mov	r0, r6
	ldr	r5, [r2, #32]
	movs	r2, #0
	blx	r5
	ldr	r3, [r4, #4]
	cmp	r0, r3
	beq	L107
	ldr	r0, L109+24
	ldr	r1, L109+28
	ldr	r3, L109+32
L104:
	add	r0, pc
L105:
	add	r1, pc
L106:
	add	r3, pc
	mov	r2, #342
L108:
	blx	L___assert_rtn$stub
L107:
	sub	sp, r7, #12
	pop	{r4, r5, r6, r7, pc}
	.p2align 2
L110:
	.align 2
L109:
	.long	L_nppt$non_lazy_ptr-4-(L94)
	.long	_pluginfuncs-4-(L93)
	.long	___func__.13789-4-(L97)
	.long	LC26-4-(L98)
	.long	LC27-4-(L99)
	.long	_pluginfuncs-4-(L100)
	.long	___func__.13789-4-(L104)
	.long	LC26-4-(L105)
	.long	LC28-4-(L106)
	.cstring
	.align 2
LC29:
	.ascii "...setValue: %d => %p\0"
	.section __DATA, __cfstring
	.align 2
LC30:
	.long	___CFConstantStringClassReference
	.long	1992
	.long	LC29
	.long	21
	.cstring
	.align 2
LC31:
	.ascii "value == (void *) kSurface_ANPDrawingModel\0"
	.align 2
LC32:
	.ascii "acceptEvents: %d\0"
	.section __DATA, __cfstring
	.align 2
LC33:
	.long	___CFConstantStringClassReference
	.long	1992
	.long	LC32
	.long	16
	.text
	.align 2
	.globl _NPN_SetValue
	.thumb
	.thumb_func _NPN_SetValue
_NPN_SetValue:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, lr}
	add	r7, sp, #12
	sub	sp, sp, #4
	ldr	r3, L126
	mov	r5, r2
	ldr	r2, L126+4
L112:
	add	r3, pc
	mov	r4, r1
	ldr	r0, [r3]
	movs	r1, #0
L113:
	add	r2, pc
	mov	r3, r4
	ldr	r6, [r0]
	str	r5, [sp]
	mov	r0, r6
	blx	L_CFStringCreateWithFormat$stub
	blx	L_logreal_$stub
	cmp	r4, #1000
	beq	L115
	movw	r3, #1001
	cmp	r4, r3
	beq	L116
	movs	r0, #1
	b	L117
L115:
	cmp	r5, #2
	beq	L125
	ldr	r0, L126+8
	ldr	r1, L126+12
	ldr	r3, L126+16
L120:
	add	r0, pc
L121:
	add	r1, pc
L122:
	add	r3, pc
	movs	r2, #238
	blx	L___assert_rtn$stub
L116:
	ldr	r2, L126+20
	ldr	r3, [r5]
	movs	r1, #0
L123:
	add	r2, pc
	mov	r0, r6
	blx	L_CFStringCreateWithFormat$stub
	blx	L_logreal_$stub
L125:
	movs	r0, #0
L117:
	sub	sp, r7, #12
	pop	{r4, r5, r6, r7, pc}
	.p2align 2
L127:
	.align 2
L126:
	.long	L_kCFAllocatorDefault$non_lazy_ptr-4-(L112)
	.long	LC30-4-(L113)
	.long	___func__.13665-4-(L120)
	.long	LC26-4-(L121)
	.long	LC31-4-(L122)
	.long	LC33-4-(L123)
	.cstring
	.align 2
LC34:
	.ascii "UnscheduleTimer\0"
	.section __DATA, __cfstring
	.align 2
LC35:
	.long	___CFConstantStringClassReference
	.long	1992
	.long	LC34
	.long	15
	.text
	.align 2
	.globl _NPN_UnscheduleTimer
	.thumb
	.thumb_func _NPN_UnscheduleTimer
_NPN_UnscheduleTimer:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r4, r7, lr}
	add	r7, sp, #4
	ldr	r0, L132
	ldr	r2, L132+4
	mov	r4, r1
L129:
	add	r0, pc
L130:
	add	r2, pc
	ldr	r0, [r0]
	movs	r1, #0
	ldr	r0, [r0]
	blx	L_CFStringCreateWithFormat$stub
	blx	L_logreal_$stub
	mov	r0, r4
	blx	L_CFRunLoopTimerInvalidate$stub
	mov	r0, r4
	blx	L_CFRelease$stub
	pop	{r4, r7, pc}
	.p2align 2
L133:
	.align 2
L132:
	.long	L_kCFAllocatorDefault$non_lazy_ptr-4-(L129)
	.long	LC35-4-(L130)
	.cstring
	.align 2
LC36:
	.ascii "ScheduleTimer interval=%u repeat=%s cb=%p\0"
	.section __DATA, __cfstring
	.align 2
LC37:
	.long	___CFConstantStringClassReference
	.long	1992
	.long	LC36
	.long	41
	.cstring
	.align 2
LC38:
	.ascii "YES\0"
	.align 2
LC39:
	.ascii "[%p] ival=%f\0"
	.section __DATA, __cfstring
	.align 2
LC40:
	.long	___CFConstantStringClassReference
	.long	1992
	.long	LC39
	.long	12
	.cstring
	.align 2
LC41:
	.ascii "NO\0"
	.text
	.align 2
	.globl _NPN_ScheduleTimer
	.thumb
	.thumb_func _NPN_ScheduleTimer
_NPN_ScheduleTimer:
	@ args = 0, pretend = 0, frame = 20
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, lr}
	add	r7, sp, #12
	push	{r8}
	fstmfdd	sp!, {d8}
	sub	sp, sp, #40
	mov	r6, r3
	ldr	r3, L148+8
	uxtb	r4, r2
	mov	r5, r1
L135:
	add	r3, pc
	ldr	r0, [r3]
	ldr	r8, [r0]
	cmp	r4, #0
	beq	L136
	ldr	r2, L148+12
	ldr	r3, L148+16
	movs	r1, #0
L138:
	add	r2, pc
L139:
	add	r3, pc
	mov	r0, r8
	str	r3, [sp]
	mov	r3, r5
	str	r6, [sp, #4]
	blx	L_CFStringCreateWithFormat$stub
	blx	L_logreal_$stub
	fmsr	s10, r5	@ int
	fuitod	d7, s10
	fldd	d6, L148
	ldr	r2, L148+20
	movs	r1, #0
	mov	r3, r6
L140:
	add	r2, pc
	mov	r0, r8
	fmuld	d5, d7, d6
	str	r1, [sp, #20]
	str	r1, [sp, #28]
	str	r1, [sp, #32]
	str	r1, [sp, #36]
	str	r6, [sp, #24]
	fcvtsd	s14, d5
	fmrrd	r4, r5, d5
	fcvtds	d7, s14
	fstd	d7, [sp]
	blx	L_CFStringCreateWithFormat$stub
	blx	L_logreal_$stub
	blx	L_CFAbsoluteTimeGetCurrent$stub
	fmdrr	d6, r4, r5
	fmdrr	d7, r0, r1
	faddd	d7, d7, d6
	b	L141
L136:
	ldr	r2, L148+24
	ldr	r3, L148+28
	mov	r1, r4
L144:
	add	r2, pc
L145:
	add	r3, pc
	mov	r0, r8
	str	r3, [sp]
	mov	r3, r5
	str	r6, [sp, #4]
	blx	L_CFStringCreateWithFormat$stub
	blx	L_logreal_$stub
	fmsr	s10, r5	@ int
	fuitod	d7, s10
	fldd	d6, L148
	ldr	r2, L148+32
	mov	r1, r4
	mov	r3, r6
L146:
	add	r2, pc
	mov	r0, r8
	fmuld	d8, d7, d6
	str	r4, [sp, #20]
	str	r4, [sp, #28]
	str	r4, [sp, #32]
	str	r4, [sp, #36]
	str	r6, [sp, #24]
	movs	r4, #0
	movs	r5, #0
	fcvtsd	s14, d8
	fcvtds	d7, s14
	fstd	d7, [sp]
	blx	L_CFStringCreateWithFormat$stub
	blx	L_logreal_$stub
	blx	L_CFAbsoluteTimeGetCurrent$stub
	fmdrr	d7, r0, r1
	faddd	d7, d7, d8
L141:
	ldr	r3, L148+36
	fmrrd	r1, r2, d7
	movs	r0, #0
L142:
	add	r3, pc
	str	r3, [sp, #12]
	add	r3, sp, #20
	str	r3, [sp, #16]
	mov	r3, r4
	str	r0, [sp, #4]
	str	r0, [sp, #8]
	str	r5, [sp]
	blx	L_CFRunLoopTimerCreate$stub
	mov	r4, r0
	blx	L_CFRunLoopGetMain$stub
	ldr	r2, L148+40
	mov	r1, r4
L143:
	add	r2, pc
	ldr	r2, [r2]
	ldr	r2, [r2]
	blx	L_CFRunLoopAddTimer$stub
	mov	r0, r4
	sub	sp, r7, #24
	fldmfdd	sp!, {d8}
	sub	sp, r7, #16
	pop	{r8}
	pop	{r4, r5, r6, r7, pc}
	.p2align 2
L149:
	.align 2
L148:
	.long	-755914244
	.long	1062232653
	.long	L_kCFAllocatorDefault$non_lazy_ptr-4-(L135)
	.long	LC37-4-(L138)
	.long	LC38-4-(L139)
	.long	LC40-4-(L140)
	.long	LC37-4-(L144)
	.long	LC41-4-(L145)
	.long	LC40-4-(L146)
	.long	_npnTimerCB-4-(L142)
	.long	L_kCFRunLoopCommonModes$non_lazy_ptr-4-(L143)
	.align 2
	.globl _new_fake_object
	.thumb
	.thumb_func _new_fake_object
_new_fake_object:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r7, lr}
	add	r7, sp, #0
	movs	r0, #8
	blx	L_malloc$stub
	ldr	r3, L153
L151:
	add	r3, pc
	str	r3, [r0]
	movs	r3, #1
	str	r3, [r0, #4]
	pop	{r7, pc}
	.p2align 2
L154:
	.align 2
L153:
	.long	_fake_class-4-(L151)
	.cstring
	.align 2
LC42:
	.ascii "!new_connection(food, stream, (char *) url, (char *) target)\0"
	.text
	.align 2
	.globl _NPN_GetURLNotify
	.thumb
	.thumb_func _NPN_GetURLNotify
_NPN_GetURLNotify:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, lr}
	add	r7, sp, #12
	push	{r8, r10}
	movs	r0, #28
	mov	r8, r1
	mov	r10, r2
	mov	r6, r3
	blx	L_malloc$stub
	movs	r5, #0
	mov	r4, r0
	str	r5, [r0]
	str	r5, [r0, #4]
	mov	r0, r8
	blx	L_strdup$stub
	str	r5, [r4, #12]
	str	r5, [r4, #16]
	str	r6, [r4, #20]
	str	r5, [r4, #24]
	mov	r1, r4
	mov	r2, r8
	mov	r3, r10
	str	r0, [r4, #8]
	ldr	r0, L163
L156:
	add	r0, pc
	ldr	r0, [r0]
	ldr	r0, [r0]
	blx	L_new_connection$stub
	cbz	r0, L157
	ldr	r0, L163+4
	ldr	r1, L163+8
	ldr	r3, L163+12
L159:
	add	r0, pc
L160:
	add	r1, pc
L161:
	add	r3, pc
	movw	r2, #361
	blx	L___assert_rtn$stub
L157:
	pop	{r8, r10}
	pop	{r4, r5, r6, r7, pc}
	.p2align 2
L164:
	.align 2
L163:
	.long	L_food$non_lazy_ptr-4-(L156)
	.long	___func__.13829-4-(L159)
	.long	LC26-4-(L160)
	.long	LC42-4-(L161)
	.const
	.align 2
LTRAMP0:
	ldr.w	ip, [pc, #4]
	ldr.w	pc, [pc, #4]
	.long	0
	.long	0
	.text
	.align 2
	.globl _connection_got_data
	.thumb
	.thumb_func _connection_got_data
_connection_got_data:
	@ args = 0, pretend = 0, frame = 28
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r4, r7, lr}
	add	r7, sp, #4
	sub	sp, sp, #28
	ldr	r0, L169
	str	r1, [sp, #8]
	str	r2, [sp]
L166:
	add	r0, pc
	str	r3, [sp, #4]
	ldmia	r0, {r0, r1, r2, r3}
	add	r4, sp, #12
	stmia	r4, {r0, r1, r2, r3}
	ldr	r3, L169+4
	add	r0, sp, #12
	add	r1, sp, #28
L167:
	add	r3, pc
	str	sp, [sp, #20]
	str	r3, [sp, #24]
	blx	L___clear_cache$stub
	orr	r0, r4, #1
	blx	L_main_thread_kthx$stub
	movs	r0, #0
	sub	sp, r7, #4
	pop	{r4, r7, pc}
	.p2align 2
L170:
	.align 2
L169:
	.long	LTRAMP0-4-(L166)
	.long	_func.13786-4-(L167)
	.align 2
	.globl _connection_response
	.thumb
	.thumb_func _connection_response
_connection_response:
	@ args = 8, pretend = 0, frame = 36
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r4, r7, lr}
	add	r7, sp, #4
	sub	sp, sp, #36
	ldr	r0, L175
	str	r2, [sp, #8]
	str	r3, [sp, #12]
	add	r2, sp, #48
	ldmia	r2, {r2-r3}
L172:
	add	r0, pc
	str	r1, [sp, #16]
	add	r4, sp, #20
	stmia	sp, {r2-r3}
	ldmia	r0, {r0, r1, r2, r3}
	stmia	r4, {r0, r1, r2, r3}
	ldr	r3, L175+4
	add	r0, sp, #20
	add	r1, sp, #36
L173:
	add	r3, pc
	str	sp, [sp, #28]
	str	r3, [sp, #32]
	blx	L___clear_cache$stub
	orr	r0, r4, #1
	blx	L_main_thread_kthx$stub
	movs	r0, #0
	sub	sp, r7, #4
	pop	{r4, r7, pc}
	.p2align 2
L176:
	.align 2
L175:
	.long	LTRAMP0-4-(L172)
	.long	_func.13731-4-(L173)
	.cstring
	.align 2
LC43:
	.ascii "!stream->headers\0"
	.align 2
LC44:
	.ascii "!pluginfuncs.newstream(&nppt, (void *) 0xdeadbeef, stream, false, &stype)\0"
	.align 2
LC45:
	.ascii "stype == NP_NORMAL\0"
	.text
	.align 2
	.thumb
	.thumb_func _func.13731
_func.13731:
	@ Nested: function declared inside another function.
	@ args = 0, pretend = 0, frame = 4
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r4, r5, r7, lr}
	add	r7, sp, #8
	sub	sp, sp, #8
	ldr	r3, [ip, #16]
	mov	r4, ip
	ldr	r5, [r3, #24]
	cbz	r5, L178
	ldr	r0, L197
	ldr	r1, L197+4
	ldr	r3, L197+8
L180:
	add	r0, pc
L181:
	add	r1, pc
L182:
	add	r3, pc
	movw	r2, #323
	b	L196
L178:
	ldr	r0, [ip, #12]
	adds	r0, r0, #1
	blx	L_malloc$stub
	ldr	r3, [r4, #16]
	str	r0, [r3, #24]
	ldr	r3, [r4, #16]
	ldr	r1, [r4, #8]
	ldr	r2, [r4, #12]
	ldr	r0, [r3, #24]
	blx	L_memcpy$stub
	ldr	r3, [r4, #16]
	ldr	r2, [r4, #12]
	ldr	r1, L197+12
	ldr	r0, L197+16
	ldr	r3, [r3, #24]
L183:
	add	r1, pc
L184:
	add	r0, pc
	strb	r5, [r3, r2]
	ldr	r2, [r4, #16]
	ldr	r3, [r4]
	ldr	r0, [r0]
	str	r3, [r2, #12]
	add	r3, sp, #6
	ldr	r2, [r4, #16]
	str	r3, [sp]
	mov	r3, r5
	ldr	r4, [r1, #16]
	ldr	r1, L197+20
	blx	r4
	uxth	r0, r0
	cbz	r0, L185
	ldr	r0, L197+24
	ldr	r1, L197+28
	ldr	r3, L197+32
L187:
	add	r0, pc
L188:
	add	r1, pc
L189:
	add	r3, pc
	mov	r2, #330
L196:
	blx	L___assert_rtn$stub
L185:
	ldrh	r3, [sp, #6]
	cmp	r3, #1
	beq	L195
	ldr	r0, L197+36
	ldr	r1, L197+40
	ldr	r3, L197+44
L192:
	add	r0, pc
L193:
	add	r1, pc
L194:
	add	r3, pc
	movw	r2, #331
	b	L196
L195:
	sub	sp, r7, #8
	pop	{r4, r5, r7, pc}
	.p2align 2
L198:
	.align 2
L197:
	.long	___func__.13733-4-(L180)
	.long	LC26-4-(L181)
	.long	LC43-4-(L182)
	.long	_pluginfuncs-4-(L183)
	.long	L_nppt$non_lazy_ptr-4-(L184)
	.long	-559038737
	.long	___func__.13733-4-(L187)
	.long	LC26-4-(L188)
	.long	LC44-4-(L189)
	.long	___func__.13733-4-(L192)
	.long	LC26-4-(L193)
	.long	LC45-4-(L194)
	.align 2
	.globl _NPN_MemFree
	.thumb
	.thumb_func _NPN_MemFree
_NPN_MemFree:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r7, lr}
	add	r7, sp, #0
	blx	L_free$stub
	pop	{r7, pc}
	.cstring
	.align 2
LC46:
	.ascii "NPN_ReleaseObject: object %p to be destroyed\0"
	.section __DATA, __cfstring
	.align 2
LC47:
	.long	___CFConstantStringClassReference
	.long	1992
	.long	LC46
	.long	44
	.cstring
	.align 2
LC48:
	.ascii "NPN_ReleaseObject: using free()\0"
	.section __DATA, __cfstring
	.align 2
LC49:
	.long	___CFConstantStringClassReference
	.long	1992
	.long	LC48
	.long	31
	.text
	.align 2
	.globl _NPN_ReleaseObject
	.thumb
	.thumb_func _NPN_ReleaseObject
_NPN_ReleaseObject:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r4, r5, r7, lr}
	add	r7, sp, #8
	ldr	r1, [r0, #4]
	mov	r4, r0
	subs	r1, r1, #1
	str	r1, [r0, #4]
	cbnz	r1, L209
	ldr	r3, L210
	ldr	r2, L210+4
L204:
	add	r3, pc
L205:
	add	r2, pc
	ldr	r0, [r3]
	mov	r3, r4
	ldr	r5, [r0]
	mov	r0, r5
	blx	L_CFStringCreateWithFormat$stub
	blx	L_logreal_$stub
	ldr	r1, [r4]
	ldr	r1, [r1, #8]
	cbz	r1, L206
	mov	r0, r4
	blx	r1
	b	L209
L206:
	ldr	r2, L210+8
	mov	r0, r5
L208:
	add	r2, pc
	blx	L_CFStringCreateWithFormat$stub
	blx	L_logreal_$stub
	mov	r0, r4
	blx	L_free$stub
L209:
	pop	{r4, r5, r7, pc}
	.p2align 2
L211:
	.align 2
L210:
	.long	L_kCFAllocatorDefault$non_lazy_ptr-4-(L204)
	.long	LC47-4-(L205)
	.long	LC49-4-(L208)
	.align 2
	.globl _fakeDeallocate
	.thumb
	.thumb_func _fakeDeallocate
_fakeDeallocate:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r7, lr}
	add	r7, sp, #0
	blx	L_free$stub
	pop	{r7, pc}
	.align 2
	.globl _NPN_MemAlloc
	.thumb
	.thumb_func _NPN_MemAlloc
_NPN_MemAlloc:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r7, lr}
	add	r7, sp, #0
	mov	r1, r0
	movs	r0, #1
	blx	L_calloc$stub
	pop	{r7, pc}
	.cstring
	.align 2
LC50:
	.ascii "NPN_CreateObject\0"
	.section __DATA, __cfstring
	.align 2
LC51:
	.long	___CFConstantStringClassReference
	.long	1992
	.long	LC50
	.long	16
	.cstring
	.align 2
LC52:
	.ascii "NPN_CreateObject: using default allocator\0"
	.section __DATA, __cfstring
	.align 2
LC53:
	.long	___CFConstantStringClassReference
	.long	1992
	.long	LC52
	.long	41
	.text
	.align 2
	.globl _NPN_CreateObject
	.thumb
	.thumb_func _NPN_CreateObject
_NPN_CreateObject:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, lr}
	add	r7, sp, #12
	ldr	r3, L224
	mov	r6, r0
	ldr	r2, L224+4
L217:
	add	r3, pc
	mov	r4, r1
	ldr	r0, [r3]
L218:
	add	r2, pc
	movs	r1, #0
	ldr	r5, [r0]
	mov	r0, r5
	blx	L_CFStringCreateWithFormat$stub
	blx	L_logreal_$stub
	ldr	r3, [r4, #4]
	cbz	r3, L219
	mov	r0, r6
	mov	r1, r4
	blx	r3
	b	L221
L219:
	ldr	r2, L224+8
	mov	r1, r3
	mov	r0, r5
L222:
	add	r2, pc
	blx	L_CFStringCreateWithFormat$stub
	blx	L_logreal_$stub
	movs	r0, #1
	movs	r1, #8
	blx	L_calloc$stub
	movs	r3, #1
	str	r3, [r0, #4]
L221:
	str	r4, [r0]
	pop	{r4, r5, r6, r7, pc}
	.p2align 2
L225:
	.align 2
L224:
	.long	L_kCFAllocatorDefault$non_lazy_ptr-4-(L217)
	.long	LC51-4-(L218)
	.long	LC53-4-(L222)
	.align 2
	.globl _fakeAllocate
	.thumb
	.thumb_func _fakeAllocate
_fakeAllocate:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r7, lr}
	add	r7, sp, #0
	movs	r0, #1
	movs	r1, #52
	blx	L_calloc$stub
	pop	{r7, pc}
	.cstring
	.align 2
LC54:
	.ascii "...getValue: %d\0"
	.section __DATA, __cfstring
	.align 2
LC55:
	.long	___CFConstantStringClassReference
	.long	1992
	.long	LC54
	.long	15
	.cstring
	.align 2
LC56:
	.ascii "   %x %x %x %x\0"
	.section __DATA, __cfstring
	.align 2
LC57:
	.long	___CFConstantStringClassReference
	.long	1992
	.long	LC56
	.long	14
	.cstring
	.align 2
LC58:
	.ascii "kJavaContext_ANPGetValue\0"
	.section __DATA, __cfstring
	.align 2
LC59:
	.long	___CFConstantStringClassReference
	.long	1992
	.long	LC58
	.long	24
	.cstring
	.align 2
LC60:
	.ascii "unknown GetValue! returning with error\0"
	.section __DATA, __cfstring
	.align 2
LC61:
	.long	___CFConstantStringClassReference
	.long	1992
	.long	LC60
	.long	38
	.text
	.align 2
	.globl _NPN_GetValue
	.thumb
	.thumb_func _NPN_GetValue
_NPN_GetValue:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, lr}
	add	r7, sp, #12
	sub	sp, sp, #12
	ldr	r3, L246
	mov	r4, r2
	ldr	r2, L246+4
L229:
	add	r3, pc
	mov	r5, r1
	ldr	r0, [r3]
	movs	r1, #0
L230:
	add	r2, pc
	mov	r3, r5
	ldr	r6, [r0]
	mov	r0, r6
	blx	L_CFStringCreateWithFormat$stub
	blx	L_logreal_$stub
	ldr	r1, [r4, #4]
	ldr	r2, L246+8
	ldr	r3, [r4]
	mov	r0, r6
	str	r1, [sp]
	ldr	r1, [r4, #8]
L231:
	add	r2, pc
	str	r1, [sp, #4]
	ldr	r1, [r4, #12]
	str	r1, [sp, #8]
	movs	r1, #0
	blx	L_CFStringCreateWithFormat$stub
	blx	L_logreal_$stub
	sub	r3, r5, #1000
	cmp	r3, #11
	bhi	L232
	mov	r0, r5
	mov	r1, r4
	blx	L_iface_getvalue$stub
	b	L244
L232:
	cmp	r5, #2000
	beq	L237
	movw	r3, #2001
	cmp	r5, r3
	beq	L238
	cmp	r5, #15
	bne	L243
	b	L245
L237:
	movs	r0, #0
	str	r0, [r4]
	b	L234
L238:
	ldr	r2, L246+12
	movs	r1, #0
	mov	r0, r6
L239:
	add	r2, pc
	blx	L_CFStringCreateWithFormat$stub
	blx	L_logreal_$stub
	ldr	r3, L246+16
L240:
	add	r3, pc
	ldr	r3, [r3]
	ldr	r3, [r3]
	str	r3, [r4]
	b	L244
L245:
	bl	_new_fake_object
	str	r0, [r4]
L244:
	movs	r0, #0
	b	L234
L243:
	ldr	r2, L246+20
	movs	r1, #0
	mov	r0, r6
L241:
	add	r2, pc
	blx	L_CFStringCreateWithFormat$stub
	blx	L_logreal_$stub
	ldr	r3, L246+24
	movs	r0, #1
	str	r3, [r4]
L234:
	sub	sp, r7, #12
	pop	{r4, r5, r6, r7, pc}
	.p2align 2
L247:
	.align 2
L246:
	.long	L_kCFAllocatorDefault$non_lazy_ptr-4-(L229)
	.long	LC55-4-(L230)
	.long	LC57-4-(L231)
	.long	LC59-4-(L239)
	.long	L_android_context$non_lazy_ptr-4-(L240)
	.long	LC61-4-(L241)
	.long	-559038751
	.cstring
	.align 2
LC62:
	.ascii "NPN_GetProperty %s\0"
	.section __DATA, __cfstring
	.align 2
LC63:
	.long	___CFConstantStringClassReference
	.long	1992
	.long	LC62
	.long	18
	.text
	.align 2
	.globl _NPN_GetProperty
	.thumb
	.thumb_func _NPN_GetProperty
_NPN_GetProperty:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, lr}
	add	r7, sp, #12
	push	{r8}
	mov	r8, r3
	ldr	r3, L252
	mov	r5, r1
	mov	r6, r2
L249:
	add	r3, pc
	ldr	r0, [r3]
	ldr	r4, [r0]
	mov	r0, r2
	blx	L_CFDataGetBytePtr$stub
	ldr	r2, L252+4
	movs	r1, #0
L250:
	add	r2, pc
	mov	r3, r0
	mov	r0, r4
	blx	L_CFStringCreateWithFormat$stub
	blx	L_logreal_$stub
	ldr	r3, [r5]
	mov	r0, r5
	mov	r1, r6
	mov	r2, r8
	ldr	r3, [r3, #32]
	blx	r3
	pop	{r8}
	pop	{r4, r5, r6, r7, pc}
	.p2align 2
L253:
	.align 2
L252:
	.long	L_kCFAllocatorDefault$non_lazy_ptr-4-(L249)
	.long	LC63-4-(L250)
	.cstring
	.align 2
LC64:
	.ascii "CFNumberGetValue(identifier, kCFNumberIntType, &ret)\0"
	.text
	.align 2
	.globl _NPN_IntFromIdentifier
	.thumb
	.thumb_func _NPN_IntFromIdentifier
_NPN_IntFromIdentifier:
	@ args = 0, pretend = 0, frame = 4
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r7, lr}
	add	r7, sp, #0
	sub	sp, sp, #4
	movs	r1, #9
	mov	r2, sp
	blx	L_CFNumberGetValue$stub
	cbnz	r0, L255
	ldr	r0, L261
	ldr	r1, L261+4
	ldr	r3, L261+8
L257:
	add	r0, pc
L258:
	add	r1, pc
L259:
	add	r3, pc
	movs	r2, #121
	blx	L___assert_rtn$stub
L255:
	ldr	r0, [sp]
	sub	sp, r7, #0
	pop	{r7, pc}
	.p2align 2
L262:
	.align 2
L261:
	.long	___func__.13484-4-(L257)
	.long	LC26-4-(L258)
	.long	LC64-4-(L259)
	.align 2
	.globl _NPN_UTF8FromIdentifier
	.thumb
	.thumb_func _NPN_UTF8FromIdentifier
_NPN_UTF8FromIdentifier:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, lr}
	add	r7, sp, #12
	push	{r8}
	mov	r6, r0
	blx	L_CFDataGetLength$stub
	movs	r4, #0
	adds	r0, r0, #1
	blx	L_malloc$stub
	mov	r8, r0
	mov	r0, r6
	blx	L_CFDataGetLength$stub
	mov	r1, r4
	mov	r3, r8
	mov	r5, r0
	mov	r2, r5
	mov	r0, r6
	blx	L_CFDataGetBytes$stub
	mov	r0, r6
	blx	L_CFDataGetLength$stub
	strb	r4, [r0, r8]
	mov	r0, r8
	pop	{r8}
	pop	{r4, r5, r6, r7, pc}
	.align 2
	.globl _NPN_IdentifierIsString
	.thumb
	.thumb_func _NPN_IdentifierIsString
_NPN_IdentifierIsString:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r4, r7, lr}
	add	r7, sp, #4
	blx	L_CFGetTypeID$stub
	mov	r4, r0
	blx	L_CFDataGetTypeID$stub
	cmp	r4, r0
	ite	ne
	movne	r0, #0
	moveq	r0, #1
	pop	{r4, r7, pc}
	.cstring
	.align 2
LC65:
	.ascii "Get*INT*Identifier: %d\0"
	.section __DATA, __cfstring
	.align 2
LC66:
	.long	___CFConstantStringClassReference
	.long	1992
	.long	LC65
	.long	22
	.text
	.align 2
	.globl _NPN_GetIntIdentifier
	.thumb
	.thumb_func _NPN_GetIntIdentifier
_NPN_GetIntIdentifier:
	@ args = 0, pretend = 0, frame = 4
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r4, r5, r7, lr}
	add	r7, sp, #8
	sub	sp, sp, #4
	str	r0, [sp]
	ldr	r0, L276
	ldr	r2, L276+4
	movs	r1, #0
L268:
	add	r0, pc
L269:
	add	r2, pc
	ldr	r0, [r0]
	ldr	r3, [sp]
	ldr	r0, [r0]
	blx	L_CFStringCreateWithFormat$stub
	blx	L_logreal_$stub
	movs	r1, #9
	movs	r0, #0
	mov	r2, sp
	blx	L_CFNumberCreate$stub
	mov	r4, r0
	ldr	r0, L276+8
	mov	r1, r4
L270:
	add	r0, pc
	ldr	r0, [r0]
	blx	L_CFSetGetValue$stub
	cbz	r0, L271
	mov	r5, r0
	b	L273
L271:
	ldr	r0, L276+12
	mov	r1, r4
	mov	r5, r4
L274:
	add	r0, pc
	ldr	r0, [r0]
	blx	L_CFSetAddValue$stub
L273:
	mov	r0, r4
	blx	L_CFRelease$stub
	mov	r0, r5
	sub	sp, r7, #8
	pop	{r4, r5, r7, pc}
	.p2align 2
L277:
	.align 2
L276:
	.long	L_kCFAllocatorDefault$non_lazy_ptr-4-(L268)
	.long	LC66-4-(L269)
	.long	_identifier_set-4-(L270)
	.long	_identifier_set-4-(L274)
	.align 2
	.globl _new_fucked_up_variant_string
	.thumb
	.thumb_func _new_fucked_up_variant_string
_new_fucked_up_variant_string:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r4, r7, lr}
	add	r7, sp, #4
	sub	sp, sp, #16
	ldr	r3, L280
	mov	r4, r0
	str	r1, [r0, #8]
	str	r3, [r0, #4]
	mov	r0, r1
	blx	L_strlen$stub
	movs	r3, #5
	str	r3, [r4]
	str	r0, [r4, #12]
	mov	r0, r4
	sub	sp, r7, #4
	pop	{r4, r7, pc}
	.p2align 2
L281:
	.align 2
L280:
	.long	-559030611
	.cstring
	.align 2
LC67:
	.ascii "evaluate %s\0"
	.section __DATA, __cfstring
	.align 2
LC68:
	.long	___CFConstantStringClassReference
	.long	1992
	.long	LC67
	.long	11
	.cstring
	.align 2
LC69:
	.ascii "document.readyState\0"
	.align 2
LC70:
	.ascii "complete\0"
	.text
	.align 2
	.globl _NPN_Evaluate
	.thumb
	.thumb_func _NPN_Evaluate
_NPN_Evaluate:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r4, r5, r7, lr}
	add	r7, sp, #8
	sub	sp, sp, #16
	ldr	r0, L291
	mov	r4, r2
	ldr	r2, L291+4
L283:
	add	r0, pc
	movs	r1, #0
	ldr	r0, [r0]
L284:
	add	r2, pc
	mov	r5, r3
	ldr	r3, [r4]
	ldr	r0, [r0]
	blx	L_CFStringCreateWithFormat$stub
	blx	L_logreal_$stub
	ldr	r1, L291+8
	ldr	r0, [r4]
L286:
	add	r1, pc
	blx	L_strcmp$stub
	cbz	r0, L290
	blx	L_abort$stub
L290:
	ldr	r1, L291+12
	mov	r0, sp
	mov	r4, sp
L288:
	add	r1, pc
	bl	_new_fucked_up_variant_string
	ldmia	sp, {r0, r1, r2, r3}
	stmia	r5, {r0, r1, r2, r3}
	movs	r0, #1
	sub	sp, r7, #8
	pop	{r4, r5, r7, pc}
	.p2align 2
L292:
	.align 2
L291:
	.long	L_kCFAllocatorDefault$non_lazy_ptr-4-(L283)
	.long	LC68-4-(L284)
	.long	LC69-4-(L286)
	.long	LC70-4-(L288)
	.align 2
	.globl _NPN_GetStringIdentifier
	.thumb
	.thumb_func _NPN_GetStringIdentifier
_NPN_GetStringIdentifier:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r4, r5, r7, lr}
	add	r7, sp, #8
	mov	r4, r0
	blx	L_strlen$stub
	mov	r1, r4
	mov	r2, r0
	movs	r0, #0
	blx	L_CFDataCreate$stub
	mov	r4, r0
	ldr	r0, L300
	mov	r1, r4
L294:
	add	r0, pc
	ldr	r0, [r0]
	blx	L_CFSetGetValue$stub
	cbz	r0, L295
	mov	r5, r0
	b	L297
L295:
	ldr	r0, L300+4
	mov	r1, r4
	mov	r5, r4
L298:
	add	r0, pc
	ldr	r0, [r0]
	blx	L_CFSetAddValue$stub
L297:
	mov	r0, r4
	blx	L_CFRelease$stub
	mov	r0, r5
	pop	{r4, r5, r7, pc}
	.p2align 2
L301:
	.align 2
L300:
	.long	_identifier_set-4-(L294)
	.long	_identifier_set-4-(L298)
	.cstring
	.align 2
LC71:
	.ascii "location\0"
	.align 2
LC72:
	.ascii "file://some.html\0"
	.text
	.align 2
	.globl _fakeGetProperty
	.thumb
	.thumb_func _fakeGetProperty
_fakeGetProperty:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r4, r5, r7, lr}
	add	r7, sp, #8
	sub	sp, sp, #16
	ldr	r0, L310
	mov	r4, r1
	mov	r5, r2
L303:
	add	r0, pc
	bl	_NPN_GetStringIdentifier
	cmp	r0, r4
	bne	L304
	ldr	r1, L310+4
	mov	r0, sp
	mov	r4, sp
L306:
	add	r1, pc
	bl	_new_fucked_up_variant_string
	b	L309
L304:
	bl	_new_fake_object
	mov	r4, sp
	mov	r1, r0
	mov	r0, sp
	bl	_new_fucked_up_variant_object
L309:
	ldmia	r4, {r0, r1, r2, r3}
	stmia	r5, {r0, r1, r2, r3}
	movs	r0, #1
	sub	sp, r7, #8
	pop	{r4, r5, r7, pc}
	.p2align 2
L311:
	.align 2
L310:
	.long	LC71-4-(L303)
	.long	LC72-4-(L306)
	.align 2
	.globl _init_identifier_dict
	.thumb
	.thumb_func _init_identifier_dict
_init_identifier_dict:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r7, lr}
	add	r7, sp, #0
	ldr	r2, L316
	movs	r0, #0
L313:
	add	r2, pc
	mov	r1, r0
	ldr	r2, [r2]
	blx	L_CFSetCreateMutable$stub
	ldr	r3, L316+4
L314:
	add	r3, pc
	str	r0, [r3]
	pop	{r7, pc}
	.p2align 2
L317:
	.align 2
L316:
	.long	L_kCFTypeSetCallBacks$non_lazy_ptr-4-(L313)
	.long	_identifier_set-4-(L314)
	.mod_init_func
	.align 2
	.long	_init_identifier_dict
	.cstring
	.align 2
LC73:
	.ascii "fakeAllocate\0"
	.align 2
LC74:
	.ascii "fakeDeallocate\0"
	.align 2
LC75:
	.ascii "fakeHasMethod\0"
	.align 2
LC76:
	.ascii "fakeInvoke\0"
	.align 2
LC77:
	.ascii "fakeInvokeDefault\0"
	.align 2
LC78:
	.ascii "fakeHasProperty\0"
	.align 2
LC79:
	.ascii "fakeGetProperty\0"
	.align 2
LC80:
	.ascii "fakeSetProperty\0"
	.align 2
LC81:
	.ascii "fakeRemoveProperty\0"
	.align 2
LC82:
	.ascii "fakeEnumeration\0"
	.align 2
LC83:
	.ascii "fakeConstruct\0"
	.align 2
LC84:
	.ascii "NPN_GetValue\0"
	.align 2
LC85:
	.ascii "NPN_SetValue\0"
	.align 2
LC86:
	.ascii "NPN_RetainObject\0"
	.align 2
LC87:
	.ascii "NPN_ReleaseObject\0"
	.align 2
LC88:
	.ascii "NPN_MemAlloc\0"
	.align 2
LC89:
	.ascii "NPN_MemFree\0"
	.align 2
LC90:
	.ascii "NPN_GetStringIdentifier\0"
	.align 2
LC91:
	.ascii "NPN_GetIntIdentifier\0"
	.align 2
LC92:
	.ascii "NPN_IdentifierIsString\0"
	.align 2
LC93:
	.ascii "NPN_UTF8FromIdentifier\0"
	.align 2
LC94:
	.ascii "NPN_IntFromIdentifier\0"
	.align 2
LC95:
	.ascii "NPN_GetProperty\0"
	.align 2
LC96:
	.ascii "NPN_ReleaseVariantValue\0"
	.align 2
LC97:
	.ascii "NPN_GetURLNotify\0"
	.align 2
LC98:
	.ascii "NPN_ScheduleTimer\0"
	.align 2
LC99:
	.ascii "NPN_UnscheduleTimer\0"
	.align 2
LC100:
	.ascii "NPN_Evaluate\0"
	.align 2
LC101:
	.ascii "Taking the plunge to %p\0"
	.section __DATA, __cfstring
	.align 2
LC102:
	.long	___CFConstantStringClassReference
	.long	1992
	.long	LC101
	.long	23
	.cstring
	.align 2
LC103:
	.ascii "NP_Initialize return value: %d\0"
	.section __DATA, __cfstring
	.align 2
LC104:
	.long	___CFConstantStringClassReference
	.long	1992
	.long	LC103
	.long	30
	.cstring
	.align 2
LC105:
	.ascii "!ret\0"
	.align 2
LC106:
	.ascii "!get_src(food, src)\0"
	.align 2
LC107:
	.ascii "tl\0"
	.align 2
LC108:
	.ascii "transparent\0"
	.align 2
LC109:
	.ascii "noorder\0"
	.align 2
LC110:
	.ascii "application/x-shockwave-flash\0"
	.align 2
LC111:
	.ascii "NPP_New return value: %d\0"
	.section __DATA, __cfstring
	.align 2
LC112:
	.long	___CFConstantStringClassReference
	.long	1992
	.long	LC111
	.long	24
	.cstring
	.align 2
LC113:
	.ascii "!pluginfuncs.setwindow(&nppt, &window)\0"
	.align 2
LC114:
	.ascii "!pluginfuncs.getvalue(&nppt, NPPVpluginScriptableNPObject, &scriptable)\0"
	.align 2
LC115:
	.ascii "FFFF\0"
	.align 2
LC116:
	.ascii "file://some.swf\0"
	.align 2
LC117:
	.ascii "!ret && stype == NP_NORMAL\0"
	.align 2
LC118:
	.ascii "ret > size\0"
	.align 2
LC119:
	.ascii "buf != MAP_FAILED\0"
	.align 2
LC120:
	.ascii "size == ret\0"
	.text
	.align 2
	.globl _go
	.thumb
	.thumb_func _go
_go:
	@ args = 0, pretend = 0, frame = 1092
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, lr}
	add	r7, sp, #12
	push	{r8, r10, r11}
	sub	sp, sp, #1104
	mov	r8, r0
	mov	r5, r1
	ldr	r0, L524
	ldr	r1, L524+4
	movs	r2, #1
L319:
	add	r0, pc
L320:
	add	r1, pc
	blx	L_stubify$stub
	ldr	r4, L524+8
	ldr	r1, L524+12
	movs	r2, #1
L321:
	add	r4, pc
L323:
	add	r1, pc
	str	r0, [r4, #4]
	ldr	r0, L524+16
L322:
	add	r0, pc
	blx	L_stubify$stub
	ldr	r3, L524+20
	ldr	r1, L524+24
	movs	r2, #1
L327:
	add	r1, pc
	str	r3, [r4, #12]
	str	r0, [r4, #8]
	ldr	r0, L524+28
L326:
	add	r0, pc
	blx	L_stubify$stub
	ldr	r1, L524+32
	movs	r2, #1
L330:
	add	r1, pc
	str	r0, [r4, #16]
	ldr	r0, L524+36
L329:
	add	r0, pc
	blx	L_stubify$stub
	ldr	r1, L524+40
	movs	r2, #1
L333:
	add	r1, pc
	str	r0, [r4, #20]
	ldr	r0, L524+44
L332:
	add	r0, pc
	blx	L_stubify$stub
	ldr	r1, L524+48
	movs	r2, #1
L336:
	add	r1, pc
	str	r0, [r4, #24]
	ldr	r0, L524+52
L335:
	add	r0, pc
	blx	L_stubify$stub
	ldr	r1, L524+56
	movs	r2, #1
L339:
	add	r1, pc
	str	r0, [r4, #28]
	ldr	r0, L524+60
L338:
	add	r0, pc
	blx	L_stubify$stub
	ldr	r1, L524+64
	movs	r2, #1
L342:
	add	r1, pc
	str	r0, [r4, #32]
	ldr	r0, L524+68
L341:
	add	r0, pc
	blx	L_stubify$stub
	ldr	r1, L524+72
	movs	r2, #1
L345:
	add	r1, pc
	str	r0, [r4, #36]
	ldr	r0, L524+76
L344:
	add	r0, pc
	blx	L_stubify$stub
	ldr	r1, L524+80
	movs	r2, #1
L348:
	add	r1, pc
	str	r0, [r4, #40]
	ldr	r0, L524+84
L347:
	add	r0, pc
	blx	L_stubify$stub
	ldr	r1, L524+88
	movs	r2, #1
L351:
	add	r1, pc
	str	r0, [r4, #44]
	ldr	r0, L524+92
L350:
	add	r0, pc
	blx	L_stubify$stub
	ldr	r3, L524+96
	movs	r2, #212
	mov	r1, #1077952576
L353:
	add	r3, pc
	ldr	r3, [r3]
	str	r5, [r3]
	ldr	r5, L524+100
L411:
	add	r5, pc
	str	r0, [r4, #48]
	ldr	r4, L524+104
L354:
	add	r4, pc
	mov	r0, r4
	blx	L_pattern$stub
	ldr	r0, L524+108
	ldr	r1, L524+112
	mov	r2, #212	@ movhi
L357:
	add	r0, pc
	strh	r2, [r4]	@ movhi
L358:
	add	r1, pc
	movs	r2, #1
	movw	r3, 9999	@ movhi
	strh	r3, [r4, #2]	@ movhi
	blx	L_stubify$stub
	ldr	r1, L524+116
	movs	r2, #1
L361:
	add	r1, pc
	str	r0, [r4, #68]
	ldr	r0, L524+120
L360:
	add	r0, pc
	blx	L_stubify$stub
	ldr	r1, L524+124
	movs	r2, #1
L364:
	add	r1, pc
	str	r0, [r4, #72]
	ldr	r0, L524+128
L363:
	add	r0, pc
	blx	L_stubify$stub
	ldr	r1, L524+132
	movs	r2, #1
L367:
	add	r1, pc
	str	r0, [r4, #112]
	ldr	r0, L524+136
L366:
	add	r0, pc
	blx	L_stubify$stub
	ldr	r1, L524+140
	movs	r2, #1
L370:
	add	r1, pc
	str	r0, [r4, #116]
	ldr	r0, L524+144
L369:
	add	r0, pc
	blx	L_stubify$stub
	ldr	r1, L524+148
	movs	r2, #1
L373:
	add	r1, pc
	str	r0, [r4, #120]
	ldr	r0, L524+152
L372:
	add	r0, pc
	blx	L_stubify$stub
	ldr	r1, L524+156
	movs	r2, #1
L376:
	add	r1, pc
	str	r0, [r4, #36]
	ldr	r0, L524+160
L375:
	add	r0, pc
	blx	L_stubify$stub
	ldr	r1, L524+164
	movs	r2, #1
L379:
	add	r1, pc
	str	r0, [r4, #40]
	ldr	r0, L524+168
L378:
	add	r0, pc
	blx	L_stubify$stub
	ldr	r1, L524+172
	movs	r2, #1
L382:
	add	r1, pc
	str	r0, [r4, #88]
	ldr	r0, L524+176
L381:
	add	r0, pc
	blx	L_stubify$stub
	ldr	r1, L524+180
	movs	r2, #1
L385:
	add	r1, pc
	str	r0, [r4, #96]
	ldr	r0, L524+184
L384:
	add	r0, pc
	blx	L_stubify$stub
	ldr	r1, L524+188
	movs	r2, #1
L388:
	add	r1, pc
	str	r0, [r4, #100]
	ldr	r0, L524+192
L387:
	add	r0, pc
	blx	L_stubify$stub
	ldr	r1, L524+196
	movs	r2, #1
L391:
	add	r1, pc
	str	r0, [r4, #104]
	ldr	r0, L524+200
L390:
	add	r0, pc
	blx	L_stubify$stub
	ldr	r1, L524+204
	movs	r2, #1
L394:
	add	r1, pc
	str	r0, [r4, #108]
	ldr	r0, L524+208
L393:
	add	r0, pc
	blx	L_stubify$stub
	ldr	r1, L524+212
	movs	r2, #1
L397:
	add	r1, pc
	str	r0, [r4, #136]
	ldr	r0, L524+216
L396:
	add	r0, pc
	blx	L_stubify$stub
	ldr	r1, L524+220
	movs	r2, #1
L400:
	add	r1, pc
	str	r0, [r4, #156]
	ldr	r0, L524+224
L399:
	add	r0, pc
	blx	L_stubify$stub
	ldr	r1, L524+228
	movs	r2, #1
L403:
	add	r1, pc
	str	r0, [r4, #60]
	ldr	r0, L524+232
L402:
	add	r0, pc
	blx	L_stubify$stub
	ldr	r1, L524+236
	movs	r2, #1
L406:
	add	r1, pc
	str	r0, [r4, #196]
	ldr	r0, L524+240
L405:
	add	r0, pc
	blx	L_stubify$stub
	ldr	r1, L524+244
	movs	r2, #1
L409:
	add	r1, pc
	str	r0, [r4, #200]
	ldr	r0, L524+248
L408:
	add	r0, pc
	blx	L_stubify$stub
	ldr	r1, L524+252
	movs	r2, #60
	str	r0, [r4, #132]
	mov	r0, r5
	blx	L_pattern$stub
	mov	r2, #60	@ movhi
	strh	r2, [r5]	@ movhi
	blx	L_init_jni$stub
	ldr	r3, L524+256
	ldr	r2, L524+260
	movs	r1, #0
L413:
	add	r3, pc
L414:
	add	r2, pc
	ldr	r0, [r3]
	mov	r3, r8
	ldr	r6, [r0]
	mov	r0, r6
	blx	L_CFStringCreateWithFormat$stub
	blx	L_logreal_$stub
	ldr	r2, L524+264
	mov	r0, r4
	mov	r1, r5
L417:
	add	r2, pc
	ldr	r3, L524+268
	ldr	r2, [r2]
	blx	r8
	ldr	r2, L524+272
	movs	r1, #0
L418:
	add	r2, pc
	uxth	r4, r0
	sxth	r3, r0
	mov	r0, r6
	blx	L_CFStringCreateWithFormat$stub
	blx	L_logreal_$stub
	cbz	r4, L419
	ldr	r0, L524+276
	ldr	r1, L524+280
	ldr	r3, L524+284
L421:
	add	r0, pc
L422:
	add	r1, pc
L423:
	add	r3, pc
	movw	r2, #469
	b	L523
L419:
	ldr	r0, L524+288
	add	r4, sp, #16
	subs	r4, r4, #4
L424:
	add	r0, pc
	mov	r1, r4
	ldr	r0, [r0]
	ldr	r0, [r0]
	blx	L_get_src$stub
	mov	r5, r0
	cbz	r0, L425
	ldr	r0, L524+292
	ldr	r1, L524+296
	ldr	r3, L524+300
L427:
	add	r0, pc
L428:
	add	r1, pc
L429:
	add	r3, pc
	mov	r2, #472
L523:
	blx	L___assert_rtn$stub
L425:
	ldr	r0, L524+304
	add	ip, sp, #1080
	str	r4, [sp, #1064]
L430:
	add	r0, pc
	ldmia	r0, {r0, r1, r2, r3}
	stmia	ip, {r0, r1, r2, r3}
	ldr	r3, L524+308
	ldr	r1, L524+312
	ldr	r2, L524+316
L431:
	add	r3, pc
L436:
	add	r1, pc
	str	r3, [sp, #1068]
	ldr	r3, L524+320
	ldr	r11, [r1]
	ldr	r0, L524+324
L432:
	add	r3, pc
	str	r3, [sp, #1072]
	ldr	r3, L524+328
L434:
	add	r2, pc
	str	ip, [sp]
L433:
	add	r3, pc
	str	r5, [sp, #8]
	str	r3, [sp, #1076]
	mov	r1, r11
	add	r3, sp, #1064
L435:
	add	r0, pc
	str	r3, [sp, #4]
	movs	r3, #4
	ldr	r4, [r2, #4]
	movs	r2, #1
	blx	r4
	ldr	r2, L524+332
	mov	r1, r5
L437:
	add	r2, pc
	uxth	r4, r0
	sxth	r3, r0
	mov	r0, r6
	blx	L_CFStringCreateWithFormat$stub
	blx	L_logreal_$stub
	cbz	r4, L438
	ldr	r0, L524+336
	ldr	r1, L524+340
	ldr	r3, L524+344
L440:
	add	r0, pc
L441:
	add	r1, pc
L442:
	add	r3, pc
	mov	r2, #478
	b	L523
L438:
	ldr	r3, L524+348
	ldr	r1, L524+352
	movs	r5, #2
L443:
	add	r3, pc
L444:
	add	r1, pc
	ldr	r3, [r3]
	str	r4, [r1, #4]
	str	r4, [r1, #8]
	strh	r4, [r1, #22]	@ movhi
	ldr	r2, [r3]
	ldr	r3, L524+356
	strh	r4, [r1, #20]	@ movhi
	str	r5, [r1, #28]
L445:
	add	r3, pc
	str	r2, [r1, #12]
	ldr	r3, [r3]
	strh	r2, [r1, #24]	@ movhi
	mov	r0, r11
	ldr	r3, [r3]
	str	r3, [r1, #16]
	strh	r3, [r1, #26]	@ movhi
	ldr	r3, L524+360
L454:
	add	r3, pc
	ldr	r3, [r3, #12]
	blx	r3
	uxth	r0, r0
	cbz	r0, L457
	ldr	r0, L524+364
	ldr	r1, L524+368
	ldr	r3, L524+372
L459:
	add	r0, pc
L460:
	add	r1, pc
L461:
	add	r3, pc
	mov	r2, #486
	b	L523
L457:
	ldr	r3, L524+376
	ldr	r2, L524+380
	mov	r0, r11
L462:
	add	r3, pc
L464:
	add	r2, pc
	ldr	r3, [r3, #52]
	movs	r1, #15
	blx	r3
	uxth	r4, r0
	cbz	r4, L465
	ldr	r0, L524+384
	ldr	r1, L524+388
	ldr	r3, L524+392
L467:
	add	r0, pc
L468:
	add	r1, pc
L469:
	add	r3, pc
	mov	r2, #488
	b	L523
L465:
	ldr	r0, L524+396
	mov	r1, r4
L470:
	add	r0, pc
	blx	L_open$stub
	mov	r3, r5
	movs	r2, #0
	movs	r1, #0
	mov	r8, r0
	blx	L_lseek$stub
	movs	r2, #0
	mov	r3, r4
	mov	r5, r0
	mov	r6, r1
	mov	r0, r8
	movs	r1, #0
	blx	L_lseek$stub
	ldr	r3, L524+400
	ldr	r1, L524+404
	add	r2, sp, #1088
L471:
	add	r3, pc
	str	r3, [sp, #1044]
	ldr	r3, L524+408
	adds	r2, r2, #14
	str	r2, [sp]
L472:
	add	r3, pc
	add	r2, sp, #1024
	ldr	ip, [r3, #16]
	str	r4, [sp, #1036]
	str	r4, [sp, #1040]
	str	r5, [sp, #1048]
	str	r4, [sp, #1052]
	str	r4, [sp, #1056]
	str	r4, [sp, #1060]
L474:
	add	r1, pc
	mov	r0, r11
	adds	r2, r2, #12
	mov	r3, r4
	blx	ip
	mov	r10, r5
	uxth	r0, r0
	cbnz	r0, L475
	ldrh	r3, [sp, #1102]
	cmp	r3, #1
	beq	L477
L475:
	ldr	r0, L524+412
	ldr	r1, L524+416
	ldr	r3, L524+420
L478:
	add	r0, pc
L479:
	add	r1, pc
L480:
	add	r3, pc
	movw	r2, #505
	b	L523
L481:
	ldr	r0, L524+424
	ldr	r1, L524+428
	ldr	r3, L524+432
L482:
	add	r0, pc
L483:
	add	r1, pc
L484:
	add	r3, pc
	mov	r2, #510
	b	L523
L485:
	movs	r2, #0
	movs	r3, #0
	movs	r0, #0
	str	r2, [sp, #4]
	str	r3, [sp, #8]
	mov	r1, r10
	movs	r2, #1
	movs	r3, #2
	str	r8, [sp]
	blx	L_mmap$stub
	cmp	r0, #-1
	mov	r2, r0
	bne	L486
	ldr	r0, L524+436
	ldr	r1, L524+440
	ldr	r3, L524+444
L488:
	add	r0, pc
L489:
	add	r1, pc
L490:
	add	r3, pc
	movw	r2, #513
	b	L523
L486:
	ldr	r0, L524+448
	ldr	r3, L524+452
	add	r1, sp, #1024
L492:
	add	r0, pc
L491:
	add	r3, pc
	ldr	r8, [r0]
	str	r2, [sp]
	adds	r1, r1, #12
	movs	r2, #0
	ldr	r4, [r3, #32]
	mov	r0, r8
	mov	r3, r5
	blx	r4
	cmp	r0, r5
	mov	r2, r0
	asr	r3, r2, #31
	bne	L495
	cmp	r6, r3
	beq	L493
L495:
	ldr	r0, L524+456
	ldr	r1, L524+460
	ldr	r3, L524+464
L496:
	add	r0, pc
L497:
	add	r1, pc
L498:
	add	r3, pc
	movw	r2, #515
	b	L523
L493:
	ldr	r3, L524+468
	add	r1, sp, #1024
	mov	r0, r8
L499:
	add	r3, pc
	adds	r1, r1, #12
	ldr	r3, [r3, #20]
	movs	r2, #0
	blx	r3
	uxth	r0, r0
	cbz	r0, L501
	ldr	r0, L524+472
	ldr	r1, L524+476
	ldr	r3, L524+480
L503:
	add	r0, pc
L504:
	add	r1, pc
L505:
	add	r3, pc
	movw	r2, #518
	b	L523
L501:
	ldr	r3, L524+484
	mov	r0, r8
	mov	r1, #2000
L506:
	add	r3, pc
	add	r2, sp, #1096
	ldr	r3, [r3, #52]
	blx	r3
	uxth	r0, r0
	cbz	r0, L508
	ldr	r0, L524+488
	ldr	r1, L524+492
	ldr	r3, L524+496
L510:
	add	r0, pc
L511:
	add	r1, pc
L512:
	add	r3, pc
	movw	r2, #522
	b	L523
L508:
	ldr	r3, L524+500
L513:
	add	r3, pc
	ldr	r0, [r3]
	ldr	r4, [r0]
L514:
	movs	r1, #0
	movs	r2, #0
	movs	r3, #1
	mov	r0, r4
	blx	L_CFRunLoopRunInMode$stub
	cmp	r0, #4
	beq	L514
	movs	r0, #5
	blx	L_post_lifecycle_event$stub
	movs	r0, #8
	blx	L_post_lifecycle_event$stub
	ldr	r0, L524+504
	movs	r1, #0
L516:
	add	r0, pc
	blx	L_do_later$stub
	blx	L_CFRunLoopRun$stub
	sub	sp, r7, #24
	pop	{r8, r10, r11}
	pop	{r4, r5, r6, r7, pc}
L477:
	ldr	r3, L524+508
	add	r1, sp, #1024
	adds	r1, r1, #12
L517:
	add	r3, pc
	mov	r0, r11
	ldr	r3, [r3, #28]
	blx	r3
	mov	r0, r0
	asr	r1, r0, #31
	cmp	r1, r6
	bgt	L485
	bne	L481
	cmp	r0, r5
	bhi	L485
	b	L481
	.p2align 2
L525:
	.align 2
L524:
	.long	_fakeAllocate-4-(L319)
	.long	LC73-4-(L320)
	.long	_fake_class-4-(L321)
	.long	LC74-4-(L323)
	.long	_fakeDeallocate-4-(L322)
	.long	-559038749
	.long	LC75-4-(L327)
	.long	_fakeHasMethod-4-(L326)
	.long	LC76-4-(L330)
	.long	_fakeInvoke-4-(L329)
	.long	LC77-4-(L333)
	.long	_fakeInvokeDefault-4-(L332)
	.long	LC78-4-(L336)
	.long	_fakeHasProperty-4-(L335)
	.long	LC79-4-(L339)
	.long	_fakeGetProperty-4-(L338)
	.long	LC80-4-(L342)
	.long	_fakeSetProperty-4-(L341)
	.long	LC81-4-(L345)
	.long	_fakeRemoveProperty-4-(L344)
	.long	LC82-4-(L348)
	.long	_fakeEnumeration-4-(L347)
	.long	LC83-4-(L351)
	.long	_fakeConstruct-4-(L350)
	.long	L_JNI_OnLoad_ptr$non_lazy_ptr-4-(L353)
	.long	_pluginfuncs-4-(L411)
	.long	_funcs-4-(L354)
	.long	_NPN_GetValue-4-(L357)
	.long	LC84-4-(L358)
	.long	LC85-4-(L361)
	.long	_NPN_SetValue-4-(L360)
	.long	LC50-4-(L364)
	.long	_NPN_CreateObject-4-(L363)
	.long	LC86-4-(L367)
	.long	_NPN_RetainObject-4-(L366)
	.long	LC87-4-(L370)
	.long	_NPN_ReleaseObject-4-(L369)
	.long	LC88-4-(L373)
	.long	_NPN_MemAlloc-4-(L372)
	.long	LC89-4-(L376)
	.long	_NPN_MemFree-4-(L375)
	.long	LC90-4-(L379)
	.long	_NPN_GetStringIdentifier-4-(L378)
	.long	LC91-4-(L382)
	.long	_NPN_GetIntIdentifier-4-(L381)
	.long	LC92-4-(L385)
	.long	_NPN_IdentifierIsString-4-(L384)
	.long	LC93-4-(L388)
	.long	_NPN_UTF8FromIdentifier-4-(L387)
	.long	LC94-4-(L391)
	.long	_NPN_IntFromIdentifier-4-(L390)
	.long	LC95-4-(L394)
	.long	_NPN_GetProperty-4-(L393)
	.long	LC96-4-(L397)
	.long	_NPN_ReleaseVariantValue-4-(L396)
	.long	LC97-4-(L400)
	.long	_NPN_GetURLNotify-4-(L399)
	.long	LC98-4-(L403)
	.long	_NPN_ScheduleTimer-4-(L402)
	.long	LC99-4-(L406)
	.long	_NPN_UnscheduleTimer-4-(L405)
	.long	LC100-4-(L409)
	.long	_NPN_Evaluate-4-(L408)
	.long	-1091633152
	.long	L_kCFAllocatorDefault$non_lazy_ptr-4-(L413)
	.long	LC102-4-(L414)
	.long	L_env$non_lazy_ptr-4-(L417)
	.long	-559030611
	.long	LC104-4-(L418)
	.long	___func__.13997-4-(L421)
	.long	LC26-4-(L422)
	.long	LC105-4-(L423)
	.long	L_food$non_lazy_ptr-4-(L424)
	.long	___func__.13997-4-(L427)
	.long	LC26-4-(L428)
	.long	LC106-4-(L429)
	.long	_C.108.14023-4-(L430)
	.long	LC107-4-(L431)
	.long	L_nppt$non_lazy_ptr-4-(L436)
	.long	_pluginfuncs-4-(L434)
	.long	LC108-4-(L432)
	.long	LC110-4-(L435)
	.long	LC109-4-(L433)
	.long	LC112-4-(L437)
	.long	___func__.13997-4-(L440)
	.long	LC26-4-(L441)
	.long	LC105-4-(L442)
	.long	L_movie_w$non_lazy_ptr-4-(L443)
	.long	_window-4-(L444)
	.long	L_movie_h$non_lazy_ptr-4-(L445)
	.long	_pluginfuncs-4-(L454)
	.long	___func__.13997-4-(L459)
	.long	LC26-4-(L460)
	.long	LC113-4-(L461)
	.long	_pluginfuncs-4-(L462)
	.long	_scriptable-4-(L464)
	.long	___func__.13997-4-(L467)
	.long	LC26-4-(L468)
	.long	LC114-4-(L469)
	.long	LC115-4-(L470)
	.long	LC116-4-(L471)
	.long	LC110-4-(L474)
	.long	_pluginfuncs-4-(L472)
	.long	___func__.13997-4-(L478)
	.long	LC26-4-(L479)
	.long	LC117-4-(L480)
	.long	___func__.13997-4-(L482)
	.long	LC26-4-(L483)
	.long	LC118-4-(L484)
	.long	___func__.13997-4-(L488)
	.long	LC26-4-(L489)
	.long	LC119-4-(L490)
	.long	L_nppt$non_lazy_ptr-4-(L492)
	.long	_pluginfuncs-4-(L491)
	.long	___func__.13997-4-(L496)
	.long	LC26-4-(L497)
	.long	LC120-4-(L498)
	.long	_pluginfuncs-4-(L499)
	.long	___func__.13997-4-(L503)
	.long	LC26-4-(L504)
	.long	LC105-4-(L505)
	.long	_pluginfuncs-4-(L506)
	.long	___func__.13997-4-(L510)
	.long	LC26-4-(L511)
	.long	LC105-4-(L512)
	.long	L_kCFRunLoopDefaultMode$non_lazy_ptr-4-(L513)
	.long	_later-4-(L516)
	.long	_pluginfuncs-4-(L517)
	.cstring
	.align 2
___func__.13997:
	.ascii "go\0"
	.align 2
LC121:
	.ascii "src\0"
	.align 2
LC122:
	.ascii "salign\0"
	.align 2
LC123:
	.ascii "wmode\0"
	.align 2
LC124:
	.ascii "scale\0"
	.const_data
	.align 2
_C.108.14023:
	.long	LC121
	.long	LC122
	.long	LC123
	.long	LC124
	.data
	.align 2
_fake_class:
	.long	3
	.long	_fakeAllocate
	.long	_fakeDeallocate
	.long	-559038749
	.long	_fakeHasMethod
	.long	_fakeInvoke
	.long	_fakeInvokeDefault
	.long	_fakeHasProperty
	.long	_fakeGetProperty
	.long	_fakeSetProperty
	.long	_fakeRemoveProperty
	.long	_fakeEnumeration
	.long	_fakeConstruct
	.cstring
	.align 2
___func__.13905:
	.ascii "handle_event\0"
	.align 2
___func__.13829:
	.ascii "NPN_GetURLNotify\0"
	.align 2
___func__.13789:
	.ascii "func\0"
	.align 2
___func__.13733:
	.ascii "func\0"
	.align 2
___func__.13665:
	.ascii "NPN_SetValue\0"
	.align 2
___func__.13484:
	.ascii "NPN_IntFromIdentifier\0"
.lcomm _pluginfuncs,60,2
.lcomm _funcs,212,2
.lcomm _scriptable,4,2
.lcomm _window,32,2
.lcomm _identifier_set,4,2
.comm _food,4,2
.comm _nppt,8,2
	.section __TEXT,__picsymbolstub4,symbol_stubs,none,16
	.align 2
	.code 32
L_CFNumberGetValue$stub:
	.indirect_symbol _CFNumberGetValue
	ldr	ip, L_CFNumberGetValue$slp
L1$scv:	add	ip, pc, ip
	ldr	pc, [ip, #0]
L_CFNumberGetValue$slp:
	.long	L_CFNumberGetValue$lazy_ptr - (L1$scv + 8)
	.lazy_symbol_pointer
L_CFNumberGetValue$lazy_ptr:
	.indirect_symbol	_CFNumberGetValue
	.long	dyld_stub_binding_helper
	.section __TEXT,__picsymbolstub4,symbol_stubs,none,16
	.align 2
	.code 32
L_CFNumberCreate$stub:
	.indirect_symbol _CFNumberCreate
	ldr	ip, L_CFNumberCreate$slp
L2$scv:	add	ip, pc, ip
	ldr	pc, [ip, #0]
L_CFNumberCreate$slp:
	.long	L_CFNumberCreate$lazy_ptr - (L2$scv + 8)
	.lazy_symbol_pointer
L_CFNumberCreate$lazy_ptr:
	.indirect_symbol	_CFNumberCreate
	.long	dyld_stub_binding_helper
	.section __TEXT,__picsymbolstub4,symbol_stubs,none,16
	.align 2
	.code 32
L_CFDataGetLength$stub:
	.indirect_symbol _CFDataGetLength
	ldr	ip, L_CFDataGetLength$slp
L3$scv:	add	ip, pc, ip
	ldr	pc, [ip, #0]
L_CFDataGetLength$slp:
	.long	L_CFDataGetLength$lazy_ptr - (L3$scv + 8)
	.lazy_symbol_pointer
L_CFDataGetLength$lazy_ptr:
	.indirect_symbol	_CFDataGetLength
	.long	dyld_stub_binding_helper
	.section __TEXT,__picsymbolstub4,symbol_stubs,none,16
	.align 2
	.code 32
L_pattern$stub:
	.indirect_symbol _pattern
	ldr	ip, L_pattern$slp
L4$scv:	add	ip, pc, ip
	ldr	pc, [ip, #0]
L_pattern$slp:
	.long	L_pattern$lazy_ptr - (L4$scv + 8)
	.lazy_symbol_pointer
L_pattern$lazy_ptr:
	.indirect_symbol	_pattern
	.long	dyld_stub_binding_helper
	.section __TEXT,__picsymbolstub4,symbol_stubs,none,16
	.align 2
	.code 32
L_stubify$stub:
	.indirect_symbol _stubify
	ldr	ip, L_stubify$slp
L5$scv:	add	ip, pc, ip
	ldr	pc, [ip, #0]
L_stubify$slp:
	.long	L_stubify$lazy_ptr - (L5$scv + 8)
	.lazy_symbol_pointer
L_stubify$lazy_ptr:
	.indirect_symbol	_stubify
	.long	dyld_stub_binding_helper
	.non_lazy_symbol_pointer
L_movie_h$non_lazy_ptr:
	.indirect_symbol _movie_h
	.long	0
L_android_context$non_lazy_ptr:
	.indirect_symbol _android_context
	.long	0
	.section __TEXT,__picsymbolstub4,symbol_stubs,none,16
	.align 2
	.code 32
L_CFRunLoopTimerCreate$stub:
	.indirect_symbol _CFRunLoopTimerCreate
	ldr	ip, L_CFRunLoopTimerCreate$slp
L6$scv:	add	ip, pc, ip
	ldr	pc, [ip, #0]
L_CFRunLoopTimerCreate$slp:
	.long	L_CFRunLoopTimerCreate$lazy_ptr - (L6$scv + 8)
	.lazy_symbol_pointer
L_CFRunLoopTimerCreate$lazy_ptr:
	.indirect_symbol	_CFRunLoopTimerCreate
	.long	dyld_stub_binding_helper
	.section __TEXT,__picsymbolstub4,symbol_stubs,none,16
	.align 2
	.code 32
L_CFDataCreate$stub:
	.indirect_symbol _CFDataCreate
	ldr	ip, L_CFDataCreate$slp
L7$scv:	add	ip, pc, ip
	ldr	pc, [ip, #0]
L_CFDataCreate$slp:
	.long	L_CFDataCreate$lazy_ptr - (L7$scv + 8)
	.lazy_symbol_pointer
L_CFDataCreate$lazy_ptr:
	.indirect_symbol	_CFDataCreate
	.long	dyld_stub_binding_helper
	.non_lazy_symbol_pointer
L_movie_w$non_lazy_ptr:
	.indirect_symbol _movie_w
	.long	0
	.section __TEXT,__picsymbolstub4,symbol_stubs,none,16
	.align 2
	.code 32
L_strdup$stub:
	.indirect_symbol _strdup
	ldr	ip, L_strdup$slp
L8$scv:	add	ip, pc, ip
	ldr	pc, [ip, #0]
L_strdup$slp:
	.long	L_strdup$lazy_ptr - (L8$scv + 8)
	.lazy_symbol_pointer
L_strdup$lazy_ptr:
	.indirect_symbol	_strdup
	.long	dyld_stub_binding_helper
	.section __TEXT,__picsymbolstub4,symbol_stubs,none,16
	.align 2
	.code 32
L_free$stub:
	.indirect_symbol _free
	ldr	ip, L_free$slp
L9$scv:	add	ip, pc, ip
	ldr	pc, [ip, #0]
L_free$slp:
	.long	L_free$lazy_ptr - (L9$scv + 8)
	.lazy_symbol_pointer
L_free$lazy_ptr:
	.indirect_symbol	_free
	.long	dyld_stub_binding_helper
	.section __TEXT,__picsymbolstub4,symbol_stubs,none,16
	.align 2
	.code 32
L_malloc$stub:
	.indirect_symbol _malloc
	ldr	ip, L_malloc$slp
L10$scv:	add	ip, pc, ip
	ldr	pc, [ip, #0]
L_malloc$slp:
	.long	L_malloc$lazy_ptr - (L10$scv + 8)
	.lazy_symbol_pointer
L_malloc$lazy_ptr:
	.indirect_symbol	_malloc
	.long	dyld_stub_binding_helper
	.section __TEXT,__picsymbolstub4,symbol_stubs,none,16
	.align 2
	.code 32
L_post_lifecycle_event$stub:
	.indirect_symbol _post_lifecycle_event
	ldr	ip, L_post_lifecycle_event$slp
L11$scv:	add	ip, pc, ip
	ldr	pc, [ip, #0]
L_post_lifecycle_event$slp:
	.long	L_post_lifecycle_event$lazy_ptr - (L11$scv + 8)
	.lazy_symbol_pointer
L_post_lifecycle_event$lazy_ptr:
	.indirect_symbol	_post_lifecycle_event
	.long	dyld_stub_binding_helper
	.non_lazy_symbol_pointer
L_food$non_lazy_ptr:
	.indirect_symbol _food
	.long	0
	.section __TEXT,__picsymbolstub4,symbol_stubs,none,16
	.align 2
	.code 32
L_logreal_$stub:
	.indirect_symbol _logreal_
	ldr	ip, L_logreal_$slp
L12$scv:	add	ip, pc, ip
	ldr	pc, [ip, #0]
L_logreal_$slp:
	.long	L_logreal_$lazy_ptr - (L12$scv + 8)
	.lazy_symbol_pointer
L_logreal_$lazy_ptr:
	.indirect_symbol	_logreal_
	.long	dyld_stub_binding_helper
	.section __TEXT,__picsymbolstub4,symbol_stubs,none,16
	.align 2
	.code 32
L_CFStringCreateWithFormat$stub:
	.indirect_symbol _CFStringCreateWithFormat
	ldr	ip, L_CFStringCreateWithFormat$slp
L13$scv:	add	ip, pc, ip
	ldr	pc, [ip, #0]
L_CFStringCreateWithFormat$slp:
	.long	L_CFStringCreateWithFormat$lazy_ptr - (L13$scv + 8)
	.lazy_symbol_pointer
L_CFStringCreateWithFormat$lazy_ptr:
	.indirect_symbol	_CFStringCreateWithFormat
	.long	dyld_stub_binding_helper
	.section __TEXT,__picsymbolstub4,symbol_stubs,none,16
	.align 2
	.code 32
L_init_jni$stub:
	.indirect_symbol _init_jni
	ldr	ip, L_init_jni$slp
L14$scv:	add	ip, pc, ip
	ldr	pc, [ip, #0]
L_init_jni$slp:
	.long	L_init_jni$lazy_ptr - (L14$scv + 8)
	.lazy_symbol_pointer
L_init_jni$lazy_ptr:
	.indirect_symbol	_init_jni
	.long	dyld_stub_binding_helper
	.non_lazy_symbol_pointer
L_env$non_lazy_ptr:
	.indirect_symbol _env
	.long	0
	.section __TEXT,__picsymbolstub4,symbol_stubs,none,16
	.align 2
	.code 32
L_CFRunLoopTimerInvalidate$stub:
	.indirect_symbol _CFRunLoopTimerInvalidate
	ldr	ip, L_CFRunLoopTimerInvalidate$slp
L15$scv:	add	ip, pc, ip
	ldr	pc, [ip, #0]
L_CFRunLoopTimerInvalidate$slp:
	.long	L_CFRunLoopTimerInvalidate$lazy_ptr - (L15$scv + 8)
	.lazy_symbol_pointer
L_CFRunLoopTimerInvalidate$lazy_ptr:
	.indirect_symbol	_CFRunLoopTimerInvalidate
	.long	dyld_stub_binding_helper
	.non_lazy_symbol_pointer
L_nppt$non_lazy_ptr:
	.indirect_symbol _nppt
	.long	0
	.section __TEXT,__picsymbolstub4,symbol_stubs,none,16
	.align 2
	.code 32
L_CFRunLoopGetMain$stub:
	.indirect_symbol _CFRunLoopGetMain
	ldr	ip, L_CFRunLoopGetMain$slp
L16$scv:	add	ip, pc, ip
	ldr	pc, [ip, #0]
L_CFRunLoopGetMain$slp:
	.long	L_CFRunLoopGetMain$lazy_ptr - (L16$scv + 8)
	.lazy_symbol_pointer
L_CFRunLoopGetMain$lazy_ptr:
	.indirect_symbol	_CFRunLoopGetMain
	.long	dyld_stub_binding_helper
	.section __TEXT,__picsymbolstub4,symbol_stubs,none,16
	.align 2
	.code 32
L_CFRelease$stub:
	.indirect_symbol _CFRelease
	ldr	ip, L_CFRelease$slp
L17$scv:	add	ip, pc, ip
	ldr	pc, [ip, #0]
L_CFRelease$slp:
	.long	L_CFRelease$lazy_ptr - (L17$scv + 8)
	.lazy_symbol_pointer
L_CFRelease$lazy_ptr:
	.indirect_symbol	_CFRelease
	.long	dyld_stub_binding_helper
	.section __TEXT,__picsymbolstub4,symbol_stubs,none,16
	.align 2
	.code 32
L_open$stub:
	.indirect_symbol _open
	ldr	ip, L_open$slp
L18$scv:	add	ip, pc, ip
	ldr	pc, [ip, #0]
L_open$slp:
	.long	L_open$lazy_ptr - (L18$scv + 8)
	.lazy_symbol_pointer
L_open$lazy_ptr:
	.indirect_symbol	_open
	.long	dyld_stub_binding_helper
	.section __TEXT,__picsymbolstub4,symbol_stubs,none,16
	.align 2
	.code 32
L_CFRunLoopRun$stub:
	.indirect_symbol _CFRunLoopRun
	ldr	ip, L_CFRunLoopRun$slp
L19$scv:	add	ip, pc, ip
	ldr	pc, [ip, #0]
L_CFRunLoopRun$slp:
	.long	L_CFRunLoopRun$lazy_ptr - (L19$scv + 8)
	.lazy_symbol_pointer
L_CFRunLoopRun$lazy_ptr:
	.indirect_symbol	_CFRunLoopRun
	.long	dyld_stub_binding_helper
	.section __TEXT,__picsymbolstub4,symbol_stubs,none,16
	.align 2
	.code 32
L_CFDataGetTypeID$stub:
	.indirect_symbol _CFDataGetTypeID
	ldr	ip, L_CFDataGetTypeID$slp
L20$scv:	add	ip, pc, ip
	ldr	pc, [ip, #0]
L_CFDataGetTypeID$slp:
	.long	L_CFDataGetTypeID$lazy_ptr - (L20$scv + 8)
	.lazy_symbol_pointer
L_CFDataGetTypeID$lazy_ptr:
	.indirect_symbol	_CFDataGetTypeID
	.long	dyld_stub_binding_helper
	.section __TEXT,__picsymbolstub4,symbol_stubs,none,16
	.align 2
	.code 32
L_do_later$stub:
	.indirect_symbol _do_later
	ldr	ip, L_do_later$slp
L21$scv:	add	ip, pc, ip
	ldr	pc, [ip, #0]
L_do_later$slp:
	.long	L_do_later$lazy_ptr - (L21$scv + 8)
	.lazy_symbol_pointer
L_do_later$lazy_ptr:
	.indirect_symbol	_do_later
	.long	dyld_stub_binding_helper
	.section __TEXT,__picsymbolstub4,symbol_stubs,none,16
	.align 2
	.code 32
L_calloc$stub:
	.indirect_symbol _calloc
	ldr	ip, L_calloc$slp
L22$scv:	add	ip, pc, ip
	ldr	pc, [ip, #0]
L_calloc$slp:
	.long	L_calloc$lazy_ptr - (L22$scv + 8)
	.lazy_symbol_pointer
L_calloc$lazy_ptr:
	.indirect_symbol	_calloc
	.long	dyld_stub_binding_helper
	.section __TEXT,__picsymbolstub4,symbol_stubs,none,16
	.align 2
	.code 32
L_main_thread_kthx$stub:
	.indirect_symbol _main_thread_kthx
	ldr	ip, L_main_thread_kthx$slp
L23$scv:	add	ip, pc, ip
	ldr	pc, [ip, #0]
L_main_thread_kthx$slp:
	.long	L_main_thread_kthx$lazy_ptr - (L23$scv + 8)
	.lazy_symbol_pointer
L_main_thread_kthx$lazy_ptr:
	.indirect_symbol	_main_thread_kthx
	.long	dyld_stub_binding_helper
	.non_lazy_symbol_pointer
L_kCFAllocatorDefault$non_lazy_ptr:
	.indirect_symbol _kCFAllocatorDefault
	.long	0
L_kCFRunLoopCommonModes$non_lazy_ptr:
	.indirect_symbol _kCFRunLoopCommonModes
	.long	0
	.section __TEXT,__picsymbolstub4,symbol_stubs,none,16
	.align 2
	.code 32
L_new_connection$stub:
	.indirect_symbol _new_connection
	ldr	ip, L_new_connection$slp
L24$scv:	add	ip, pc, ip
	ldr	pc, [ip, #0]
L_new_connection$slp:
	.long	L_new_connection$lazy_ptr - (L24$scv + 8)
	.lazy_symbol_pointer
L_new_connection$lazy_ptr:
	.indirect_symbol	_new_connection
	.long	dyld_stub_binding_helper
	.section __TEXT,__picsymbolstub4,symbol_stubs,none,16
	.align 2
	.code 32
L_CFSetGetValue$stub:
	.indirect_symbol _CFSetGetValue
	ldr	ip, L_CFSetGetValue$slp
L25$scv:	add	ip, pc, ip
	ldr	pc, [ip, #0]
L_CFSetGetValue$slp:
	.long	L_CFSetGetValue$lazy_ptr - (L25$scv + 8)
	.lazy_symbol_pointer
L_CFSetGetValue$lazy_ptr:
	.indirect_symbol	_CFSetGetValue
	.long	dyld_stub_binding_helper
	.section __TEXT,__picsymbolstub4,symbol_stubs,none,16
	.align 2
	.code 32
L___clear_cache$stub:
	.indirect_symbol ___clear_cache
	ldr	ip, L___clear_cache$slp
L26$scv:	add	ip, pc, ip
	ldr	pc, [ip, #0]
L___clear_cache$slp:
	.long	L___clear_cache$lazy_ptr - (L26$scv + 8)
	.lazy_symbol_pointer
L___clear_cache$lazy_ptr:
	.indirect_symbol	___clear_cache
	.long	dyld_stub_binding_helper
	.section __TEXT,__picsymbolstub4,symbol_stubs,none,16
	.align 2
	.code 32
L_CFGetTypeID$stub:
	.indirect_symbol _CFGetTypeID
	ldr	ip, L_CFGetTypeID$slp
L27$scv:	add	ip, pc, ip
	ldr	pc, [ip, #0]
L_CFGetTypeID$slp:
	.long	L_CFGetTypeID$lazy_ptr - (L27$scv + 8)
	.lazy_symbol_pointer
L_CFGetTypeID$lazy_ptr:
	.indirect_symbol	_CFGetTypeID
	.long	dyld_stub_binding_helper
	.section __TEXT,__picsymbolstub4,symbol_stubs,none,16
	.align 2
	.code 32
L_CFAbsoluteTimeGetCurrent$stub:
	.indirect_symbol _CFAbsoluteTimeGetCurrent
	ldr	ip, L_CFAbsoluteTimeGetCurrent$slp
L28$scv:	add	ip, pc, ip
	ldr	pc, [ip, #0]
L_CFAbsoluteTimeGetCurrent$slp:
	.long	L_CFAbsoluteTimeGetCurrent$lazy_ptr - (L28$scv + 8)
	.lazy_symbol_pointer
L_CFAbsoluteTimeGetCurrent$lazy_ptr:
	.indirect_symbol	_CFAbsoluteTimeGetCurrent
	.long	dyld_stub_binding_helper
	.section __TEXT,__picsymbolstub4,symbol_stubs,none,16
	.align 2
	.code 32
L_get_src$stub:
	.indirect_symbol _get_src
	ldr	ip, L_get_src$slp
L29$scv:	add	ip, pc, ip
	ldr	pc, [ip, #0]
L_get_src$slp:
	.long	L_get_src$lazy_ptr - (L29$scv + 8)
	.lazy_symbol_pointer
L_get_src$lazy_ptr:
	.indirect_symbol	_get_src
	.long	dyld_stub_binding_helper
	.section __TEXT,__picsymbolstub4,symbol_stubs,none,16
	.align 2
	.code 32
L_strlen$stub:
	.indirect_symbol _strlen
	ldr	ip, L_strlen$slp
L30$scv:	add	ip, pc, ip
	ldr	pc, [ip, #0]
L_strlen$slp:
	.long	L_strlen$lazy_ptr - (L30$scv + 8)
	.lazy_symbol_pointer
L_strlen$lazy_ptr:
	.indirect_symbol	_strlen
	.long	dyld_stub_binding_helper
	.section __TEXT,__picsymbolstub4,symbol_stubs,none,16
	.align 2
	.code 32
L_CFRunLoopRunInMode$stub:
	.indirect_symbol _CFRunLoopRunInMode
	ldr	ip, L_CFRunLoopRunInMode$slp
L31$scv:	add	ip, pc, ip
	ldr	pc, [ip, #0]
L_CFRunLoopRunInMode$slp:
	.long	L_CFRunLoopRunInMode$lazy_ptr - (L31$scv + 8)
	.lazy_symbol_pointer
L_CFRunLoopRunInMode$lazy_ptr:
	.indirect_symbol	_CFRunLoopRunInMode
	.long	dyld_stub_binding_helper
	.section __TEXT,__picsymbolstub4,symbol_stubs,none,16
	.align 2
	.code 32
L_strcmp$stub:
	.indirect_symbol _strcmp
	ldr	ip, L_strcmp$slp
L32$scv:	add	ip, pc, ip
	ldr	pc, [ip, #0]
L_strcmp$slp:
	.long	L_strcmp$lazy_ptr - (L32$scv + 8)
	.lazy_symbol_pointer
L_strcmp$lazy_ptr:
	.indirect_symbol	_strcmp
	.long	dyld_stub_binding_helper
	.section __TEXT,__picsymbolstub4,symbol_stubs,none,16
	.align 2
	.code 32
L_CFSetCreateMutable$stub:
	.indirect_symbol _CFSetCreateMutable
	ldr	ip, L_CFSetCreateMutable$slp
L33$scv:	add	ip, pc, ip
	ldr	pc, [ip, #0]
L_CFSetCreateMutable$slp:
	.long	L_CFSetCreateMutable$lazy_ptr - (L33$scv + 8)
	.lazy_symbol_pointer
L_CFSetCreateMutable$lazy_ptr:
	.indirect_symbol	_CFSetCreateMutable
	.long	dyld_stub_binding_helper
	.non_lazy_symbol_pointer
L_kCFTypeSetCallBacks$non_lazy_ptr:
	.indirect_symbol _kCFTypeSetCallBacks
	.long	0
	.section __TEXT,__picsymbolstub4,symbol_stubs,none,16
	.align 2
	.code 32
L_lseek$stub:
	.indirect_symbol _lseek
	ldr	ip, L_lseek$slp
L34$scv:	add	ip, pc, ip
	ldr	pc, [ip, #0]
L_lseek$slp:
	.long	L_lseek$lazy_ptr - (L34$scv + 8)
	.lazy_symbol_pointer
L_lseek$lazy_ptr:
	.indirect_symbol	_lseek
	.long	dyld_stub_binding_helper
	.section __TEXT,__picsymbolstub4,symbol_stubs,none,16
	.align 2
	.code 32
L_abort$stub:
	.indirect_symbol _abort
	ldr	ip, L_abort$slp
L35$scv:	add	ip, pc, ip
	ldr	pc, [ip, #0]
L_abort$slp:
	.long	L_abort$lazy_ptr - (L35$scv + 8)
	.lazy_symbol_pointer
L_abort$lazy_ptr:
	.indirect_symbol	_abort
	.long	dyld_stub_binding_helper
	.section __TEXT,__picsymbolstub4,symbol_stubs,none,16
	.align 2
	.code 32
L_iface_getvalue$stub:
	.indirect_symbol _iface_getvalue
	ldr	ip, L_iface_getvalue$slp
L36$scv:	add	ip, pc, ip
	ldr	pc, [ip, #0]
L_iface_getvalue$slp:
	.long	L_iface_getvalue$lazy_ptr - (L36$scv + 8)
	.lazy_symbol_pointer
L_iface_getvalue$lazy_ptr:
	.indirect_symbol	_iface_getvalue
	.long	dyld_stub_binding_helper
	.non_lazy_symbol_pointer
L_JNI_OnLoad_ptr$non_lazy_ptr:
	.indirect_symbol _JNI_OnLoad_ptr
	.long	0
	.section __TEXT,__picsymbolstub4,symbol_stubs,none,16
	.align 2
	.code 32
L_mmap$stub:
	.indirect_symbol _mmap
	ldr	ip, L_mmap$slp
L37$scv:	add	ip, pc, ip
	ldr	pc, [ip, #0]
L_mmap$slp:
	.long	L_mmap$lazy_ptr - (L37$scv + 8)
	.lazy_symbol_pointer
L_mmap$lazy_ptr:
	.indirect_symbol	_mmap
	.long	dyld_stub_binding_helper
	.non_lazy_symbol_pointer
L_kCFRunLoopDefaultMode$non_lazy_ptr:
	.indirect_symbol _kCFRunLoopDefaultMode
	.long	0
	.section __TEXT,__picsymbolstub4,symbol_stubs,none,16
	.align 2
	.code 32
L_memcpy$stub:
	.indirect_symbol _memcpy
	ldr	ip, L_memcpy$slp
L38$scv:	add	ip, pc, ip
	ldr	pc, [ip, #0]
L_memcpy$slp:
	.long	L_memcpy$lazy_ptr - (L38$scv + 8)
	.lazy_symbol_pointer
L_memcpy$lazy_ptr:
	.indirect_symbol	_memcpy
	.long	dyld_stub_binding_helper
	.section __TEXT,__picsymbolstub4,symbol_stubs,none,16
	.align 2
	.code 32
L___assert_rtn$stub:
	.indirect_symbol ___assert_rtn
	ldr	ip, L___assert_rtn$slp
L39$scv:	add	ip, pc, ip
	ldr	pc, [ip, #0]
L___assert_rtn$slp:
	.long	L___assert_rtn$lazy_ptr - (L39$scv + 8)
	.lazy_symbol_pointer
L___assert_rtn$lazy_ptr:
	.indirect_symbol	___assert_rtn
	.long	dyld_stub_binding_helper
	.section __TEXT,__picsymbolstub4,symbol_stubs,none,16
	.align 2
	.code 32
L_CFDataGetBytes$stub:
	.indirect_symbol _CFDataGetBytes
	ldr	ip, L_CFDataGetBytes$slp
L40$scv:	add	ip, pc, ip
	ldr	pc, [ip, #0]
L_CFDataGetBytes$slp:
	.long	L_CFDataGetBytes$lazy_ptr - (L40$scv + 8)
	.lazy_symbol_pointer
L_CFDataGetBytes$lazy_ptr:
	.indirect_symbol	_CFDataGetBytes
	.long	dyld_stub_binding_helper
	.section __TEXT,__picsymbolstub4,symbol_stubs,none,16
	.align 2
	.code 32
L_CFDataGetBytePtr$stub:
	.indirect_symbol _CFDataGetBytePtr
	ldr	ip, L_CFDataGetBytePtr$slp
L41$scv:	add	ip, pc, ip
	ldr	pc, [ip, #0]
L_CFDataGetBytePtr$slp:
	.long	L_CFDataGetBytePtr$lazy_ptr - (L41$scv + 8)
	.lazy_symbol_pointer
L_CFDataGetBytePtr$lazy_ptr:
	.indirect_symbol	_CFDataGetBytePtr
	.long	dyld_stub_binding_helper
	.section __TEXT,__picsymbolstub4,symbol_stubs,none,16
	.align 2
	.code 32
L_CFRunLoopAddTimer$stub:
	.indirect_symbol _CFRunLoopAddTimer
	ldr	ip, L_CFRunLoopAddTimer$slp
L42$scv:	add	ip, pc, ip
	ldr	pc, [ip, #0]
L_CFRunLoopAddTimer$slp:
	.long	L_CFRunLoopAddTimer$lazy_ptr - (L42$scv + 8)
	.lazy_symbol_pointer
L_CFRunLoopAddTimer$lazy_ptr:
	.indirect_symbol	_CFRunLoopAddTimer
	.long	dyld_stub_binding_helper
	.section __TEXT,__picsymbolstub4,symbol_stubs,none,16
	.align 2
	.code 32
L_CFSetAddValue$stub:
	.indirect_symbol _CFSetAddValue
	ldr	ip, L_CFSetAddValue$slp
L43$scv:	add	ip, pc, ip
	ldr	pc, [ip, #0]
L_CFSetAddValue$slp:
	.long	L_CFSetAddValue$lazy_ptr - (L43$scv + 8)
	.lazy_symbol_pointer
L_CFSetAddValue$lazy_ptr:
	.indirect_symbol	_CFSetAddValue
	.long	dyld_stub_binding_helper
	.subsections_via_symbols
