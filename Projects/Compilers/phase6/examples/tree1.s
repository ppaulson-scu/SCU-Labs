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
	movl	, %ecx
	movl		%ecx, 8(%ebp)		#Assignment
	movl	12(%ebp), %edx
				#SPILL
	movl	%eax, -8(%ebp)
	movl	, %eax
	movl		%edx, (%eax)		#Deref Assignment
				#SPILL
	movl	%eax, -12(%ebp)
	movl	null, %eax
				#SPILL
	movl	%eax, -16(%ebp)
	movl	, %eax
	movl		-16(%ebp), (%eax)		#Deref Assignment
				#SPILL
	movl	%eax, -20(%ebp)
	movl	null, %eax
				#SPILL
	movl	%eax, -24(%ebp)
	movl	, %eax
	movl		-24(%ebp), (%eax)		#Deref Assignment
	jmp	.L1
.L0:
	movl	12(%ebp), %eax
				#cmpl
	cmpl		, %eax
	setl	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	je	.L2
	pushl	12(%ebp)		#Call: Push arguments[1]  int *
	movl	, %eax
				#CAST
	pushl			#Call: Push arguments[0]  int **
	movl	%eax, -28(%ebp)
	call	insert		#Call: function call
	addl	$8, %esp		#Call: stack pointer readjustment
				#CAST
	movl	, %ecx
	movl	, %edx
	movl		%ecx, (%edx)		#Deref Assignment
	jmp	.L3
.L2:
				#SPILL
	movl	%eax, -32(%ebp)
	movl	12(%ebp), %eax
				#cmpg
	cmpl		, %eax
	setg	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	je	.L4
	pushl	12(%ebp)		#Call: Push arguments[1]  int *
	movl	, %eax
				#CAST
	pushl			#Call: Push arguments[0]  int **
	movl	%eax, -36(%ebp)
	movl	%ecx, -40(%ebp)
	movl	%edx, -44(%ebp)
	call	insert		#Call: function call
	addl	$8, %esp		#Call: stack pointer readjustment
				#CAST
	movl	, %ecx
	movl	, %edx
	movl		%ecx, (%edx)		#Deref Assignment
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

	.set	insert.size, 0
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
	movl	12(%ebp), %eax
				#cmpl
	cmpl		, %eax
	setl	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	je	.L8
	pushl	12(%ebp)		#Call: Push arguments[1]  int *
	movl	, %eax
				#CAST
	pushl			#Call: Push arguments[0]  int **
	movl	%eax, -4(%ebp)
	call	search		#Call: function call
	addl	$8, %esp		#Call: stack pointer readjustment
				#RETURN
	jmp 	search.exit
.L8:
	movl	12(%ebp), %eax
				#cmpg
	cmpl		, %eax
	setg	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	je	.L10
	pushl	12(%ebp)		#Call: Push arguments[1]  int *
	movl	, %eax
				#CAST
	pushl			#Call: Push arguments[0]  int **
	movl	%eax, -8(%ebp)
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

	.set	search.size, 0
	.globl	search

preorder:		#Function
	pushl	%ebp
	movl	%esp, %ebp
	subl	$preorder.size, %esp
	movl	8(%ebp), %eax
	cmpl	$0, %eax
	je	.L12
	pushl			#Call: Push arguments[1]  int
	.data
.L14:	.asciz	"%d\n"
	.text
	leal	.L14, %eax
	pushl	%eax		#Call: Push arguments[0]  char *
	movl	%eax, -4(%ebp)
	call	printf		#Call: function call
	addl	$8, %esp		#Call: stack pointer readjustment
	movl	, %ecx
				#CAST
	pushl			#Call: Push arguments[0]  int **
	movl	%eax, -8(%ebp)
	movl	%ecx, -12(%ebp)
	call	preorder		#Call: function call
	addl	$4, %esp		#Call: stack pointer readjustment
	movl	, %ecx
				#CAST
	pushl			#Call: Push arguments[0]  int **
	movl	%eax, -16(%ebp)
	movl	%ecx, -20(%ebp)
	call	preorder		#Call: function call
	addl	$4, %esp		#Call: stack pointer readjustment
.L12:
preorder.exit:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.set	preorder.size, 0
	.globl	preorder

inorder:		#Function
	pushl	%ebp
	movl	%esp, %ebp
	subl	$inorder.size, %esp
	movl	8(%ebp), %eax
	cmpl	$0, %eax
	je	.L15
	movl	, %eax
				#CAST
	pushl			#Call: Push arguments[0]  int **
	movl	%eax, -4(%ebp)
	call	inorder		#Call: function call
	addl	$4, %esp		#Call: stack pointer readjustment
	pushl			#Call: Push arguments[1]  int
	.data
