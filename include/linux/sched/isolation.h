#ifndef _LINUX_SCHED_ISOLATION_H
#define _LINUX_SCHED_ISOLATION_H

#include <linux/cpumask.h>
#include <linux/init.h>
#include <linux/tick.h>
#include <linux/percpu-rwsem.h>

enum hk_type {
	HK_TYPE_NOHZ_FULL,
	HK_TYPE_SCHED,
	HK_TYPE_DOMAIN,
	HK_TYPE_MANAGED_IRQ,
	HK_TYPE_MAX
};

#ifdef CONFIG_CPU_ISOLATION
DECLARE_STATIC_KEY_FALSE(housekeeping_overridden);
extern struct percpu_rw_semaphore housekeeping_rwsem;

static inline void hk_down_read(void)
{
	percpu_down_read(&housekeeping_rwsem);
}

static inline void hk_up_read(void)
{
	percpu_up_read(&housekeeping_rwsem);
}

extern int housekeeping_any_cpu(enum hk_type type);
extern const struct cpumask *housekeeping_cpumask(enum hk_type type);
extern bool housekeeping_enabled(enum hk_type type);
extern void housekeeping_affine(struct task_struct *t, enum hk_type type);
extern bool housekeeping_test_cpu(int cpu, enum hk_type type);
extern int housekeeping_cpumask_set(struct cpumask *cpumask, enum hk_type type);
extern int housekeeping_cpumask_clear(struct cpumask *cpumask, enum hk_type type);
extern void __init housekeeping_init(void);

#else

static inline void hk_down_read(void) { }
static inline void hk_up_read(void) { }

static inline int housekeeping_any_cpu(enum hk_type type)
{
	return smp_processor_id();
}

static inline const struct cpumask *housekeeping_cpumask(enum hk_type type)
{
	return cpu_possible_mask;
}

static inline bool housekeeping_enabled(enum hk_type type)
{
	return false;
}

static inline void housekeeping_affine(struct task_struct *t,
				       enum hk_type type) { }

static inline int housekeeping_cpumask_set(struct cpumask *cpumask, enum hk_type type)
{
	return -EINVAL;
}

static inline int housekeeping_cpumask_clear(struct cpumask *cpumask, enum hk_type type)
{
	return -EINVAL;
}

static inline void housekeeping_init(void) { }
#endif /* CONFIG_CPU_ISOLATION */

static inline bool housekeeping_cpu(int cpu, enum hk_type type)
{
#ifdef CONFIG_CPU_ISOLATION
	if (static_branch_unlikely(&housekeeping_overridden))
		return housekeeping_test_cpu(cpu, type);
#endif
	return true;
}

#endif /* _LINUX_SCHED_ISOLATION_H */
