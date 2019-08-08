insert:		#Function
	pushl	%ebp
	movl	%esp, %ebp
	subl	$insert.size, %esp
	movl	8(%ebp), %eax
	cmpl		$0, %eax		#NOT
	sete	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	je	.L0
	movl	$4, %eax
	imull	$3, %eax		#MUL
	pushl	%eax		#Call: Push arguments[0]  int
	movl	%eax, -4(%ebp)
	call	malloc		#Call: function call
	addl	$4, %esp		#Call: stack pointer readjustment
				#CAST
	movl		%eax, 8(%ebp)		#Assignment
	movl	$0, %ecx
	imull	$4, %ecx		#MUL
	movl	8(%ebp), %edx
	addl		%ecx, %edx		#ADD
	movl	%eax, -8(%ebp)
	movl	12(%ebp), %eax
	movl		%eax, (%edx)		#Deref Assignment
	movl	$1, %ecx
	imull	$4, %ecx		#MUL
				#SPILL
	movl	%eax, -12(%ebp)
	movl	8(%ebp), %eax
	addl		%ecx, %eax		#ADD
	movl	%eax, -16(%ebp)
	movl	null, %eax
	movl	-16(%ebp), %ecx
	movl		%eax, (%ecx)		#Deref Assignment
				#SPILL
	movl	%eax, -20(%ebp)
	movl	$2, %eax
	imull	$4, %eax		#MUL
				#SPILL
	movl	%eax, -24(%ebp)
	movl	8(%ebp), %eax
	addl		-24(%ebp), %eax		#ADD
	movl	%eax, -28(%ebp)
	movl	null, %eax
	movl	%ecx, -32(%ebp)
	movl	-28(%ebp), %ecx
	movl		%eax, (%ecx)		#Deref Assignment
	jmp	.L1
.L0:
	movl	$0, %eax
	imull	$4, %eax		#MUL
	movl	8(%ebp), %ecx
	addl		%eax, %ecx		#ADD
					#DEREF
					#NON-LVALUE DEREF
	movl		(%ecx), %ecx
	movl	12(%ebp), %eax
				#cmpl
	cmpl		%ecx, %eax
	setl	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	je	.L2
	movl	$1, %eax
	imull	$4, %eax		#MUL
	movl	8(%ebp), %ecx
	addl		%eax, %ecx		#ADD
	pushl	12(%ebp)		#Call: Push arguments[1]  int *
	movl	$1, %eax
	imull	$4, %eax		#MUL
	movl	8(%ebp), %edx
	addl		%eax, %edx		#ADD
					#DEREF
					#NON-LVALUE DEREF
	movl		(%edx), %edx
				#CAST
	pushl	%edx		#Call: Push arguments[0]  int **
	movl	%ecx, -36(%ebp)
	movl	%edx, -40(%ebp)
	call	insert		#Call: function call
	addl	$8, %esp		#Call: stack pointer readjustment
				#CAST
	movl	-36(%ebp), %ecx
	movl		%eax, (%ecx)		#Deref Assignment
	jmp	.L3
.L2:
	movl	$0, %edx
	imull	$4, %edx		#MUL
				#SPILL
	movl	%eax, -44(%ebp)
	movl	8(%ebp), %eax
	addl		%edx, %eax		#ADD
					#DEREF
					#NON-LVALUE DEREF
	movl		(%eax), %eax
	movl	12(%ebp), %edx
				#cmpg
	cmpl		%eax, %edx
	setg	%dl
	movzbl	%dl, %edx
	cmpl	$0, %edx
	je	.L4
	movl	$2, %eax
	imull	$4, %eax		#MUL
	movl	8(%ebp), %edx
	addl		%eax, %edx		#ADD
	pushl	12(%ebp)		#Call: Push arguments[1]  int *
	movl	$2, %eax
	imull	$4, %eax		#MUL
				#SPILL
	movl	%eax, -48(%ebp)
	movl	8(%ebp), %eax
	addl		-48(%ebp), %eax		#ADD
					#DEREF
					#NON-LVALUE DEREF
	movl		(%eax), %eax
				#CAST
	pushl	%eax		#Call: Push arguments[0]  int **
	movl	%eax, -52(%ebp)
	movl	%ecx, -56(%ebp)
	movl	%edx, -60(%ebp)
	call	insert		#Call: function call
	addl	$8, %esp		#Call: stack pointer readjustment
				#CAST
	movl	-60(%ebp), %ecx
	movl		%eax, (%ecx)		#Deref Assignment
.L4:
.L3:
.L1:
				#RETURN
	movl	8(%ebp), %eax
	jmp 	insert.exit
insert.exit:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.set	insert.size, 60
	.globl	insert

