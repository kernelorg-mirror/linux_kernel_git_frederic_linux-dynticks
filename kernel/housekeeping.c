/*
 *  Housekeeping management. Manage the targets for routine code that can run on
 *  any CPU: unbound workqueues, timers, kthreads and any offloadable work.
 *
 * Copyright (C) 2017 Red Hat, Inc., Frederic Weisbecker
 *
 */

#include <linux/housekeeping.h>
#include <linux/tick.h>
#include <linux/init.h>
#include <linux/static_key.h>

DEFINE_STATIC_KEY_FALSE(housekeeping_overriden);
EXPORT_SYMBOL_GPL(housekeeping_overriden);
static cpumask_var_t housekeeping_mask;

int housekeeping_any_cpu(void)
{
	if (static_branch_unlikely(&housekeeping_overriden))
		return cpumask_any_and(housekeeping_mask, cpu_online_mask);

	return smp_processor_id();
}

const struct cpumask *housekeeping_cpumask(void)
{
	if (static_branch_unlikely(&housekeeping_overriden))
		return housekeeping_mask;

	return cpu_possible_mask;
}

void housekeeping_affine(struct task_struct *t)
{
	if (static_branch_unlikely(&housekeeping_overriden))
		set_cpus_allowed_ptr(t, housekeeping_mask);
}

bool housekeeping_test_cpu(int cpu)
{
	if (static_branch_unlikely(&housekeeping_overriden))
		return cpumask_test_cpu(cpu, housekeeping_mask);

	return true;
}

/* Parse the boot-time housekeeping CPU list from the kernel parameters. */
static int __init housekeeping_setup(char *str)
{
	alloc_bootmem_cpumask_var(&housekeeping_mask);
	if (cpulist_parse(str, housekeeping_mask) < 0) {
		pr_warn("Housekeeping: Incorrect cpumask\n");
		free_bootmem_cpumask_var(housekeeping_mask);
		return 1;
	}

	static_branch_enable(&housekeeping_overriden);

	/* We need at least one CPU to handle housekeeping work */
	WARN_ON_ONCE(cpumask_empty(housekeeping_mask));

	return 1;
}
__setup("housekeeping=", housekeeping_setup);
