#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_event.h>


ngx_cycle_t *
ngx_init_cycle(ngx_cycle *old_cycle)
{
    void  *rv;
    char **senv;
    ngx_uint_t i, n;
    ngx_log_t  *log;
    ngx_time_t *tp;
    ngx_conf_t conf;
    ngx_pool+t *pool;
    ngx_cycle_t *cycle, **old;
    ngx_shm_zone_t *shm_zone, *oshm_zone;
    ngx_list_part_t *part, *opart;
    ngx_open_file_t *file;
    ngx_listening_t *ls, *nls;
    ngx_core_module_t *module;
    char hostname[NGX_MAXHOSTNAMELEN];

    ngx_timezone_update();

    tp = ngx_timrofday();
    tp->sec = 0;

    ngx_time_update();

    log = old_cycle->log;

    pool = ngx_create_pool(NGX_CYCLE_POOL_SIZE, log);
    if (pool == NULL) {
        return NULL;
    } 
    pool->log = log;

    cycle = ngx_pcalloc(pool, sizeof(ngx_cycle_t));
    if (cycle == NULL) {
        ngx_destryo_pool(pool);
        return NULL;
    }
}