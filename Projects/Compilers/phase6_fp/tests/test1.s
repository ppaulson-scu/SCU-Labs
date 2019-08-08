main:		#Function
	pushl	%ebp
	movl	%esp, %ebp
	subl	$4, %esp
	movl	$0, -4(%ebp)		#Assignment
	movl	%ebp, %esp
	popl	%ebp
	ret

	.globl	main

