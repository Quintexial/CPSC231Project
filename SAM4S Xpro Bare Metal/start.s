.syntax unified

.section .vectors 	
.skip 4	
.word start

.section .text
.thumb
.type start, %function
start:
	mov r0, #0x05			/* Question 1 */
	mov r1, #0x08
	mov r2, r0
	mov r0, r1
	mov r1, r2
	b .