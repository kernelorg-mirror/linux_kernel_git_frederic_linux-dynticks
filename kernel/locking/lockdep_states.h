/*
 * Lockdep states,
 *
 * please update XXX_LOCK_USAGE_STATES in include/linux/lockdep.h whenever
 * you add one, or come up with a nice dynamic solution.
 */
LOCKDEP_STATE(HARDIRQ)
#define SOFTIRQ_VECTOR(__SVEC) LOCKDEP_STATE(__SVEC##_SOFTIRQ)
#include <linux/softirq_vector.h>
#undef SOFTIRQ_VECTOR
