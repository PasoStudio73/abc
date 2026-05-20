/**CFile****************************************************************

  FileName    [rewireTime.h]

  SystemName  [ABC: Logic synthesis and verification system.]

  PackageName [Re-wiring.]

  Synopsis    []

  Author      [Jiun-Hao Chen]
  
  Affiliation [National Taiwan University]

  Date        [Ver. 1.0. Started - June 20, 2005.]

  Revision    [$Id: rewireTime.h,v 1.00 2005/06/20 00:00:00 alanmi Exp $]

***********************************************************************/

#ifndef RAR_TIME_H
#define RAR_TIME_H

/*************************************************************
                 counting wall time
**************************************************************/

#include "base/abc/abc.h"
#if defined(__APPLE__)
#include <sys/time.h>
#ifndef CLOCK_REALTIME
#define CLOCK_REALTIME 0
#endif
#ifndef CLOCK_MONOTONIC
#define CLOCK_MONOTONIC 0
#endif
static int abc_clock_gettime_compat(int clk_id, struct timespec *ts) {
    struct timeval tv; (void)clk_id;
    if (gettimeofday(&tv, NULL) != 0) return -1;
    ts->tv_sec = tv.tv_sec; ts->tv_nsec = tv.tv_usec * 1000; return 0;
}
#ifndef clock_gettime
#define clock_gettime abc_clock_gettime_compat
#endif
#endif

ABC_NAMESPACE_HEADER_START

static inline iword Time_Clock() {
#if defined(__APPLE__) && defined(__MACH__)
#define APPLE_MACH (__APPLE__ & __MACH__)
#else
#define APPLE_MACH 0
#endif
#if (defined(LIN) || defined(LIN64)) && !APPLE_MACH && !defined(__MINGW32__)
    struct timespec ts;
    if (clock_gettime(CLOCK_MONOTONIC, &ts) < 0)
        return (iword)-1;
    iword res = ((iword)ts.tv_sec) * CLOCKS_PER_SEC;
    res += (((iword)ts.tv_nsec) * CLOCKS_PER_SEC) / 1000000000;
    return res;
#else
    return (iword)clock();
#endif
}

static inline void Time_Print(const char *pStr, iword time) {
    printf("%s = %10.2f sec", pStr, (float)1.0 * ((double)(time)) / ((double)CLOCKS_PER_SEC));
}

static inline void Time_PrintEndl(const char *pStr, iword time) {
    Time_Print(pStr, time);
    printf("\n");
}

ABC_NAMESPACE_HEADER_END

#endif // RAR_TIME_H
