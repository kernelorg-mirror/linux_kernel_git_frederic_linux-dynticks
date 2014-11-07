#ifndef __LINUX_CPUTIME_H
#define __LINUX_CPUTIME_H

#include <asm/cputime.h>

#ifndef cputime_to_nsecs
# define cputime_to_nsecs(__ct)	\
	(cputime_to_usecs(__ct) * NSEC_PER_USEC)
#endif

#ifndef nsecs_to_cputime
# define nsecs_to_cputime(__nsecs)	\
	usecs_to_cputime((__nsecs) / NSEC_PER_USEC)
#endif

#ifndef nsecs_to_cputime
# define nsecs_to_cputime(__nsecs)	\
	usecs_to_cputime((__nsecs) / NSEC_PER_USEC)
#endif

#ifndef nsecs_to_cputime64
# define nsecs_to_cputime64(__nsecs)	\
	((__force cputime64_t) nsecs_to_cputime(__nsecs))
#endif

#ifndef nsecs_to_scaled
static inline u64 nsecs_to_scaled(u64 nsecs)
{
	cputime_t cputime, scaled;

	cputime = nsecs_to_cputime(nsecs);
	scaled = cputime_to_scaled(cputime);

	return cputime_to_nsecs(scaled);
}
#endif

#endif /* __LINUX_CPUTIME_H */