.L17:	.asciz	"%d\n"
	.text
	leal	.L17, %ecx
	pushl	%ecx		#Call: Push arguments[0]  char *
	movl	%eax, -8(%ebp)
	movl	%ecx, -12(%ebp)
	call	printf		#Call: function call
	addl	$8, %esp		#Call: stack pointer readjustment
	movl	, %ecx
				#CAST
	pushl			#Call: Push arguments[0]  int **
	movl	%eax, -16(%ebp)
	movl	%ecx, -20(%ebp)
	call	inorder		#Call: function call
	addl	$4, %esp		#Call: stack pointer readjustment
.L15:
inorder.exit:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.set	inorder.size, 0
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
	movl	-48(%ebp), %ecx
	movl	, %edx
	movl		%ecx, (%edx)		#Deref Assignment
					#-48(%ebp)
					#$1
				#SPILL
	movl	%eax, -52(%ebp)
	movl	-48(%ebp), %eax
	addl		$1, %eax		#ADD
	movl		%eax, -48(%ebp)		#Assignment
	jmp	.L18
.L19:
	movl	null, %eax
				#CAST
	movl	, %ecx
	movl		%ecx, -4(%ebp)		#Assignment
	pushl			#Call: Push arguments[1]  int *
	pushl	-4(%ebp)		#Call: Push arguments[0]  int **
	movl	%eax, -56(%ebp)
	movl	%ecx, -60(%ebp)
	call	insert		#Call: function call
	addl	$8, %esp		#Call: stack pointer readjustment
	movl		%eax, -4(%ebp)		#Assignment
	pushl			#Call: Push arguments[1]  int *
	pushl	-4(%ebp)		#Call: Push arguments[0]  int **
	movl	%eax, -64(%ebp)
	call	insert		#Call: function call
	addl	$8, %esp		#Call: stack pointer readjustment
	movl		%eax, -4(%ebp)		#Assignment
	pushl			#Call: Push arguments[1]  int *
	pushl	-4(%ebp)		#Call: Push arguments[0]  int **
	movl	%eax, -68(%ebp)
	call	insert		#Call: function call
	addl	$8, %esp		#Call: stack pointer readjustment
	movl		%eax, -4(%ebp)		#Assignment
	pushl			#Call: Push arguments[1]  int *
	pushl	-4(%ebp)		#Call: Push arguments[0]  int **
	movl	%eax, -72(%ebp)
	call	insert		#Call: function call
	addl	$8, %esp		#Call: stack pointer readjustment
	movl		%eax, -4(%ebp)		#Assignment
	pushl			#Call: Push arguments[1]  int *
	pushl	-4(%ebp)		#Call: Push arguments[0]  int **
	movl	%eax, -76(%ebp)
	call	insert		#Call: function call
	addl	$8, %esp		#Call: stack pointer readjustment
	movl		%eax, -4(%ebp)		#Assignment
	pushl			#Call: Push arguments[1]  int *
	pushl	-4(%ebp)		#Call: Push arguments[0]  int **
	movl	%eax, -80(%ebp)
	call	insert		#Call: function call
	addl	$8, %esp		#Call: stack pointer readjustment
	movl		%eax, -4(%ebp)		#Assignment
	pushl			#Call: Push arguments[1]  int *
	pushl	-4(%ebp)		#Call: Push arguments[0]  int **
	movl	%eax, -84(%ebp)
	call	insert		#Call: function call
	addl	$8, %esp		#Call: stack pointer readjustment
	movl		%eax, -4(%ebp)		#Assignment
	pushl			#Call: Push arguments[1]  int *
	pushl	-4(%ebp)		#Call: Push arguments[0]  int **
	movl	%eax, -88(%ebp)
	call	insert		#Call: function call
	addl	$8, %esp		#Call: stack pointer readjustment
	movl		%eax, -4(%ebp)		#Assignment
	.data
.L20:	.asciz	"preorder traversal:\n"
	.text
	leal	.L20, %ecx
	pushl	%ecx		#Call: Push arguments[0]  char *
	movl	%eax, -92(%ebp)
	movl	%ecx, -96(%ebp)
	call	printf		#Call: function call
	addl	$4, %esp		#Call: stack pointer readjustment
	pushl	-4(%ebp)		#Call: Push arguments[0]  int **
	movl	%eax, -100(%ebp)
	call	preorder		#Call: function call
	addl	$4, %esp		#Call: stack pointer readjustment
	.data
.L21:	.asciz	"inorder traversal:\n"
	.text
	leal	.L21, %ecx
	pushl	%ecx		#Call: Push arguments[0]  char *
	movl	%eax, -104(%ebp)
	movl	%ecx, -108(%ebp)
	call	printf		#Call: function call
	addl	$4, %esp		#Call: stack pointer readjustment
	pushl	-4(%ebp)		#Call: Push arguments[0]  int **
	movl	%eax, -112(%ebp)
	call	inorder		#Call: function call
	addl	$4, %esp		#Call: stack pointer readjustment
main.exit:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.set	main.size, 48
	.globl	main

	.comm	null, 4
