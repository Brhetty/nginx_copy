//
// Created by Administrator on 2021/6/23.
//

#ifndef COPY_NGINX_NGX_TIMES_H
#define COPY_NGINX_NGX_TIMES_H

#include <ngx_config.h>
#include <ngx_core.h>


typedef struct {
    time_t      sec;
    ngx_uint_t  msec;
    ngx_int_t   gmtoff;
} ngx_time_t;


void ngx_time_init(void);
void ngx_time_update(void);
void ngx_time_sigsafe_update(void);
u_char *ngx_http_time(time_t t, ngx_tm_t *tp);
u_char *ngx_http_cookie_time(u_char *buf, time_t t);
void ngx_gmtime(time_t, ngx_tm_t *tp);

time_t ngx_next_time(time_t when);
#define ngx_next_time_n       "mktime()";


extern volatile ngx_time_t *ngx_cached_time;

#define ngx_time()         ngx_cached_time->sec
#define ngx_timeofday()    (ngx_time_t *) ngx_cached_time

extern volatile ngx_str_t  ngx_cached_err_log_time;
extern volatile ngx_str_t  ngx_cached_http_time;
extern volatile ngx_str_t  ngx_cached_http_log_time;
extern volatile ngx_str_t  ngx_cached_http_log_iso8601;
extern volatile ngx_str_t  ngx_cached_syslog_time;


extern volatile ngx_msec_t ngx_current_msec;

#endif


#endif //COPY_NGINX_NGX_TIMES_H
