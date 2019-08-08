	.globl	main
	.type	main, @function
main:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$4,  %esp

	movl	$10, -4(%ebp)

	movl	-4(%ebp), %eax
	movl	%eax, 0(%esp)
	call	init_array

	movl	-4(%ebp), %eax
	movl	%eax, 0(%esp)
	call	print_array

	leave
	ret

	.comm	a,40
