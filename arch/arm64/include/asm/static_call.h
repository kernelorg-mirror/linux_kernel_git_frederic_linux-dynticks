/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_STATIC_CALL_H
#define _ASM_STATIC_CALL_H

#define __ARCH_DEFINE_STATIC_CALL_TRAMP(name, insn)			    \
	asm("	.pushsection	.static_call.text, \"ax\"		\n" \
	    "	.align		4					\n" \
	    "	.globl		" STATIC_CALL_TRAMP_STR(name) "		\n" \
	    "0:	.quad	0x0						\n" \
	    STATIC_CALL_TRAMP_STR(name) ":				\n" \
	    "	hint 	34	/* BTI C */				\n" \
		insn "							\n" \
	    "	ldr	x16, 0b						\n" \
	    "	cbz	x16, 1f						\n" \
	    "	br	x16						\n" \
	    "1:	ret							\n" \
	    "	.popsection						\n")

#define ARCH_DEFINE_STATIC_CALL_TRAMP(name, func)			\
	__ARCH_DEFINE_STATIC_CALL_TRAMP(name, "b " #func)

#define ARCH_DEFINE_STATIC_CALL_NULL_TRAMP(name)			\
	__ARCH_DEFINE_STATIC_CALL_TRAMP(name, "ret")

#endif /* _ASM_STATIC_CALL_H */
