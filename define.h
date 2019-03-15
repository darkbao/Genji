/*
* @Author:             darkbao
* @Date:               2019-03-14 09:31:40
* @Last Modified by:   darkbao
* @Email:              freecare_j@163.com
*/
#ifndef __MJ_DEFINE_H__
#define __MJ_DEFINE_H__

#ifdef CONFIG_SMP
#define smp_mb()	mb()
#define smp_rmb()	rmb()
#define smp_wmb()	wmb()
#else
#define smp_mb() 	barrier()
#define smp_rmb()	barrier()
#define smp_wmb()	barrier()
#endif

#define barrier()	__asm__ __volatile__("": : :"memory")
/*
 * Force strict CPU ordering.
 * And yes, this is required on UP too when we're talking
 * to devices.
 */
#ifdef CONFIG_X86_32
/*
 * Some non-Intel clones support out of order store. wmb() ceases to be a
 * nop for these.
 */
#define mb() 	alternative("lock; addl $0,0(%%esp)", "mfence", X86_FEATURE_XMM2)
#define rmb()	alternative("lock; addl $0,0(%%esp)", "lfence", X86_FEATURE_XMM2)
#define wmb()	alternative("lock; addl $0,0(%%esp)", "sfence", X86_FEATURE_XMM)
#else
#define mb()    asm volatile("mfence":::"memory")
#define rmb()   asm volatile("lfence":::"memory")
#define wmb()   asm volatile("sfence" ::: "memory")
#endif

#define cpu_pause()   __asm__ __volatile__("pause;")

#endif // __MJ_DEFINE_H__