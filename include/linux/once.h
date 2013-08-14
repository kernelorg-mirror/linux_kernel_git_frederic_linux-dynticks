#ifndef _LINUX_ONCE_H
#define _LINUX_ONCE_H

#include <linux/compiler.h>

#define DO_COND(condition, to_do) ({		\
	int __ret = !!(condition);		\
	if (unlikely(__ret)) {			\
		to_do;				\
	}					\
	unlikely(__ret);			\
})

#define DO_ONCE(to_do) ({			\
	static bool __done;			\
						\
	if (!__done) {				\
		__done = true;			\
		to_do;				\
	}					\
})

#define DO_ONCE_COND(condition, to_do)		\
	DO_COND(condition, DO_ONCE(to_do))

#endif /* _LINUX_ONCE_H */
