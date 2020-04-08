.syntax unified
/* PMC see pg 520 documentation */
.equ PMC,	0x400E0400 /* PMC Base Address */
.equ PCER0, 0x10	/* Peripheral Clock Enable Register 0*/

/* PIOC see pg 575 documentation */
.equ PIOC,			0x400E1200 /* PIOC Base Address */
.equ PIOA,			0x400E0E00 /* PIOA Base Address */
.equ OER_OFFSET,	0x10	/* Output Enable Register */
.equ ODR_OFFSET,	0x14	/* Output Disable Register */
.equ SODR_OFFSET,	0x30	/* Set Output Data Register */
.equ CODR_OFFSET,	0x34	/* Clear Output Data Register */
.equ OWER_OFFSET,	0xA0	/* Output Write Enable Register*/

/* PIOC PID pg. 48*/
.equ PID13, 13	
/* PIOA PID */
.equ PID11, 11

.equ INPUT_DIR,		1 
.equ OUTPUT_DIR,	0
.equ LEVEL_HIGH,	1 
.equ LEVEL_LOW,		0

.section .text

.thumb_func
.global turnOnPC20
turnOnPC20:	
	/* 
		Assuming the loader did its job, 
		at this point memory has been 
		initialized and the machine is 
		ready for execution 
	*/

	/* init pioc */
	ldr r0, =(PMC + PCER0)
	mov r1, #1
	lsl r1, r1, PID13
	str r1, [r0] /* PCER0.bit13 = 1 */
	ldr r0, =(PMC + PCER0)
	mov r1, #1
	lsl r1, r1, PID11
	str r1, [r0] /* PCER0.bit11 = 1 */
	
	/* configure PC20 as output */
	mov r0, OUTPUT_DIR
	mov r1, 20
	pioc_dir_set:	
	cmp r0, OUTPUT_DIR
	bne else_d
	ldr r0, =(PIOC + OER_OFFSET) 
	b end_d
	else_d:
	ldr r0, =(PIOC + ODR_OFFSET) 
	end_d:
	mov r2, #1
	lsl r2, r2, r1			
	str r2, [r0]	/* OxR.bitx = 1 */
	enable_writing:
	ldr r0, =(PIOC + OWER_OFFSET) 
	mov r2, #1
	lsl r2, r2, r1
	str r2, [r0]	/* OWER.bitx = 1 */

	/* set PC20 high (logic is inverted) */
	mov r0, LEVEL_LOW
	mov r1, 20
	pioc_level_set:
	cmp r0, LEVEL_HIGH
	bne else_l
	ldr r0, =(PIOC + SODR_OFFSET) 
	b end_l
	else_l:
	ldr r0, =(PIOC + CODR_OFFSET) 
	end_l:
	mov r2, #1
	lsl r2, r2, r1  
	str r2, [r0]	/* xODR.bitx = 1 */

	bx lr

.thumb_func
.global turnOffPC20
turnOffPC20:
	/* init pioc */
	ldr r0, =(PMC + PCER0)
	mov r1, #1
	lsl r1, r1, PID13
	str r1, [r0] /* PCER0.bit13 = 1 */
	ldr r0, =(PMC + PCER0)
	mov r1, #1
	lsl r1, r1, PID11
	str r1, [r0] /* PCER0.bit11 = 1 */

	/* configure PC20 as output */
	mov r0, OUTPUT_DIR
	mov r1, 20
	pioc_dir_seto:	
	cmp r0, OUTPUT_DIR
	bne else_do
	ldr r0, =(PIOC + OER_OFFSET) 
	b end_do
	else_do:
	ldr r0, =(PIOC + ODR_OFFSET) 
	end_do:
	mov r2, #1
	lsl r2, r2, r1			
	str r2, [r0]	/* OxR.bitx = 1 */
	enable_writingo:
	ldr r0, =(PIOC + OWER_OFFSET) 
	mov r2, #1
	lsl r2, r2, r1
	str r2, [r0]	/* OWER.bitx = 1 */

	/* set PC20 low (logic is inverted) */
	mov r0, LEVEL_HIGH
	mov r1, 20
	pioc_level_seto:
	cmp r0, LEVEL_HIGH
	bne else_lo
	ldr r0, =(PIOC + SODR_OFFSET) 
	b end_lo
	else_lo:
	ldr r0, =(PIOC + CODR_OFFSET) 
	end_lo:
	mov r2, #1
	lsl r2, r2, r1  
	str r2, [r0]	/* xODR.bitx = 1 */

	bx lr

