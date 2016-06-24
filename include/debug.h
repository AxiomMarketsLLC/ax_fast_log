#ifndef __DEBUG__
#define __DEBUG__

#define DEBUG
#if defined(DEBUG)

#define DBG(fmt, args...) fprintf(stdout, "axlogger: %s:%d:%s(): \r\n" fmt, \
    __FILE__, __LINE__, __func__, ##args)
#else
 #define DBG(fmt, args...) /* Don't do anything in release builds */
#endif
#endif
