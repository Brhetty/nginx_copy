#include <ngx_config.h>
#include <ngx_string.h>
#include <ngx_errno.h>
#include <ngx_times.h>
#include <ngx_palloc.h>
#include <ngx_log.h>
#include <ngx_cycle.h>

typedef struct ngx_pool_s            ngx_pool_t;
typedef struct ngx_cycle_s           ngx_cycle_t;

typedef void (*ngx_connection_handler_pt)(ngx_connection_t *c);

#define  NGX_OK          0
#define  NGX_ERROR      -1

#define ngx_max(val1, val2)  ((val1 < val2) ? (val2) : (val1))
#define ngx_min(val1, val2)  ((val1 > val2) ? (val2) : (val1))