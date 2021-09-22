#include <ngx_core.h>
#include <ngx_config.h>
#include <nginx.h>
#include <sys/types.h>

static void ngx_show_version_info(void);
static ngx_int_t ngx_get_options(int argc, char *const *argv);


static ngx_uint_t   ngx_show_help;
static ngx_uint_t   ngx_show_version;
static ngx_uint_t   ngx_show_configure;
static u_char      *ngx_prefix;
static u_char      *ngx_error_log;
static u_char      *ngx_conf_file;
static u_char      *ngx_conf_params;
static char        *ngx_signal;

int ngx_cdecl
main(int argc, char *const *argv)
{
    if (ngx_strerror_init() != NGX_OK) {
        return 1;
    }

    if (ngx_get_options(argc, argv) != NGX_OK) {
        return 1;
    }

    if (ngx_show_version) {
        ngx_show_version_info();

        if (!ngx_test_config) {
            return 0;
        }
    }

    /* TODO */ ngx_max_sockets = -1;

    ngx_time_init();
#if (NGX_PCRE)
    ngx_regex_init();
#endif
    ngx_pid = ngx_getpid();
    ngx_parent = ngx_getppid();

    log = ngx_log_init(ngx_prefix, ngx_error_log);
    if (log == NULL) {
        return 1;
    }

#if (NGX_OPENSSL)
    ngx_ssl_init(log);
#endif

    ngx_memzero(&init_cycle, sizeof(ngx_cycle_t));
    init_cycle.log = log;
    ngx_cycle = &init_cycle;

    init_cycle.pool = ngx_create_pool(1024, log);
    if (init_cycle.pool == NULL) {
        return 1;
    }

    if (ngx_save_argv(&init_cycle, argc, argv) != NGX_OK) {
        return 1;
    }

    if (ngx_process_options(&init_cycle) != NGX_OK) {
        return 1;
    }

    if (ngx_os_init(log) != NGX_OK) {
        return 1;
    }

    if (ngx_crc32_table_init() != NGX_OK) {
        return 1;
    }

    ngx_slab_sizes_init();

    if (ngx_add_inherited_sockets(&init_cycle) != NGX_OK) {
        return 1;
    }

    
}



static ngx_int_t
ngx_get_options(int argc, char *const *argv)
{
    u_char     *p;
    ngx_int_t   i;

    for (i = 1; i < argc; i++) {
        p = (u_char *) argv[i];

        if (*p++ != '-') {
            ngx_log_stderr(0, "invalid option: \"%s\"", argv[i]);
            return NGX_ERROR;

        }

        while (*p) {

            switch (*p++) {
                case '?':
                case 'h':
                    ngx_show_version = 1;
                    ngx_show_help = 1;
                    break;;

                case 'v':
                    ngx_show_version = 1;
                    ngx_show_configure = 1;
                    break;

                case 't':
                    ngx_test_config = 1;
                    break;

                case 'T':
                    ngx_test_config = 1;
                    ngx_dump_config = 1;
                    break;

                case 'q':
                    ngx_quiet_mode = 1;
                    break;

                case 'p':
                    if (*p) {
                        ngx_prefix = p;
                        goto next;
                    }

                    if (argv[++i]) {
                        ngx_prefix = (u_char *) argv[i];
                        goto next;
                    }

                    ngx_log_stderr(0, "option \"-p\" requires directory name");
                    return  NGX_ERROR;

                case 'e':
                    if (*p) {
                        ngx_error_log = p;

                    } else if (argv[++i]) {
                        ngx_error_log = (u_char *) argv[i];

                    } else {
                        ngx_log_stderr();
                        return NGX_ERROR;
                    }

                    if (ngx_strcmp(ngx_error_log, "stderr") == 0) {
                        ngx_error_log = (u_char *) "";
                    }

                    goto next;
                case 'c':
                    if (*p) {
                        ngx_conf_file = p;
                        goto  next;
                    }

                    if (argv[++i]) {
                        ngx_conf_file = (u_char *) argv[i];
                        goto next;
                    }

                    ngx_log_stderr(0, "option \"-c\" requires file name");
                    return NGX_ERROR;

                case 'g':
                    if (*p) {
                        ngx_conf_params = p;
                        goto next;
                    }

                    if (argv[++i]) {
                        ngx_conf_params = (u_char *) argv[i];
                        goto next;
                    }

                    ngx_log_stderr(0, "option \"-g\" requires parameter");
                    return NGX_ERROR;

                case 's':
                    if (*p) {
                        ngx_signal = (char *) p;

                    } else if (argv[++i]) {
                        ngx_signal = argv[i];

                    } else {
                        ngx_log_stderr(0, "option \"-s\" requires parameter");
                        return NGX_ERROR;
                    }

                    if (ngx_strcmp(ngx_signal, "stop") == 0
                        || ngx_strcmp(ngx_signal, "quit") == 0
                        || ngx_strcmp(ngx_signal, "reopen") == 0
                        || ngx_strcmp(ngx_signal, "reload") == 0)
                    {
                        ngx_process = NGX_PROCESS_SIGNALLER;
                        goto next;
                    }

                    ngx_log_stderr(0, "invalid option: \"-s %s\"", ngx_signal);
                    return NGX_ERROR;

                default:
                    ngx_log_stderr(0, "invalid option: \"%c\"", *(p - 1));
                    return NGX_ERROR;
            }
        }

        next:

           continue;
    }

    return NGX_OK;
}


