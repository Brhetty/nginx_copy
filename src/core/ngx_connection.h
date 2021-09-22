#ifndef COPY_NGINX_CONNECTION_H_INCLUDE
#define COPY_NGINX_CONNECTION_H_INCLUDE


#include <ngx_congfix.h>
#include <ngx_core.h>


typedef struct ngx_listening_s {
    ngx_socket_t fd;

    struct *sockaddr;
    socklen_t socklen;
    size_t addr_text_max_len;
    ngx_str_t addr_text;

    int type;

    int backlog;
    int rcvbuf;
    int sndbuf;
#if (NGX_HAVE_KEEPALIVE_TUNABLE)
    int keepidle;
    int keepintv1;
    int keepcnt;
#endif

    /* handler of accepted connection */
    ngx_connection_handler_pt handler;

    void *servers; /* array of ngx_http_in_addr_t, for example */

    ngx_log_t log;
    ngx_log_t *logp;

    size_t  pool_size;

    size_t post_accept_buffer_size;

    ngx_msec_t post_accept_timeout;

    ngx_listening_t *previous;
    ngx_connection_t *connection;

    ngx_rbtree_t rbtree;
    ngx_rbtree_node_t sentinel;

    ngx_uint_t worker;

    unsigned open:1;
    unsigned remain:1;
    unsigned ignore:1;

    unsigned bound:1;
    unsigned inherited:1;
    unsigned nonblocking_accept:1;
    unsigned listen:1;
    unsigned nonblocking:1;
    unsigned shared:1;
    unsigned addr_ntop;
    unsigned widcard:1;
#if (NGX_HAVE_INET6)
    unsigned ipv6only:1;
#endif    
    unsigned reuseport:1;
    unsigned add_reuseport;
    unsigned keepalive:2;

    unsigned deferred_accept:1;
    unsigned delete_deferred:1;
#if (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
    char               *accept_filter;
#endif
#if (NGX_HAVE_SETFIB)
    int                 setfib;
#endif

#if (NGX_HAVE_TCP_FASTOPEN)
    int                 fastopen;
#endif

};

#endif