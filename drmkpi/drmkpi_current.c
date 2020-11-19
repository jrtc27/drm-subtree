/*-
 * Copyright (c) 2017 Hans Petter Selasky
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice unmodified, this list of conditions, and the following
 *    disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <sys/cdefs.h>
__FBSDID("$FreeBSD$");

#include <sys/kernel.h>
#include <sys/eventhandler.h>
#include <sys/malloc.h>

#include <linux/compat.h>

static MALLOC_DEFINE(M_DRMKPI_CURRENT, "drmkpcurrent", "LinuxKPI task structure");

int
drmkpi_alloc_current(struct thread *td, int flags)
{
	struct proc *proc;
	struct task_struct *ts;

	MPASS(td->td_lkpi_task == NULL);

	ts = malloc(sizeof(*ts), M_DRMKPI_CURRENT, flags | M_ZERO);
	if (ts == NULL)
		return (ENOMEM);

	/* setup new task structure */
	ts->task_thread = td;
	atomic_set(&ts->state, TASK_RUNNING);

	proc = td->td_proc;

	/* store pointer to task struct */
	PROC_LOCK(proc);
	td->td_lkpi_task = ts;
	PROC_UNLOCK(proc);

	return (0);
}
