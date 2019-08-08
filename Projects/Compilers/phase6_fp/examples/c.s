allocate:		#Function
	pushl	%ebp
	movl	%esp, %ebp
	subl	$allocate.size, %esp
	movl	$0, %eax
	movl		%eax, -4(%ebp)		#Assignment
	movl	8(%ebp), %ecx
	imull	$4, %ecx		#MUL
	pushl	%ecx		#Call: Push arguments[0]  int
	movl	%eax, -12(%ebp)
	movl	%ecx, -16(%ebp)
	call	malloc		#Call: function call
	addl	$4, %esp		#Call: stack pointer readjustment
				#CAST
	movl		%eax, -8(%ebp)		#Assignment
.L0:
	movl	-4(%ebp), %ecx
				#cmpl
	cmpl		8(%ebp), %ecx
	setl	%cl
	movzbl	%cl, %ecx
	cmpl	$0, %ecx
	je	.L1
	movl	-4(%ebp), %ecx
	imull	$4, %ecx		#MUL
	movl	-8(%ebp), %edx
	addl		%ecx, %edx		#ADD
	movl	8(%ebp), %ecx
	imull	$4, %ecx		#MUL
	pushl	%ecx		#Call: Push arguments[0]  int
	movl	%eax, -20(%ebp)
	movl	%ecx, -24(%ebp)
	movl	%edx, -28(%ebp)
	call	malloc		#Call: function call
	addl	$4, %esp		#Call: stack pointer readjustment
	movl	-28(%ebp), %ecx
	movl		%eax, (%ecx)		#Deref Assignment
	movl	-4(%ebp), %edx
	addl		$1, %edx		#ADD
	movl		%edx, -4(%ebp)		#Assignment
	jmp	.L0
.L1:
				#RETURN
	movl	-8(%ebp), %eax
	jmp 	allocate.exit
allocate.exit:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.set	allocate.size, 28
	.globl	allocate

initialize:		#Function
	pushl	%ebp
	movl	%esp, %ebp
	subl	$initialize.size, %esp
	movl	$0, %eax
	movl		%eax, -4(%ebp)		#Assignment
.L2:
	movl	-4(%ebp), %ecx
				#cmpl
	cmpl		12(%ebp), %ecx
	setl	%cl
	movzbl	%cl, %ecx
	cmpl	$0, %ecx
	je	.L3
	movl	$0, %ecx
	movl		%ecx, -8(%ebp)		#Assignment
.L4:
	movl	-8(%ebp), %edx
				#cmpl
	cmpl		12(%ebp), %edx
	setl	%dl
	movzbl	%dl, %edx
	cmpl	$0, %edx
	je	.L5
	movl	-4(%ebp), %edx
	imull	$4, %edx		#MUL
				#SPILL
	movl	%eax, -12(%ebp)
	movl	8(%ebp), %eax
	addl		%edx, %eax		#ADD
					#DEREF
					#NON-LVALUE DEREF
	movl		(%eax), %eax
	movl	-8(%ebp), %edx
	imull	$4, %edx		#MUL
	addl		%edx, %eax		#ADD
	movl	-4(%ebp), %edx
	addl		-8(%ebp), %edx		#ADD
	movl		%edx, (%eax)		#Deref Assignment
				#SPILL
	movl	%eax, -16(%ebp)
	movl	-8(%ebp), %eax
	addl		$1, %eax		#ADD
	movl		%eax, -8(%ebp)		#Assignment
	jmp	.L4
.L5:
	movl	-4(%ebp), %eax
	addl		$1, %eax		#ADD
	movl		%eax, -4(%ebp)		#Assignment
	jmp	.L2
.L3:
initialize.exit:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.set	initialize.size, 16
	.globl	initialize

display:		#Function
	pushl	%ebp
	movl	%esp, %ebp
	subl	$display.size, %esp
	movl	$0, %eax
	movl		%eax, -4(%ebp)		#Assignment
.L6:
	movl	-4(%ebp), %ecx
				#cmpl
	cmpl		12(%ebp), %ecx
	setl	%cl
	movzbl	%cl, %ecx
	cmpl	$0, %ecx
	je	.L7
	movl	$0, %ecx
	movl		%ecx, -8(%ebp)		#Assignment
.L8:
	movl	-8(%ebp), %edx
				#cmpl
	cmpl		12(%ebp), %edx
	setl	%dl
	movzbl	%dl, %edx
	cmpl	$0, %edx
	je	.L9
	movl	-4(%ebp), %edx
	imull	$4, %edx		#MUL
				#SPILL
	movl	%eax, -16(%ebp)
	movl	8(%ebp), %eax
	addl		%edx, %eax		#ADD
					#DEREF
					#NON-LVALUE DEREF
	movl		(%eax), %eax
	movl		%eax, -12(%ebp)		#Assignment
	movl	-8(%ebp), %edx
	imull	$4, %edx		#MUL
				#SPILL
	movl	%eax, -20(%ebp)
	movl	-12(%ebp), %eax
	addl		%edx, %eax		#ADD
					#DEREF
					#NON-LVALUE DEREF
	movl		(%eax), %eax
	pushl	%eax		#Call: Push arguments[1]  int
				#ADDR
	.data
