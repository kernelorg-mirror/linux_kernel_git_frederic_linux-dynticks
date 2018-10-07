/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_BH_H
#define _LINUX_BH_H

#include <linux/preempt.h>

/* PLEASE, avoid to allocate new softirqs, if you need not _really_ high
   frequency threaded job scheduling. For almost all the purposes
   tasklets are more than enough. F.e. all serial device BHs et
   al. should be converted to tasklets, not to softirqs.
 */

enum
{
	HI_SOFTIRQ=0,
	TIMER_SOFTIRQ,
	NET_TX_SOFTIRQ,
	NET_RX_SOFTIRQ,
	BLOCK_SOFTIRQ,
	IRQ_POLL_SOFTIRQ,
	TASKLET_SOFTIRQ,
	SCHED_SOFTIRQ,
	HRTIMER_SOFTIRQ, /* Unused, but kept as tools rely on the
			    numbering. Sigh! */
	RCU_SOFTIRQ,    /* Preferable RCU should always be the last softirq */

	NR_SOFTIRQS
};

#define SOFTIRQ_STOP_IDLE_MASK (~(1 << RCU_SOFTIRQ))
#define SOFTIRQ_ALL_MASK (BIT(NR_SOFTIRQS) - 1)

#define SOFTIRQ_ENABLED_SHIFT 16
#define SOFTIRQ_PENDING_MASK (BIT(SOFTIRQ_ENABLED_SHIFT) - 1)

#define SOFTIRQ_DATA_INIT (SOFTIRQ_ALL_MASK << SOFTIRQ_ENABLED_SHIFT)


#ifdef CONFIG_TRACE_IRQFLAGS
extern unsigned int __local_bh_disable_ip(unsigned long ip, unsigned int cnt,
					  unsigned int mask);
#else
static __always_inline unsigned int __local_bh_disable_ip(unsigned long ip, unsigned int cnt)
{
	preempt_count_add(cnt);
	barrier();
}
#endif

static inline unsigned int local_bh_disable(unsigned int mask)
{
	return __local_bh_disable_ip(_THIS_IP_, SOFTIRQ_OFFSET, mask);
}

extern void local_bh_enable_no_softirq(unsigned int bh);
extern void __local_bh_enable_ip(unsigned long ip,
				 unsigned int cnt, unsigned int bh);

static inline void local_bh_enable_ip(unsigned long ip, unsigned int bh)
{
	__local_bh_enable_ip(ip, SOFTIRQ_OFFSET, bh);
}

static inline void local_bh_enable(unsigned int bh)
{
	__local_bh_enable_ip(_THIS_IP_, SOFTIRQ_OFFSET, bh);
}

extern void local_bh_disable_all(void);
extern void local_bh_enable_all(void);

#endif /* _LINUX_BH_H */
