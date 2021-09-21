#ifndef COPY_NGINX_CYCLE_H_INCLUDE
#define COPY_NGINX_CYCLE_H_INCLUDE


#include <ngx_config.h>
#include <ngx_core.h>

struct ngx_cycle_s {
    void          ****conf_ctx;
    ngx_pool         *pool;

    ngx_log_t        *log;
    ngx_log_t         new_log;

    ngx_uint_t        log_use_stderr;

    ngx_connection_t **files;
    ngx_connection_t *free_connections;
    ngx_uint_t        free_connection_n;

    ngx_module_t    **modules;
    ngx_uint_t        modules_n;
    ngx_uint_t        modules_used;

    ngx_queue_t       reusable_connection_queue;
    ngx_uint_t        reusable_connection_n;
    time_t            connections_resue_time;

    ngx_array_t       listening;
    ngx_array_t       paths;

    ngx_array_t       congfig_dump;
    ngx_rbtree_t      config_dump_retree;
    ngx_rbtree_node_t config_dump_sentinel;

    ngx_list_t        open_files;
    ngx_list_t        shared_memory;

    ngx_uint_t        connection_n;
    ngx_uint_t        files_n;

    ngx_connection_t *connections;
    ngx_event_t      *read_events;
    ngx_event_t      *write_events;

    ngx_cycle_t      *old_cycle;

    ngx_str_t        conf_file;
    ngx_str_t        conf_param;
    ngx_str_t        conf_prefix;
    ngx_str_t        prefix;
    ngx_str_t        error_log;
    ngx_str_t        lock_file;
    ngx_str_t        hostname; 
};


#endif