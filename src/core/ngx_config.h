#include <stdint.h>
#include <ngx_linux_config.h>

#define ngx_cdecl

/* TODO: auto_conf: ngx_inline   inline __inline __inline__ */
#ifndef ngx_inline
#define ngx_inline      inline
#endif


typedef intptr_t        ngx_int_t;
typedef uintptr_t       ngx_uint_t;
typedef intptr_t        ngx_flag_t;

#if (NGX_FREEBSD)
#include <ngx_freebsd_config.h>



#elif (NGX_LUNUX)
#include <ngx_linux_config.h>


#else
#include <ngx_posix_config.h>

#endif