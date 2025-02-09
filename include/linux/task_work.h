/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_TASK_WORK_H
#define _LINUX_TASK_WORK_H

#include <linux/list.h>
#include <linux/sched.h>

#define TASK_WORK_DEQUEUED	((void *) -1UL)

typedef void (*task_work_func_t)(struct callback_head *);

static inline void
init_task_work(struct callback_head *twork, task_work_func_t func)
{
	twork->func = func;
	twork->next = TASK_WORK_DEQUEUED;
}

enum task_work_notify_mode {
	TWA_NONE = 0,
	TWA_RESUME,
	TWA_SIGNAL,
	TWA_SIGNAL_NO_IPI,
	TWA_NMI_CURRENT,
};

static inline bool task_work_pending(struct task_struct *task)
{
	return READ_ONCE(task->task_works);
}

/*
 * Check if a work is queued. Beware: this is inherently racy if the work can
 * be queued elsewhere than the current task.
 */
static inline bool task_work_queued(struct callback_head *twork)
{
	return twork->next != TASK_WORK_DEQUEUED;
}

int task_work_add(struct task_struct *task, struct callback_head *twork,
			enum task_work_notify_mode mode);

struct callback_head *task_work_cancel_match(struct task_struct *task,
	bool (*match)(struct callback_head *, void *data), void *data);
struct callback_head *task_work_cancel_func(struct task_struct *, task_work_func_t);
bool task_work_cancel(struct task_struct *task, struct callback_head *cb);
void task_work_run(void);

static inline void exit_task_work(struct task_struct *task)
{
	task_work_run();
}

#endif	/* _LINUX_TASK_WORK_H */
