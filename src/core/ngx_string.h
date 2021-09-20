#ifndef _NGX_STRING_H_INCLUDED_
#define _NGX_STRING_H_INCLUDED_


#include <ngx_config.h>
#include <ngx_core.h>


typedef struct {
    size_t len;
    u_char *data;
} ngx_str_t;


#define ngx_string(str)    { sizeof(str) - 1, (u_char *) str }
#define ngx_strlen(s)     strlen((const char *) s)

#define ngx_memzero(buf, n) (void) memset(buf, 0, n)

#define ngx_strcmp(s1, s2)  strcmp((const char *) s1, (const char *) s2)


#if (NGX_MEMCPY_LIMIT)

void    *ngx_memcpy(void *dst, const void *src, size_t n);
#define  ngx_cpymem(dst, src, n)    (((u_char *) ngx_memcpy(dst, src, n)) + (n))

#else


#define  ngx_memcpy(dst, src, n)    (void) memcpy(dst, src, n)
#define  ngx_cpymem(dst, src, n)    (((u_char *)memcpy(dst, src, n)) + (n))

#endif

