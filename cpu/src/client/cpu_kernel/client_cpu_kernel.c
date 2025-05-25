#include "client_cpu_kernel.h"

int client_cpu_kernel(t_log *logger)
{
    char *ip_kernel;
    char *purto_kernel;
    t_config *config = NULL;

    config = iniciar_config(logger, "cpu.config");
    if (config == NULL)
    {
        log_error(logger, "No se pudo iniciar la configuración.");
        return EXIT_FAILURE;
    }
    log_info(logger, "🛠️ Configuración del cliente instanciada exitosamente.");

    ip_kernel = config_get_string_value(config, "IP_KERNEL");
    purto_kernel = config_get_string_value(config, "PUERTO_KERNEL");

    int conexion_cpu_kernel = create_connection(logger, purto_kernel, ip_kernel);

    return conexion_cpu_kernel;
}