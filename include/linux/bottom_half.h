/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_BH_H
#define _LINUX_BH_H

#include <linux/preempt.h>


/*
 * PLEASE, avoid to allocate new softirqs, if you need not _really_ high
 * frequency threaded job scheduling. For almost all the purposes
 * tasklets are more than enough. F.e. all serial device BHs et
 * al. should be converted to tasklets, not to softirqs.
 */
enum
{
#define SOFTIRQ_VECTOR(__SVEC) \
	__SVEC##_SOFTIRQ,
#include <linux/softirq_vector.h>
#undef SOFTIRQ_VECTOR
	NR_SOFTIRQS
};

#define SOFTIRQ_STOP_IDLE_MASK (~(1 << RCU_SOFTIRQ))
#define SOFTIRQ_ALL_MASK (BIT(NR_SOFTIRQS) - 1)

#define SOFTIRQ_ENABLED_SHIFT 16
#define SOFTIRQ_PENDING_MASK (BIT(SOFTIRQ_ENABLED_SHIFT) - 1)

#define SOFTIRQ_DATA_INIT (SOFTIRQ_ALL_MASK << SOFTIRQ_ENABLED_SHIFT)

extern void __local_bh_disable_ip(unsigned long ip, unsigned int cnt);

static inline void local_bh_disable(void)
{
	__local_bh_disable_ip(_THIS_IP_, SOFTIRQ_DISABLE_OFFSET);
}

extern unsigned int local_bh_disable_mask(unsigned long ip,
					  unsigned int cnt, unsigned int mask);


extern void local_bh_enable_no_softirq(void);
extern void __local_bh_enable_ip(unsigned long ip, unsigned int cnt);

static inline void local_bh_enable_ip(unsigned long ip)
{
	__local_bh_enable_ip(ip, SOFTIRQ_DISABLE_OFFSET);
}

static inline void local_bh_enable(void)
{
	__local_bh_enable_ip(_THIS_IP_, SOFTIRQ_DISABLE_OFFSET);
}

extern void local_bh_enable_mask(unsigned long ip, unsigned int cnt,
				 unsigned int mask);

#endif /* _LINUX_BH_H */
