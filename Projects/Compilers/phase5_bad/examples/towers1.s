	.globl	towers
	.type	towers, @function
towers:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$16,  %esp

	movl	-12(%ebp), %eax
	movl	%eax, 12(%esp)
	movl	-16(%ebp), %eax
	movl	%eax, 8(%esp)
	movl	-8(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	-4(%ebp), %eax
	movl	%eax, 0(%esp)
	call	call_towers

	movl	-12(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	-8(%ebp), %eax
	movl	%eax, 0(%esp)
	call	print_move

	movl	-8(%ebp), %eax
	movl	%eax, 12(%esp)
	movl	-12(%ebp), %eax
	movl	%eax, 8(%esp)
	movl	-16(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	-4(%ebp), %eax
	movl	%eax, 0(%esp)
	call	call_towers

	leave
	ret

	.globl	main
	.type	main, @function
main:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$4,  %esp

	movl	$3, -4(%ebp)

	movl	-4(%ebp), %eax
	movl	%eax, 0(%esp)
	call	print

	movl	$3, %eax
	movl	%eax, 12(%esp)
	movl	$2, %eax
	movl	%eax, 8(%esp)
	movl	$1, %eax
	movl	%eax, 4(%esp)
	movl	-4(%ebp), %eax
	movl	%eax, 0(%esp)
	call	towers

	leave
	ret

