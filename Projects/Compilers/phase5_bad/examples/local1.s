	.globl	main
	.type	main, @function
main:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$12,  %esp

	movl	$1, -4(%ebp)

	movl	$2, -8(%ebp)

	movl	$3, -12(%ebp)

	movl	-4(%ebp), %eax
	movl	%eax, 8(%esp)
	movl	-8(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	-12(%ebp), %eax
	movl	%eax, 0(%esp)
	call	print

	leave
	ret

