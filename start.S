
@ InfoTM iMAPx800 start up code
@ warits.wang Thu Sep 1, 2011

#include <config.h>
#include CONFIG_IMAPX_REGS_FILE

.globl _start
_start: b	reset
	bl .	@ undefined instruction
	bl .	@ software interrupt
	bl .	@ prefetch abort
	bl .	@ data abort
	bl .	@ not used
	bl .	@ irq
	bl .	@ fiq
	.word	0
	.word	1
	.word	2
	.word	3
	.word	4
	.word	5
	.word	6
	.word	7
.global _end_vect
_end_vect:

	.balignl 16,0x900dc0de

.globl _armboot_start
_armboot_start:
	.word _start

@ these are defined in the board-specific linker script.
.globl _bss_start
_bss_start:
	.word __bss_start

.globl _bss_end
_bss_end:
	.word _end

__stack:
	.word __lds_stack_svc


@ the actual reset code
reset:
	mov r0, #0
	mov r1, #0
	mov r2, #0
	mov r3, #0
	mov r4, #0
	mov r5, #0
	mov r6, #0
	mov r7, #0
	mov r8, #0
	mov r9, #0
	mov r10, #0
	mov r11, #0
	mov r12, #0
	mov r13, #0
	mov r14, #0
	@ set the cpu to SVC32 mode
	mrs	r0,cpsr
	bic	r0,r0,#0x1f
	orr	r0,r0,#0xd3			@ (NOINT | SVC)
	msr	cpsr,r0

	@ the mask ROM code should have PLL and others stable
	bl  core_init_crit

	@ hold all cores except core0
	mrc  p15, 0, r0, c0, c0, 5
	ubfx r0, r0, #0, #2
	cmp  r0, #0
	bne  core_hold			@ core_hold accept r0 as the core index

	@ do ASM lowlevel init

	@ set up the stack
	ldr	sp, __stack

	@ initialize the bss section
	ldr	r0, _bss_start		@ find start of bss segment
	ldr	r1, _bss_end		@ stop here
	mov	r2, #0

clbss_l:
	str	r2, [r0]			@ clear loop...
	add	r0, r0, #4
	cmp	r0, r1
	bne	clbss_l
	bl start_neon
	ldr	pc, _main

_main:
	.word main

start_neon :
	mrc p15,0,r0,c1,c0,2
	orr r0,r0,#0x00f00000
	mcr p15,0,r0,c1,c0,2
	isb
	mov r0,#0x40000000
	vmsr FPEXC,r0

@ initialize critical core registers
core_init_crit:

	@ flush v4 I/D caches
	mov	r0, #0                  @ set up for MCR
	mcr	p15, 0, r0, c8, c7, 0   @ invalidate TLBs
	mcr	p15, 0, r0, c7, c5, 0   @ invalidate icache

	@ disable MMU stuff and caches
	mrc	p15, 0, r0, c1, c0, 0
	bic	r0, r0, #0x00002000	@ clear bits 13 (--V-)
	bic	r0, r0, #0x00000007	@ clear bits 2:0 (-CAM)
	orr	r0, r0, #0x00000002	@ set bit 1 (--A-) Align
	orr	r0, r0, #0x00000800	@ set bit 12 (Z---) BTB
	mcr	p15, 0, r0, c1, c0, 0

	@ jump to board specific initialization...
	@ The Mask ROM will have already initialized
	@ basic memory.  Go here to bump up clock rate and handle
	@ wake up conditions.

	@ disable watch dog in each core
	ldr r1, =TIMER_WATCHDOG_ADDR
	ldr r0, =0x12345678
	str r0, [r1, #0x34]
	ldr r0, =0x87654321
	str r0, [r1, #0x34]

	mov	pc, lr		@ back to my caller

core_hold:
	@ hold current core, r0 is reserved for core index
	ldr r3, =RTC_INFO0
	ldr r1, [r3]
	mov r2, #0x3b
	cmp r1, r2
	bne low_power

	@ load alternate entry
	ldr r1, [r3, #0xc]
	lsl r2, r1, #8
	ldr r1, [r3, #0x8]
	orr r2, r2, r1
	lsl r2, r2, #8
	ldr r1, [r3, #0x4]
	orr r2, r2, r1
	lsl r1, r2, #8

	lsl r2, r0, #2
	add r1, r2, r1
	mov r2, #1
	str r2, [r1]
	.word 0xf57ff04f	@ DSB

hold_l:
	ldr r0, [r1]
	cmp r0, #1
	beq hold_l
	
	@jump to the re-directed address
	mov pc, r0

low_power:
	.word 0xe320f003	@ put core into WFI mode
	b .			@ never reach