.L10:	.asciz	"%d "
	.text
	leal	.L10, %edx
	pushl	%edx		#Call: Push arguments[0]  char *
	movl	%eax, -24(%ebp)
	movl	%ecx, -28(%ebp)
	movl	%edx, -32(%ebp)
	call	printf		#Call: function call
	addl	$8, %esp		#Call: stack pointer readjustment
	movl	-8(%ebp), %ecx
	addl		$1, %ecx		#ADD
	movl		%ecx, -8(%ebp)		#Assignment
	jmp	.L8
.L9:
	movl	-4(%ebp), %eax
	addl		$1, %eax		#ADD
	movl		%eax, -4(%ebp)		#Assignment
				#ADDR
	.data
.L11:	.asciz	"\n"
	.text
	leal	.L11, %ecx
	pushl	%ecx		#Call: Push arguments[0]  char *
	movl	%eax, -36(%ebp)
	movl	%ecx, -40(%ebp)
	call	printf		#Call: function call
	addl	$4, %esp		#Call: stack pointer readjustment
	jmp	.L6
.L7:
display.exit:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.set	display.size, 40
	.globl	display

deallocate:		#Function
	pushl	%ebp
	movl	%esp, %ebp
	subl	$deallocate.size, %esp
	movl	$0, %eax
	movl		%eax, -4(%ebp)		#Assignment
.L12:
	movl	-4(%ebp), %ecx
				#cmpl
	cmpl		12(%ebp), %ecx
	setl	%cl
	movzbl	%cl, %ecx
	cmpl	$0, %ecx
	je	.L13
	movl	-4(%ebp), %ecx
	imull	$4, %ecx		#MUL
	movl	8(%ebp), %edx
	addl		%ecx, %edx		#ADD
					#DEREF
					#NON-LVALUE DEREF
	movl		(%edx), %edx
	pushl	%edx		#Call: Push arguments[0]  int *
	movl	%eax, -8(%ebp)
	movl	%edx, -12(%ebp)
	call	free		#Call: function call
	addl	$4, %esp		#Call: stack pointer readjustment
	movl	-4(%ebp), %ecx
	addl		$1, %ecx		#ADD
	movl		%ecx, -4(%ebp)		#Assignment
	jmp	.L12
.L13:
	pushl	8(%ebp)		#Call: Push arguments[0]  int **
	call	free		#Call: function call
	addl	$4, %esp		#Call: stack pointer readjustment
deallocate.exit:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.set	deallocate.size, 12
	.globl	deallocate

main:		#Function
	pushl	%ebp
	movl	%esp, %ebp
	subl	$main.size, %esp
				#ADDR
	leal	-8(%ebp), %eax
	pushl	%eax		#Call: Push arguments[1]  int *
				#ADDR
	.data
.L14:	.asciz	"%d"
	.text
	leal	.L14, %ecx
	pushl	%ecx		#Call: Push arguments[0]  char *
	movl	%eax, -12(%ebp)
	movl	%ecx, -16(%ebp)
	call	scanf		#Call: function call
	addl	$8, %esp		#Call: stack pointer readjustment
	pushl	-8(%ebp)		#Call: Push arguments[0]  int
	movl	%eax, -20(%ebp)
	call	allocate		#Call: function call
	addl	$4, %esp		#Call: stack pointer readjustment
	movl		%eax, -4(%ebp)		#Assignment
	pushl	-8(%ebp)		#Call: Push arguments[1]  int
	pushl	-4(%ebp)		#Call: Push arguments[0]  int **
	movl	%eax, -24(%ebp)
	call	initialize		#Call: function call
	addl	$8, %esp		#Call: stack pointer readjustment
	pushl	-8(%ebp)		#Call: Push arguments[1]  int
	pushl	-4(%ebp)		#Call: Push arguments[0]  int **
	movl	%eax, -28(%ebp)
	call	display		#Call: function call
	addl	$8, %esp		#Call: stack pointer readjustment
	pushl	-8(%ebp)		#Call: Push arguments[1]  int
	pushl	-4(%ebp)		#Call: Push arguments[0]  int **
	movl	%eax, -32(%ebp)
	call	deallocate		#Call: function call
	addl	$8, %esp		#Call: stack pointer readjustment
main.exit:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.set	main.size, 32
	.globl	main

