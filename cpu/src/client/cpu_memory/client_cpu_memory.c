#include "client_cpu_memory.h"

int client_cpu_memory(t_log *logger)
{
    char *ip_memoria;
    char *puerto_memoria;
    t_config *config = NULL;

    config = iniciar_config(logger, "cpu.config");
    if (config == NULL)
    {
        log_error(logger, "No se pudo iniciar la configuración.");
        return EXIT_FAILURE;
    }
    log_info(logger, "🛠️ Configuración del cliente instanciada exitosamente.");

    ip_memoria = config_get_string_value(config, "IP_MEMORIA");
    puerto_memoria = config_get_string_value(config, "PUERTO_MEMORIA");

    int conexion_cpu_memoria = create_connection(logger, puerto_memoria, ip_memoria);

    return conexion_cpu_memoria;
}