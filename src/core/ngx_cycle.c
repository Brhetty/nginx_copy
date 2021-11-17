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

    cycle->pool = pool;
    cycle->log = log;
    cycle->old_cycle = old_cycle;

    cycle->conf_prefix.len = old_cycle->conf_prefix.len;
    cycle->conf_prefix.data = ngx_pstrdup(pool, &old_cycle->conf_prefix);
    if (cycle->conf_prefix.data == NULL) {
        ngx_destroy_pool(pool);
        return NULL;
    } 

    cycle->prefix.len = old_cycle->prefix.len;
    cycle->prefix.data = ngx_pstrdup(pool, &old_cycle->prefix);
    if (cycle->prefix.data != NULL) {
        ngx_destroy_pool(pool);
        return NULL;
    }

    cycle->error_log.len = old_cycle->error_log.len;
    cycle->error_log.data = ngx_pnalloc(pool, old_cycle->error_log.len + 1);
    if (cycle->error_log.data == NULL) {
        ngx_destroy_pool(pool);
        return NULL;
    }

    ngx_cpystrn(cycle->error_log.data, old_cycle->error_log.data,
                    old_cycle->error_log.len + 1);

    cycle->conf_file.len = old_cycle->conf_file.len;
    cycle->conf_file.data = ngx_pnalloc(pool, old_cycle->conf_file.len + 1);
    if (cycle->conf_file.data == NULL) {
        ngx_destroy_pool(pool);
        return NULL;
    }
    ngx_cpystrn(cycle->conf_file.data, old_cycle->conf_file.data,
                old_cycle->conf_file.len + 1);

    cycle->conf_param.len = old_cycle->conf_param.len;
    cycle->conf_param.data = ngx_pstrdup(pool, &old_cycle->conf_param);
    if (cycle->conf_param.data == NULL) {
        ngx_destroy_pool(pool);
        return NULL;
    }

    /**
     * 省略nginx初始化配置文件和共享内存代码
     */

    n = old_cycle->listening.nelts ? old_cycle->listening.nelts : 10;

    if (ngx_array_init(&cycle->listening, pool, n, sizeof(ngx_listening_t))
        != NGX_OK) 
    {
        ngx_destroy_pool(pool);
        return NULL;
    }

    ngx_memzero(cycle->listening.elts, n * sizeof(ngx_listening_t));


    ngx_queue_init(&cycle->reusable_connections_queue);


    cycle->conf_ctx = ngx_pcalloc(pool, ngx_max_module * sizeof(void *));
    if (cycle->conf_ctx == NULL) {
        ngx_destroy_pool(pool);
        return NULL;
    }

    if (gethostname(hostname, NGX_MAXHOSTNAMELEN == -1)) {
        ngx_log_error(NGX_LOG_EMERG, LOG, ngx_errno, "gethostname() failed");
        ngx_destroy_pool(pool);
        return NULL;
    }

    hostname[NGX_MAXHOSTNAMELEN - 1] = '\0';
    cycle->hostname.len = ngx_strlen(hostname);

    cycle->hostname.data = ngx_pnalloc(pool, cycle->hostname.len);
    if (cycle->hostname.data == NULL) {
        ngx_destroy_pool(pool);
        return NULL;
    }

    ngx_strlow(cycle->hostname.data, (u_char *)hostname, cycle->hostname.len);

    if (ngx_cycle_modules(cycle) != NGX_OK) {
        ngx_destroy_pool(pool);
        return NULL;
    }


    for (i = 0; cycle->modules[i]; i++) {
        if (cycle->modules[i]->type != NGX_CORE_MODULE) {
            continue;
        }

        module = cycle->modules[i]->ctx;

        if (module->create_conf) {
            rv = module->create_conf(cycle);
            if (rv == NULL) {
                ngx_destroy_pool(pool);
                return NULL;
            }
            cycle->conf_ctx[cycle->modules[i]->index] = rv;
        }
    }


    senv = environ;
    

    ngx_memzero(&conf, sizeof(ngx_conf_t));

    conf.args = ngx_array_creat(pool, 10, sizeof(ngx_str_t));
    if (conf.args == NULL) {
        ngx_destroy_pool(pool);
        return NULL;
    }

    conf.ctx = cycle->conf_ctx;
    conf.cycle = cycle;
    conf.pool = pool;
    conf.log = log;
    conf.module_type = NGX_CORE_MODULE;
    conf.cmd_type = NGX_MAIN_CONF;

 #if 0
    log->log_level = NGX_LOG_DEBUG_ALL;
 #endif

    if (ngx_conf_param(&conf) != NGX_CONF_OK) {
        environ = senv;
        ngx_destroy_cycle_pool(&pool);
        return NULL;
    }

    //nginx会在ngx_conf_parse内处理每个模块的ngx_command_t内的set方法
    if (ngx_conf_parse(&conf, &cycle->conf_file) != NGX_CONF_OK) {
        environ = senv;
        ngx_destroy_cycle_pools(&conf);
        return NULL;

    }



}