search:		#Function
	pushl	%ebp
	movl	%esp, %ebp
	subl	$search.size, %esp
	movl	8(%ebp), %eax
	cmpl		$0, %eax		#NOT
	sete	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	je	.L6
				#RETURN
	movl	$0, %eax
	jmp 	search.exit
.L6:
	movl	$0, %eax
	imull	$4, %eax		#MUL
	movl	8(%ebp), %ecx
	addl		%eax, %ecx		#ADD
					#DEREF
					#NON-LVALUE DEREF
	movl		(%ecx), %ecx
	movl	12(%ebp), %eax
				#cmpl
	cmpl		%ecx, %eax
	setl	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	je	.L8
	pushl	12(%ebp)		#Call: Push arguments[1]  int *
	movl	$1, %eax
	imull	$4, %eax		#MUL
	movl	8(%ebp), %ecx
	addl		%eax, %ecx		#ADD
					#DEREF
					#NON-LVALUE DEREF
	movl		(%ecx), %ecx
				#CAST
	pushl	%ecx		#Call: Push arguments[0]  int **
	movl	%ecx, -4(%ebp)
	call	search		#Call: function call
	addl	$8, %esp		#Call: stack pointer readjustment
				#RETURN
	jmp 	search.exit
.L8:
	movl	$0, %eax
	imull	$4, %eax		#MUL
	movl	8(%ebp), %ecx
	addl		%eax, %ecx		#ADD
					#DEREF
					#NON-LVALUE DEREF
	movl		(%ecx), %ecx
	movl	12(%ebp), %eax
				#cmpg
	cmpl		%ecx, %eax
	setg	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	je	.L10
	pushl	12(%ebp)		#Call: Push arguments[1]  int *
	movl	$2, %eax
	imull	$4, %eax		#MUL
	movl	8(%ebp), %ecx
	addl		%eax, %ecx		#ADD
					#DEREF
					#NON-LVALUE DEREF
	movl		(%ecx), %ecx
				#CAST
	pushl	%ecx		#Call: Push arguments[0]  int **
	movl	%ecx, -8(%ebp)
	call	search		#Call: function call
	addl	$8, %esp		#Call: stack pointer readjustment
				#RETURN
	jmp 	search.exit
.L10:
				#RETURN
	movl	$1, %eax
	jmp 	search.exit
search.exit:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.set	search.size, 8
	.globl	search

preorder:		#Function
	pushl	%ebp
	movl	%esp, %ebp
	subl	$preorder.size, %esp
	movl	8(%ebp), %eax
	cmpl	$0, %eax
	je	.L12
	movl	$0, %eax
	imull	$4, %eax		#MUL
	movl	8(%ebp), %ecx
	addl		%eax, %ecx		#ADD
					#DEREF
					#NON-LVALUE DEREF
	movl		(%ecx), %ecx
					#DEREF
					#LVALUE DEREF
	pushl	%ecx		#Call: Push arguments[1]  int
				#ADDR
	.data
.L14:	.asciz	"%d\n"
	.text
	leal	.L14, %eax
	pushl	%eax		#Call: Push arguments[0]  char *
	movl	%eax, -4(%ebp)
	movl	%ecx, -8(%ebp)
	call	printf		#Call: function call
	addl	$8, %esp		#Call: stack pointer readjustment
	movl	$1, %ecx
	imull	$4, %ecx		#MUL
	movl	8(%ebp), %edx
	addl		%ecx, %edx		#ADD
					#DEREF
					#NON-LVALUE DEREF
	movl		(%edx), %edx
				#CAST
	pushl	%edx		#Call: Push arguments[0]  int **
	movl	%eax, -12(%ebp)
	movl	%edx, -16(%ebp)
	call	preorder		#Call: function call
	addl	$4, %esp		#Call: stack pointer readjustment
	movl	$2, %ecx
	imull	$4, %ecx		#MUL
	movl	8(%ebp), %edx
	addl		%ecx, %edx		#ADD
					#DEREF
					#NON-LVALUE DEREF
	movl		(%edx), %edx
				#CAST
	pushl	%edx		#Call: Push arguments[0]  int **
	movl	%eax, -20(%ebp)
	movl	%edx, -24(%ebp)
	call	preorder		#Call: function call
	addl	$4, %esp		#Call: stack pointer readjustment
.L12:
preorder.exit:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.set	preorder.size, 24
	.globl	preorder

