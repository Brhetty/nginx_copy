#include <ngx_config.h>
#include <ngx_string.h>
#include <ngx_errno.h>
#include <ngx_times.h>
#include <ngx_palloc.h>

typedef struct ngx_pool_s            ngx_pool_t;

#define  NGX_OK          0
#define  NGX_ERROR      -1

#define ngx_max(val1, val2)  ((val1 < val2) ? (val2) : (val1))
#define ngx_min(val1, val2)  ((val1 > val2) ? (val2) : (val1))