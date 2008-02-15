
/* -*- linux-c -*- */

/*
 * Author : Stephen Smalley, <sds@epoch.ncsc.mil> 
 */

#ifndef _LINUX_FLASK_TYPES_H_
#define _LINUX_FLASK_TYPES_H_

/*
 * The basic Flask types and constants.
 */

#include <asm/types.h>

/*
 * A security context is a set of security attributes 
 * associated with each subject and object controlled
 * by the security policy.  The security context type
 * is defined as a variable-length string that can be
 * interpreted by any application or user with an 
 * understanding of the security policy.
 */
typedef char* security_context_t;

/*
 * A security identifier (SID) is a fixed-size value
 * that is mapped by the security server to a 
 * particular security context.  The SID mapping
 * cannot be assumed to be consistent either across
 * executions (reboots) of the security server or
 * across security servers on different nodes.
 *
 * Certain SIDs (specified in flask/initial_sids) are 
 * predefined for system initialization. The corresponding
 * constants are defined in the automatically generated
 * header file flask.h.
 */
typedef __u32 security_id_t;
#define SECSID_NULL			0x00000000 /* unspecified SID */
#define SECSID_WILD			0xFFFFFFFF /* wildcard SID */

/*
 * An access vector (AV) is a collection of related permissions
 * for a pair of SIDs.  The bits within an access vector
 * are interpreted differently depending on the class of
 * the object.  The access vector interpretations are specified
 * in flask/access_vectors, and the corresponding constants
 * for permissions are defined in the automatically generated
 * header file av_permissions.h.
 */
typedef __u32 access_vector_t;

/*
 * Each object class is identified by a fixed-size value.
 * The set of security classes is specified in flask/security_classes, 
 * with the corresponding constants defined in the automatically 
 * generated header file flask.h.
 */
typedef __u16 security_class_t;
#define SECCLASS_NULL			0x0000 /* no class */

/*
 * A persistent security identifier (PSID) is a fixed-size
 * value that is assigned by the file system component
 * to each security context associated with an object
 * in the file system.  A separate PSID mapping is
 * maintained for each file system.
 */
typedef __u32 psid_t;
struct psidtab;

#define CUR_SID current->sid 

#ifdef __KERNEL__
#include <linux/sched.h>
#include <linux/spinlock.h>
#include <linux/interrupt.h>
#include <asm/semaphore.h>

/*
 * Use this for memory allocation flags (e.g. for kmalloc()) when the 
 * interrupt state is unknown.
 */
#define SAFE_ALLOC (in_interrupt () ? GFP_ATOMIC : GFP_KERNEL)

/*
 * Use these for semaphores when the interrupt state is unknown.
 */

static inline int safe_down(struct semaphore *sem)
{
	if (in_interrupt()) {
		if (down_trylock(sem))
			return -EAGAIN;
	} else {
		down(sem);
	}
	return 0;
}

static inline void safe_up(struct semaphore *sem)
{
	up(sem);
}
#endif

#endif
