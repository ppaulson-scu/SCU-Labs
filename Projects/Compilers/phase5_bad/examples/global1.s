	.globl	foo
	.type	foo, @function
foo:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$0,  %esp

	movl	$1, %eax
	movl	%eax, x

	movl	$2, %eax
	movl	%eax, y

	movl	$3, %eax
	movl	%eax, z

	leave
	ret

	.comm	x,4
	.comm	y,4
	.comm	z,4
