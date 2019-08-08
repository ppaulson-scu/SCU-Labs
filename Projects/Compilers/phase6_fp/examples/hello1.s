main:		#Function
	pushl	%ebp
	movl	%esp, %ebp
	subl	$main.size, %esp
	.data
.L0:	.asciz	"hello world\n"
	.text
	leal	.L0, %eax
	pushl	%eax		#Call: Push arguments[0]  char *
	movl	%eax, -4(%ebp)
	call	printf		#Call: function call
	addl	$4, %esp		#Call: stack pointer readjustment
main.exit:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.set	main.size, 0
	.globl	main

