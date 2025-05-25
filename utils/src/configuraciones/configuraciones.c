#include "configuraciones.h"

/**
 * @brief Crea e inicializa un archivo de configuración.
 *
 * Esta función intenta cargar un archivo de configuración utilizando la ruta proporcionada.
 * Si no puede abrirse o el archivo no existe, se loguea un error y el programa finaliza.
 *
 * @param logger Logger utilizado para registrar errores.
 * @param config_file Ruta al archivo de configuración.
 * @return t_config* Puntero a la estructura de configuración cargada. Termina el programa si falla.
 */
t_config *iniciar_config(t_log *logger, char *config_file)
{
    t_config *nuevo_config;
    nuevo_config = config_create(config_file);

    if (nuevo_config == NULL)
    {
        log_error(logger, "Hubo un error al crear el archivo de configuración.\n");
        exit(EXIT_FAILURE);
    }

    return nuevo_config;
}
