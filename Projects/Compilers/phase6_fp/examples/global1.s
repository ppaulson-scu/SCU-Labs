foo:		#Function
	pushl	%ebp
	movl	%esp, %ebp
	subl	$0, %esp
					#x
					#$1
	movl	x, %eax
	addl		$1, %eax		#ADD
	movl		%eax, x		#Assignment
					#x
					#$1
	movl	x, %ecx
	addl		$1, %ecx		#ADD
				#RETURN
	movl	%eax, -4(%ebp)
	movl	%ecx, %eax
	jmp 	foo.exit
foo.exit:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.globl	foo

main:		#Function
	pushl	%ebp
	movl	%esp, %ebp
	subl	$0, %esp
	movl	$65, %ecx
	movl		%ecx, x		#Assignment
	pushl	x		#Call: Push arguments[0]  int
	call	putchar		#Call: function call
	addl	$4, %esp		#Call: stack pointer readjustment
	call	foo		#Call: function call
	pushl	%eax		#Call: Push arguments[0]  int
	call	putchar		#Call: function call
	addl	$4, %esp		#Call: stack pointer readjustment
	pushl	x		#Call: Push arguments[0]  int
	call	putchar		#Call: function call
	addl	$4, %esp		#Call: stack pointer readjustment
	pushl	$10		#Call: Push arguments[0]  int
	call	putchar		#Call: function call
	addl	$4, %esp		#Call: stack pointer readjustment
main.exit:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.globl	main

	.comm	x, 4
