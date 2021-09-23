#include <ngx_config.h>
#include <ngx_core.h>

#define NGX_MAX_DYNAMIC_MODULES 128


ngx_int_t
ngx_preinit_modules(void) 
{
    ngx_uint_t i;

    for (i = 0; ngx_modules[i]; i++)
    {
        ngx_modules[i]->index = i;
        ngx_modules[i]->name = ngx_module_names[i];
    }

    ngx_modules_n = i;
    ngx_max_module = ngx_modules_n + NGX_MAX_DYNAMIC_MODULES；

    return NGX_OK；
}