.thumb_func
.global turnOnPC16
	turnOnPC16:
	/* init pioc */
	ldr r0, =(PMC + PCER0)
	mov r1, #1
	lsl r1, r1, PID13
	str r1, [r0] /* PCER0.bit13 = 1 */
	ldr r0, =(PMC + PCER0)
	mov r1, #1
	lsl r1, r1, PID11
	str r1, [r0] /* PCER0.bit11 = 1 */

	/* configure PC16 as output */
	mov r0, OUTPUT_DIR
	mov r1, 16
	pioa_dir_set:	
	cmp r0, OUTPUT_DIR
	bne else_d_a
	ldr r0, =(PIOA + OER_OFFSET) 
	b end_d_a
	else_d_a:
	ldr r0, =(PIOA + ODR_OFFSET) 
	end_d_a:
	mov r2, #1
	lsl r2, r2, r1			
	str r2, [r0]	/* OxR.bitx = 1 */
	enable_writing_a:
	ldr r0, =(PIOA + OWER_OFFSET) 
	mov r2, #1
	lsl r2, r2, r1
	str r2, [r0]	/* OWER.bitx = 1 */

	/* set PC16 low (logic is inverted) */
	mov r0, LEVEL_LOW
	mov r1, 16
	pioa_level_set:
	cmp r0, LEVEL_HIGH
	bne else_l_a
	ldr r0, =(PIOA + SODR_OFFSET) 
	b end_l_a
	else_l_a:
	ldr r0, =(PIOA + CODR_OFFSET) 
	end_l_a:
	mov r2, #1
	lsl r2, r2, r1  
	str r2, [r0]	/* xODR.bitx = 1 */
	bx lr

.thumb_func
.global turnOffPC16
turnOffPC16:
	/* init pioc */
	ldr r0, =(PMC + PCER0)
	mov r1, #1
	lsl r1, r1, PID13
	str r1, [r0] /* PCER0.bit13 = 1 */
	ldr r0, =(PMC + PCER0)
	mov r1, #1
	lsl r1, r1, PID11
	str r1, [r0] /* PCER0.bit11 = 1 */

	/* configure PC16 as output */
	mov r0, OUTPUT_DIR
	mov r1, 16
	pioa_dir_seto:	
	cmp r0, OUTPUT_DIR
	bne else_d_ao
	ldr r0, =(PIOA + OER_OFFSET) 
	b end_d_ao
	else_d_ao:
	ldr r0, =(PIOA + ODR_OFFSET) 
	end_d_ao:
	mov r2, #1
	lsl r2, r2, r1			
	str r2, [r0]	/* OxR.bitx = 1 */
	enable_writing_ao:
	ldr r0, =(PIOA + OWER_OFFSET) 
	mov r2, #1
	lsl r2, r2, r1
	str r2, [r0]	/* OWER.bitx = 1 */

	/* set PC16 low (logic is inverted) */
	mov r0, LEVEL_HIGH
	mov r1, 16
	pioa_level_seto:
	cmp r0, LEVEL_HIGH
	bne else_l_ao
	ldr r0, =(PIOA + SODR_OFFSET) 
	b end_l_ao
	else_l_ao:
	ldr r0, =(PIOA + CODR_OFFSET) 
	end_l_ao:
	mov r2, #1
	lsl r2, r2, r1  
	str r2, [r0]	/* xODR.bitx = 1 */
	bx lr


	