static void
ngx_show_version_info(void)
{
    ngx_write_stderr("nginx version: " NGINX_VER_BUILD NGX_LINEFEED);

    if (ngx_show_help) {
        ngx_write_stderr(
                "Usage: nginx [-?hvVtTq] [-s signal] [-p prefix]" NGX_LINEFEED
        "             [-e filename] [-c filename] [-g directives]"
        NGX_LINEFEED NGX_LINEFEED
        "Options:" NGX_LINEFEED
        "  -?,-h         : this help" NGX_LINEFEED
        "  -v            : show version and exit" NGX_LINEFEED
        "  -V            : show version and configure options then exit"
        NGX_LINEFEED
        "  -t            : test configuration and exit" NGX_LINEFEED
        "  -T            : test configuration, dump it and exit"
        NGX_LINEFEED
        "  -q            : suppress non-error messages "
        "during configuration testing" NGX_LINEFEED
        "  -s signal     : send signal to a master process: "
        "stop, quit, reopen, reload" NGX_LINEFEED
#ifdef NGX_PREFIX
        "  -p prefix     : set prefix path (default: " NGX_PREFIX ")"
                               NGX_LINEFEED
#else
        "  -p prefix     : set prefix path (default: NONE)" NGX_LINEFEED
#endif
        "  -e filename   : set error log file (default: "
#ifdef NGX_ERROR_LOG_STDERR
        "stderr)" NGX_LINEFEED
#else
        NGX_ERROR_LOG_PATH ")" NGX_LINEFEED
#endif
        "  -c filename   : set configuration file (default: " NGX_CONF_PATH
        ")" NGX_LINEFEED
        "  -g directives : set global directives out of configuration "
        "file" NGX_LINEFEED NGX_LINEFEED
        );
    }

    if (ngx_show_configure) {

#ifdef NGX_COMPILER
        ngx_write_stderr("built by " NGX_COMPILER NGX_LINEFEED);
#endif

#if (NGX_SSL)
        if (ngx_strcmp(ngx_ssl_version(), OPENSSL_VERSION_TEXT) == 0) {
            ngx_write_stderr("built with " OPENSSL_VERSION_TEXT NGX_LINEFEED);
        } else {
            ngx_write_stderr("built with " OPENSSL_VERSION_TEXT
                             " (running with ");
            ngx_write_stderr((char *) (uintptr_t) ngx_ssl_version());
            ngx_write_stderr(")" NGX_LINEFEED);
        }
#ifdef SSL_CTRL_SET_TLSEXT_HOSTNAME
        ngx_write_stderr("TLS SNI support enabled" NGX_LINEFEED);
#else
        ngx_write_stderr("TLS SNI support disabled" NGX_LINEFEED);
#endif
#endif

        ngx_write_stderr("configure arguments:" NGX_CONFIGURE NGX_LINEFEED);
    }
}


