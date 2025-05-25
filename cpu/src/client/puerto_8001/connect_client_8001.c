#include "../client.h"

// cpu -> KERNEL
int connect_client_8001(t_log *logger)
{
    char *ip;
    char *puerto;
    t_config *config = NULL;

    config = iniciar_config(logger, "cpu.config");
    if (config == NULL)
    {
        log_error(logger, "No se pudo iniciar la configuración.");
        return EXIT_FAILURE;
    }
    log_info(logger, "🛠️ Configuración del cliente instanciada exitosamente.");

    ip = config_get_string_value(config, "IP_KERNEL");
    puerto = config_get_string_value(config, "PUERTO_KERNEL_DISPATCH");

    int conexion = create_connection(logger, puerto, ip);

    return conexion;
}