inorder:		#Function
	pushl	%ebp
	movl	%esp, %ebp
	subl	$inorder.size, %esp
	movl	8(%ebp), %eax
	cmpl	$0, %eax
	je	.L15
	movl	$1, %eax
	imull	$4, %eax		#MUL
	movl	8(%ebp), %ecx
	addl		%eax, %ecx		#ADD
					#DEREF
					#NON-LVALUE DEREF
	movl		(%ecx), %ecx
				#CAST
	pushl	%ecx		#Call: Push arguments[0]  int **
	movl	%ecx, -4(%ebp)
	call	inorder		#Call: function call
	addl	$4, %esp		#Call: stack pointer readjustment
	movl	$0, %ecx
	imull	$4, %ecx		#MUL
	movl	8(%ebp), %edx
	addl		%ecx, %edx		#ADD
					#DEREF
					#NON-LVALUE DEREF
	movl		(%edx), %edx
					#DEREF
					#LVALUE DEREF
	pushl	%edx		#Call: Push arguments[1]  int
				#ADDR
	.data
.L17:	.asciz	"%d\n"
	.text
	leal	.L17, %ecx
	pushl	%ecx		#Call: Push arguments[0]  char *
	movl	%eax, -8(%ebp)
	movl	%ecx, -12(%ebp)
	movl	%edx, -16(%ebp)
	call	printf		#Call: function call
	addl	$8, %esp		#Call: stack pointer readjustment
	movl	$2, %ecx
	imull	$4, %ecx		#MUL
	movl	8(%ebp), %edx
	addl		%ecx, %edx		#ADD
					#DEREF
					#NON-LVALUE DEREF
	movl		(%edx), %edx
				#CAST
	pushl	%edx		#Call: Push arguments[0]  int **
	movl	%eax, -20(%ebp)
	movl	%edx, -24(%ebp)
	call	inorder		#Call: function call
	addl	$4, %esp		#Call: stack pointer readjustment
.L15:
inorder.exit:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.set	inorder.size, 24
	.globl	inorder

main:		#Function
	pushl	%ebp
	movl	%esp, %ebp
	subl	$main.size, %esp
	movl	$0, %eax
	movl		%eax, -48(%ebp)		#Assignment
.L18:
	movl	-48(%ebp), %ecx
				#cmpl
	cmpl		$8, %ecx
	setl	%cl
	movzbl	%cl, %ecx
	cmpl	$0, %ecx
	je	.L19
				#ADDR
	leal	-44(%ebp), %ecx
	movl	-48(%ebp), %edx
	imull	$4, %edx		#MUL
	addl		%edx, %ecx		#ADD
	movl	%eax, -52(%ebp)
	movl	-48(%ebp), %eax
	movl		%eax, (%ecx)		#Deref Assignment
	movl	-48(%ebp), %edx
	addl		$1, %edx		#ADD
	movl		%edx, -48(%ebp)		#Assignment
	jmp	.L18
