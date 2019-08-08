	.globl	main
	.type	main, @function
main:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$0,  %esp

	movl	$83, %eax
	movl	%eax, 0(%esp)
	call	putchar

	movl	$101, %eax
	movl	%eax, 0(%esp)
	call	putchar

	movl	$103, %eax
	movl	%eax, 0(%esp)
	call	putchar

	movl	$109, %eax
	movl	%eax, 0(%esp)
	call	putchar

	movl	$101, %eax
	movl	%eax, 0(%esp)
	call	putchar

	movl	$110, %eax
	movl	%eax, 0(%esp)
	call	putchar

	movl	$116, %eax
	movl	%eax, 0(%esp)
	call	putchar

	movl	$97, %eax
	movl	%eax, 0(%esp)
	call	putchar

	movl	$116, %eax
	movl	%eax, 0(%esp)
	call	putchar

	movl	$105, %eax
	movl	%eax, 0(%esp)
	call	putchar

	movl	$111, %eax
	movl	%eax, 0(%esp)
	call	putchar

	movl	$110, %eax
	movl	%eax, 0(%esp)
	call	putchar

	movl	$32, %eax
	movl	%eax, 0(%esp)
	call	putchar

	movl	$102, %eax
	movl	%eax, 0(%esp)
	call	putchar

	movl	$97, %eax
	movl	%eax, 0(%esp)
	call	putchar

	movl	$117, %eax
	movl	%eax, 0(%esp)
	call	putchar

	movl	$108, %eax
	movl	%eax, 0(%esp)
	call	putchar

	movl	$116, %eax
	movl	%eax, 0(%esp)
	call	putchar

	movl	$10, %eax
	movl	%eax, 0(%esp)
	call	putchar

	leave
	ret

