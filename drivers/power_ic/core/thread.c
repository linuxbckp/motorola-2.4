/*
 * Copyright 2005-2006 Motorola, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as 
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307, USA
 *
 *  Author     Date                Comment
 * ======== ===========   =====================================================
 * Motorola 2005-Jul-25 - Design of the common thread handling functions and definitions.
 * Motorola 2006-Jan-10 - Finalize the thread handling functions.
 * Motorola 2006-Jan-30 - Add lock_kernel when setup thread.i
 * Motorola 2006-Oct-25 - Update comments.
 */

/*!
 * @file thread.c
 *
 * @ingroup poweric_core
 *
 * @brief This file contains common thread handling functions and definitions.
 *
 */
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/smp_lock.h>
#include <linux/version.h>

#include "thread.h"

/*!
 * @brief Performs common calls to set up a kernel thread correctly.
 *
 * This function calls everything that is needed for a kernel thread to identify itself
 * and turn itself into a daemon. This should be the first function called at the start
 * of each new kernel thread.
 *
 * @param thread_name  String constant that will be used to name the thread.
 *
 * @note If the thread needs to react to events, etc. then it is suggested that 
 * thread_set_realtime_priority be called after this function to set the thread up
 * as a realtime process and raise its priority from the default.
 */
void thread_common_setup(char * thread_name)
{
    lock_kernel();
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0))
    daemonize();
    strcpy(current->comm, thread_name);
#else
    daemonize(thread_name);
#endif
    reparent_to_init();
    /* Ignore all signals, but those which terminate the thread. */
    siginitsetinv(&current->blocked, sigmask(SIGKILL)|sigmask(SIGINT)|sigmask(SIGTERM));
    unlock_kernel();
}

/*!
 * @brief Sets a realtime thread's priority.
 *
 * This function sets up a thread as a "realtime" thread and sets the thread's priority
 * to that passed to the function. The function doesn't do much other than hide the
 * use of setscheduler() and the annoying structure that needs to go with its call.
 *
 * @param priority     New priority for the thread.
 *
 * @note Linux "realtime" threads are realtime in name only - they still experiencce
 * significant latencies in waking up, etc. This function just tells the scheduler to
 * treat this thread with a little more priority than usual.
 */
int thread_set_realtime_priority(int priority)
{
    struct sched_param thread_sched_params = {.sched_priority = priority};
    
    /* The name for this function was changed in Linux 2.6, presumably for a laugh. */
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0))
    return (setscheduler(0, SCHED_FIFO, &thread_sched_params));
#else
    return (sched_setscheduler(current, SCHED_FIFO, &thread_sched_params));
#endif
}
