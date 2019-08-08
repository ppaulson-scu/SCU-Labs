fib:		#Function
	pushl	%ebp
	movl	%esp, %ebp
	subl	$fib.size, %esp
	movl	8(%ebp), %eax
				#cmpe
	cmpl		$0, %eax
	sete	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	jne	.L2
	movl	8(%ebp), %eax
				#cmpe
	cmpl		$1, %eax
	sete	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	jne	.L2
				#OR
	movl	$0, %eax
	jmp	.L3
.L2:
	movl	$1, %eax
.L3:
	cmpl	$0, %eax
	je	.L0
				#RETURN
	movl	$1, %eax
	jmp 	fib.exit
.L0:
	movl	8(%ebp), %eax
	subl	$1, %eax		#SUB
	pushl	%eax		#Call: Push arguments[0]  int
	movl	%eax, -4(%ebp)
	call	fib		#Call: function call
	addl	$4, %esp		#Call: stack pointer readjustment
	movl	8(%ebp), %ecx
	subl	$2, %ecx		#SUB
	pushl	%ecx		#Call: Push arguments[0]  int
	movl	%eax, -8(%ebp)
	movl	%ecx, -12(%ebp)
	call	fib		#Call: function call
	addl	$4, %esp		#Call: stack pointer readjustment
					#-8(%ebp)
					#%eax
	movl	-8(%ebp), %ecx
	addl		%eax, %ecx		#ADD
				#RETURN
	movl	%ecx, %eax
	jmp 	fib.exit
fib.exit:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.set	fib.size, 12
	.globl	fib

main:		#Function
	pushl	%ebp
	movl	%esp, %ebp
	subl	$main.size, %esp
	leal	-4(%ebp), %eax
	pushl	%eax		#Call: Push arguments[1]  int *
	.data
.L4:	.asciz	"%d"
	.text
	leal	.L4, %ecx
	pushl	%ecx		#Call: Push arguments[0]  char *
	movl	%eax, -8(%ebp)
	movl	%ecx, -12(%ebp)
	call	scanf		#Call: function call
	addl	$8, %esp		#Call: stack pointer readjustment
	pushl	-4(%ebp)		#Call: Push arguments[0]  int
	movl	%eax, -16(%ebp)
	call	fib		#Call: function call
	addl	$4, %esp		#Call: stack pointer readjustment
	pushl	%eax		#Call: Push arguments[1]  int
	.data
.L5:	.asciz	"%d\n"
	.text
	leal	.L5, %ecx
	pushl	%ecx		#Call: Push arguments[0]  char *
	movl	%eax, -20(%ebp)
	movl	%ecx, -24(%ebp)
	call	printf		#Call: function call
	addl	$8, %esp		#Call: stack pointer readjustment
main.exit:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.set	main.size, 24
	.globl	main

