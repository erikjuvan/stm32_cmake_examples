#ifndef _CONSTANTS_COMMON_H_
#define _CONSTANTS_COMMON_H_


#define SAFETY_DRAIN_TIME_MS                    180000U  // Drain time after which there is a safe window of time to unlock door
#define SAFE_WINDOW_AFTER_DRAIN_TIME_MS         20000U   // Safe window of time after safety drain in which it is safe to unlock door
#define USER_DRAIN_RESERVE_MS                   4000U    // Drain time reserve (due to potential timing difference)

#endif
