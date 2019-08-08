	.comm	a,40,32
	.globl	main
	.type	main, @function
main:
	pushl	%ebp
	movl	%esp, %ebp
	andl	$-16, %esp
	subl	$32, %esp

	movl	$10, 28(%esp)

	movl	28(%esp), %eax
	movl	%eax, (%esp)
	call	init_array

	movl	28(%esp), %eax
	movl	%eax, (%esp)
	call	print_array


	leave

	ret

