#ifndef __DEBUG__
#define __DEBUG__

#if defined(DEBUG)

#define DBG(fmt, args...) fprintf(stdout, "\r\nax_lg: %s:%d:%s(): " fmt, \
    __FILE__, __LINE__, __func__, ##args)
#else
 #define DBG(fmt, args...) /* Don't do anything in release builds */
#endif
#endif
