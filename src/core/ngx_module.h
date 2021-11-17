#include <ngx_config.h>
#include <ngx_core.h>
#include <nginx.h>

extern ngx_module_t *ngx_modules[];
extern ngx_uint_t ngx_max_module;

extern char *ngx_module_names[];

ngx_int_t ngx_preinit_modules(void);
ngx_int_t ngx_cycle_modules(ngx_cycle_t *cycle);