static ngx_int_t
ngx_process_options(ngx_cycle_t *cycle) 
{
    u_char *p;
    size_t  len;

    if (ngx_prefix) {
        len = ngx_strlen(ngx_prefix);
        p = ngx_prefix;

        if (len && !ngx_path_separator(p[len - 1])) {
            p = ngx_pnalloc(cycle->pool, len + 1);
            if (p == NULL) {
                return NGX_ERROR;
            }

            ngx_memcpy(p, ngx_prefix, len);
            p[len++] = '/';
        }

        cycle->conf_prefix.len = len;
        cycle->conf_prefix.data = p;
        cycle->prefix.len = len;
        cycle->prefix.data = p;
    } else {

#ifndef NGX_PREFIX
        P = ngx_pnalloc(cycle->pool, NGX_MAX_PATH);
        if (p == NULL) {
            return NGX_ERROR;
        }

        if (ngx_getcwd(p, NGX_MAX_PATH) == 0) {
            ngx_log_stderr(ngx_errno,  "[emerg]: " ngx_getcwd_n " failed");
            return NGX_ERROR;
        }

        len = ngx_strlen(p);

        p[len++] = '/';

        cycle->conf_prefix.len = len;
        cycle->conf_prefix.data = p;
        cycle->prefix.len = len;
        cycle->prefix.data = p;

#else

#ifdef NGX_CONF_PREFIX
        ngx_str_set(&cycle->conf_prefix, NGX_CONF_PREFIX);
#else
        ngx_str_set(&cycle->conf_prefix, NGX_PREFIX);
#endif
        ngx_str_set(&cycle->prefix, NGX_PREFIX);      

#endif
    }

        if (ngx_conf_file) {
        cycle->conf_file.len = ngx_strlen(ngx_conf_file);
        cycle->conf_file.data = ngx_conf_file;

    } else {
        ngx_str_set(&cycle->conf_file, NGX_CONF_PATH);
    }

    if (ngx_conf_full_name(cycle, &cycle->conf_file, 0) != NGX_OK) {
        return NGX_ERROR;
    }

    for (p = cycle->conf_file.data + cycle->conf_file.len - 1;
         p > cycle->conf_file.data;
         p--)
    {
        if (ngx_path_separator(*p)) {
            cycle->conf_prefix.len = p - cycle->conf_file.data + 1;
            cycle->conf_prefix.data = cycle->conf_file.data;
            break;
        }
    }

    if (ngx_error_log) {
        cycle->error_log.len = ngx_strlen(ngx_error_log);
        cycle->error_log.data = ngx_error_log;

    } else {
        ngx_str_set(&cycle->error_log, NGX_ERROR_LOG_PATH);
    }

    if (ngx_conf_params) {
        cycle->conf_param.len = ngx_strlen(ngx_conf_params);
        cycle->conf_param.data = ngx_conf_params;
    }

    if (ngx_test_config) {
        cycle->log->log_level = NGX_LOG_INFO;
    }

    return NGX_OK;
}

static ngx_int_t
ngx_add_inherited_sockets(ngx_cycle_t *cycle)
{
    u_char *p, *v, *inherited;
    ngx_int_t s;
    ngx_listening_t *ls;

    inherited = (u_char *) getenv(NGINX_VAR);

    if (inherited == NULL) {
        return NGX_OK;
    }

}