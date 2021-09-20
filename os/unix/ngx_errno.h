#ifndef _NGX_ERRNO_H_INCLUDED_
#define _NGX_ERRNO_H_INCLUDED_


#include <ngx_config.h>
#include <ngx_core.h>


typedef int ngx_err_t


#define ngx_errno                  errno
#define ngx_socket_errno           errno
#define ngx_set_errno(err)         errno = err
#define ngx_set_socker_errno(err)  errno = err


u_char *ngx_strerror(ngx_err_t err, u_char *errstr, size_t size);
ngx_int_t ngx_strerror_init(void);


#endif
