main:		#Function
	pushl	%ebp
	movl	%esp, %ebp
	subl	$main.size, %esp
	movl	$100, %eax
	movl		%eax, -4(%ebp)		#Assignment
	movl	$30, %ecx
	movl		%ecx, -8(%ebp)		#Assignment
	movl	$2, %edx
	movl		%edx, -12(%ebp)		#Assignment
#%eax
#%eax
					#-4(%ebp)
					#-8(%ebp)
				#SPILL
	movl	%eax, -36(%ebp)
	movl	-4(%ebp), %eax
	addl		-8(%ebp), %eax		#ADD
#%eax
#%eax
					#%eax
					#-12(%ebp)
	addl		-12(%ebp), %eax		#ADD
	movl		%eax, -16(%ebp)		#Assignment
				#SPILL
	movl	%eax, -40(%ebp)
	movl	-4(%ebp), %eax
	subl	-8(%ebp), %eax		#SUB
	subl	-12(%ebp), %eax		#SUB
	movl		%eax, -20(%ebp)		#Assignment
				#SPILL
	movl	%eax, -44(%ebp)
	movl	-4(%ebp), %eax
	imull	-8(%ebp), %eax		#MUL
	imull	-12(%ebp), %eax		#MUL
	movl		%eax, -24(%ebp)		#Assignment
	movl	%eax, -48(%ebp)
	movl	-4(%ebp), %eax
	movl	%edx, -52(%ebp)
	movl	%eax, %edx
	movl	%ecx, -56(%ebp)
	movl	-8(%ebp), %ecx
	sarl	$31, %edx		#DIV
	idivl	%ecx
#%eax
#%eax
					#%eax
					#-12(%ebp)
	addl		-12(%ebp), %eax		#ADD
	movl		%eax, -28(%ebp)		#Assignment
	movl	%eax, -60(%ebp)
	movl	-4(%ebp), %eax
	movl	%eax, %edx
	movl	-8(%ebp), %ecx
	sarl	$31, %edx		#REM
	idivl	%ecx
	subl	-12(%ebp), %edx		#SUB
	movl		%edx, -32(%ebp)		#Assignment
	pushl	-16(%ebp)		#Call: Push arguments[1]  int
	.data
.L0:	.asciz	"%d\n"
	.text
	leal	.L0, %eax
	pushl	%eax		#Call: Push arguments[0]  char *
	movl	%eax, -64(%ebp)
	movl	%edx, -68(%ebp)
	call	printf		#Call: function call
	addl	$8, %esp		#Call: stack pointer readjustment
	pushl	-20(%ebp)		#Call: Push arguments[1]  int
	.data
.L1:	.asciz	"%d\n"
	.text
	leal	.L1, %ecx
	pushl	%ecx		#Call: Push arguments[0]  char *
	movl	%eax, -72(%ebp)
	movl	%ecx, -76(%ebp)
	call	printf		#Call: function call
	addl	$8, %esp		#Call: stack pointer readjustment
	pushl	-24(%ebp)		#Call: Push arguments[1]  int
	.data
.L2:	.asciz	"%d\n"
	.text
	leal	.L2, %ecx
	pushl	%ecx		#Call: Push arguments[0]  char *
	movl	%eax, -80(%ebp)
	movl	%ecx, -84(%ebp)
	call	printf		#Call: function call
	addl	$8, %esp		#Call: stack pointer readjustment
	pushl	-28(%ebp)		#Call: Push arguments[1]  int
	.data
.L3:	.asciz	"%d\n"
	.text
	leal	.L3, %ecx
	pushl	%ecx		#Call: Push arguments[0]  char *
	movl	%eax, -88(%ebp)
	movl	%ecx, -92(%ebp)
	call	printf		#Call: function call
	addl	$8, %esp		#Call: stack pointer readjustment
	pushl	-32(%ebp)		#Call: Push arguments[1]  int
	.data
.L4:	.asciz	"%d\n"
	.text
	leal	.L4, %ecx
	pushl	%ecx		#Call: Push arguments[0]  char *
	movl	%eax, -96(%ebp)
	movl	%ecx, -100(%ebp)
	call	printf		#Call: function call
	addl	$8, %esp		#Call: stack pointer readjustment
main.exit:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.set	main.size, 100
	.globl	main