.L19:
	movl	null, %eax
				#CAST
	movl		%eax, -4(%ebp)		#Assignment
				#ADDR
				#ADDR
	leal	-44(%ebp), %ecx
	movl	$7, %edx
	imull	$4, %edx		#MUL
	addl		%edx, %ecx		#ADD
					#DEREF
					#NON-LVALUE DEREF
	movl		(%ecx), %ecx
	pushl	%ecx		#Call: Push arguments[1]  int *
	pushl	-4(%ebp)		#Call: Push arguments[0]  int **
	movl	%eax, -56(%ebp)
	movl	%ecx, -60(%ebp)
	call	insert		#Call: function call
	addl	$8, %esp		#Call: stack pointer readjustment
	movl		%eax, -4(%ebp)		#Assignment
				#ADDR
				#ADDR
	leal	-44(%ebp), %ecx
	movl	$4, %edx
	imull	$4, %edx		#MUL
	addl		%edx, %ecx		#ADD
					#DEREF
					#NON-LVALUE DEREF
	movl		(%ecx), %ecx
	pushl	%ecx		#Call: Push arguments[1]  int *
	pushl	-4(%ebp)		#Call: Push arguments[0]  int **
	movl	%eax, -64(%ebp)
	movl	%ecx, -68(%ebp)
	call	insert		#Call: function call
	addl	$8, %esp		#Call: stack pointer readjustment
	movl		%eax, -4(%ebp)		#Assignment
				#ADDR
				#ADDR
	leal	-44(%ebp), %ecx
	movl	$1, %edx
	imull	$4, %edx		#MUL
	addl		%edx, %ecx		#ADD
					#DEREF
					#NON-LVALUE DEREF
	movl		(%ecx), %ecx
	pushl	%ecx		#Call: Push arguments[1]  int *
	pushl	-4(%ebp)		#Call: Push arguments[0]  int **
	movl	%eax, -72(%ebp)
	movl	%ecx, -76(%ebp)
	call	insert		#Call: function call
	addl	$8, %esp		#Call: stack pointer readjustment
	movl		%eax, -4(%ebp)		#Assignment
				#ADDR
				#ADDR
	leal	-44(%ebp), %ecx
	movl	$0, %edx
	imull	$4, %edx		#MUL
	addl		%edx, %ecx		#ADD
					#DEREF
					#NON-LVALUE DEREF
	movl		(%ecx), %ecx
	pushl	%ecx		#Call: Push arguments[1]  int *
	pushl	-4(%ebp)		#Call: Push arguments[0]  int **
	movl	%eax, -80(%ebp)
	movl	%ecx, -84(%ebp)
	call	insert		#Call: function call
	addl	$8, %esp		#Call: stack pointer readjustment
	movl		%eax, -4(%ebp)		#Assignment
				#ADDR
				#ADDR
	leal	-44(%ebp), %ecx
	movl	$5, %edx
	imull	$4, %edx		#MUL
	addl		%edx, %ecx		#ADD
					#DEREF
					#NON-LVALUE DEREF
	movl		(%ecx), %ecx
	pushl	%ecx		#Call: Push arguments[1]  int *
	pushl	-4(%ebp)		#Call: Push arguments[0]  int **
	movl	%eax, -88(%ebp)
	movl	%ecx, -92(%ebp)
	call	insert		#Call: function call
	addl	$8, %esp		#Call: stack pointer readjustment
	movl		%eax, -4(%ebp)		#Assignment
				#ADDR
				#ADDR
	leal	-44(%ebp), %ecx
	movl	$2, %edx
	imull	$4, %edx		#MUL
	addl		%edx, %ecx		#ADD
					#DEREF
					#NON-LVALUE DEREF
	movl		(%ecx), %ecx
	pushl	%ecx		#Call: Push arguments[1]  int *
	pushl	-4(%ebp)		#Call: Push arguments[0]  int **
	movl	%eax, -96(%ebp)
	movl	%ecx, -100(%ebp)
	call	insert		#Call: function call
	addl	$8, %esp		#Call: stack pointer readjustment
	movl		%eax, -4(%ebp)		#Assignment
				#ADDR
				#ADDR
	leal	-44(%ebp), %ecx
	movl	$3, %edx
	imull	$4, %edx		#MUL
	addl		%edx, %ecx		#ADD
					#DEREF
					#NON-LVALUE DEREF
	movl		(%ecx), %ecx
	pushl	%ecx		#Call: Push arguments[1]  int *
	pushl	-4(%ebp)		#Call: Push arguments[0]  int **
	movl	%eax, -104(%ebp)
	movl	%ecx, -108(%ebp)
	call	insert		#Call: function call
	addl	$8, %esp		#Call: stack pointer readjustment
	movl		%eax, -4(%ebp)		#Assignment
				#ADDR
				#ADDR
	leal	-44(%ebp), %ecx
	movl	$6, %edx
	imull	$4, %edx		#MUL
	addl		%edx, %ecx		#ADD
					#DEREF
					#NON-LVALUE DEREF
	movl		(%ecx), %ecx
	pushl	%ecx		#Call: Push arguments[1]  int *
	pushl	-4(%ebp)		#Call: Push arguments[0]  int **
	movl	%eax, -112(%ebp)
	movl	%ecx, -116(%ebp)
	call	insert		#Call: function call
	addl	$8, %esp		#Call: stack pointer readjustment
	movl		%eax, -4(%ebp)		#Assignment
				#ADDR
	.data
.L20:	.asciz	"preorder traversal:\n"
	.text
	leal	.L20, %ecx
	pushl	%ecx		#Call: Push arguments[0]  char *
	movl	%eax, -120(%ebp)
	movl	%ecx, -124(%ebp)
	call	printf		#Call: function call
	addl	$4, %esp		#Call: stack pointer readjustment
	pushl	-4(%ebp)		#Call: Push arguments[0]  int **
	movl	%eax, -128(%ebp)
	call	preorder		#Call: function call
	addl	$4, %esp		#Call: stack pointer readjustment
				#ADDR
	.data
.L21:	.asciz	"inorder traversal:\n"
	.text
	leal	.L21, %ecx
	pushl	%ecx		#Call: Push arguments[0]  char *
	movl	%eax, -132(%ebp)
	movl	%ecx, -136(%ebp)
	call	printf		#Call: function call
	addl	$4, %esp		#Call: stack pointer readjustment
	pushl	-4(%ebp)		#Call: Push arguments[0]  int **
	movl	%eax, -140(%ebp)
	call	inorder		#Call: function call
	addl	$4, %esp		#Call: stack pointer readjustment
main.exit:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.set	main.size, 140
	.globl	main

	.comm	null, 4
