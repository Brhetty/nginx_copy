
#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_event.h>


static ngx_command_t ngx_events_commands[] = {
	{ ngx_string("events"),
      NGX_MAIN_CONF|NGX_CONF_BLOCK|NGX_CONF_NOARGS,
      ngx_events_block,
      0,
      0,
      NULL },

      ngx_null_command
};


static ngx_core_module_t ngx_events_module_ctx = {
	ngx_string("events"),
	NULL,
	ngx_event_init_conf
};

ngx_module_t ngx_events_module = {
    NGX_MODULE_V1,
    &ngx_events_module_ctx,                /* module context */
    ngx_events_commands,                   /* module directives */
    NGX_CORE_MODULE,                       /* module type */
    NULL,                                  /* init master */
    NULL,                                  /* init module */
    NULL,                                  /* init process */
    NULL,                                  /* init thread */
    NULL,                                  /* exit thread */
    NULL,                                  /* exit process */
    NULL,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


static char *
ngx_events_block(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
	char              *rv;
	void            ***ctx;
	ngx_uint_t         i;
	ngx_conf_t         pcf;
	ngx_event_module_t *m;

	if (*(void **) conf) {
		return "is duplicate";
	}

}
