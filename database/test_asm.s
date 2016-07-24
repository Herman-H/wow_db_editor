	.file	"test.cpp"
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC0:
	.string	"Tuple initialized to <0,1,2,3>:\n"
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC1:
	.string	"Index 0 = %d\n"
.LC2:
	.string	"Index 1 = %d\n"
.LC4:
	.string	"Index 2 = %f\n"
.LC6:
	.string	"Index 3 = %f\n"
	.text
	.p2align 4,,15
	.globl	_Z10test_tuplev
	.type	_Z10test_tuplev, @function
_Z10test_tuplev:
.LFB110:
	.cfi_startproc
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	movl	$.LC0, %esi
	movl	$1, %edi
	xorl	%eax, %eax
	call	__printf_chk
	xorl	%edx, %edx
	movl	$.LC1, %esi
	movl	$1, %edi
	xorl	%eax, %eax
	call	__printf_chk
	movl	$1, %edx
	movl	$.LC2, %esi
	movl	$1, %edi
	xorl	%eax, %eax
	call	__printf_chk
	movsd	.LC3(%rip), %xmm0
	movl	$.LC4, %esi
	movl	$1, %edi
	movl	$1, %eax
	call	__printf_chk
	movsd	.LC5(%rip), %xmm0
	movl	$.LC6, %esi
	movl	$1, %edi
	movl	$1, %eax
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	jmp	__printf_chk
	.cfi_endproc
.LFE110:
	.size	_Z10test_tuplev, .-_Z10test_tuplev
	.section	.rodata.str1.1
.LC7:
	.string	"Hej!"
.LC8:
	.string	"String is: %s\n"
.LC9:
	.string	"Size is: %d\n"
.LC10:
	.string	"String[0] is: %s\n"
.LC11:
	.string	"and its size is: %d\n"
.LC12:
	.string	"Hejda"
.LC13:
	.string	"String[1] is: %s\n"
.LC14:
	.string	"Lol"
.LC15:
	.string	"String[2] is: %s\n"
.LC16:
	.string	"Total size of strings is: %d\n"
	.text
	.p2align 4,,15
	.globl	_Z18test_sized_cstringv
	.type	_Z18test_sized_cstringv, @function
_Z18test_sized_cstringv:
.LFB111:
	.cfi_startproc
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	movl	$.LC7, %edx
	movl	$.LC8, %esi
	movl	$1, %edi
	xorl	%eax, %eax
	call	__printf_chk
	movl	$5, %edx
	movl	$.LC9, %esi
	movl	$1, %edi
	xorl	%eax, %eax
	call	__printf_chk
	movl	$.LC7, %edx
	movl	$.LC10, %esi
	movl	$1, %edi
	xorl	%eax, %eax
	call	__printf_chk
	movl	$5, %edx
	movl	$.LC11, %esi
	movl	$1, %edi
	xorl	%eax, %eax
	call	__printf_chk
	movl	$.LC12, %edx
	movl	$.LC13, %esi
	movl	$1, %edi
	xorl	%eax, %eax
	call	__printf_chk
	movl	$6, %edx
	movl	$.LC11, %esi
	movl	$1, %edi
	xorl	%eax, %eax
	call	__printf_chk
	movl	$.LC14, %edx
	movl	$.LC15, %esi
	movl	$1, %edi
	xorl	%eax, %eax
	call	__printf_chk
	movl	$4, %edx
	movl	$.LC11, %esi
	movl	$1, %edi
	xorl	%eax, %eax
	call	__printf_chk
	movl	$12, %edx
	movl	$.LC16, %esi
	movl	$1, %edi
	xorl	%eax, %eax
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	jmp	__printf_chk
	.cfi_endproc
.LFE111:
	.size	_Z18test_sized_cstringv, .-_Z18test_sized_cstringv
	.section	.text.startup,"ax",@progbits
	.p2align 4,,15
	.globl	main
	.type	main, @function
main:
.LFB112:
	.cfi_startproc
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	call	_Z10test_tuplev
	call	_Z18test_sized_cstringv
	xorl	%eax, %eax
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE112:
	.size	main, .-main
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC3:
	.long	0
	.long	1073741824
	.align 8
.LC5:
	.long	0
	.long	1074266112
	.ident	"GCC: (Ubuntu/Linaro 4.7.2-2ubuntu1) 4.7.2"
	.section	.note.GNU-stack,"",@progbits
