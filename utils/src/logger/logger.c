#include "logger.h"

const char *color_azul = "\033[34m";
const char *color_verde = "\033[32m";
const char *reset_color = "\033[0m";
const char *color_rojo = "\033[31m";

/**
 * @brief Inicializa un logger para registrar eventos en el archivo "kernel.log".
 *
 * Esta función crea un logger que se usará en el núcleo del sistema. Si no se
 * puede crear el logger, se imprime un mensaje de error y el programa termina.
 *
 * @return t_log* Puntero al nuevo logger creado. Si no se puede crear, se termina el programa.
 */
t_log *iniciar_logger(char *file, char *name)
{
    // Intentamos crear un nuevo logger con el archivo "kernel.log", un nombre de
    // instancia "KERNEL", con el flag para sobreescribir el archivo, y el nivel
    // de loggeado establecido en LOG_LEVEL_INFO.
    t_log *nuevo_logger = log_create(file, name, 1, LOG_LEVEL_INFO);

    // Verificamos si la creación del logger falló (si el puntero es NULL).
    if (nuevo_logger == NULL)
    {
        // Si hubo un error, imprimimos un mensaje y terminamos el programa.
        custorm_log_error("No se pudo instanciar el nuevo_logger. (Return NULL)");
        exit(EXIT_FAILURE);
    }

    // Si todo salió bien, devolvemos el puntero al nuevo logger.
    return nuevo_logger;
}

void iterator(char *value)
{
    log_info(logger, "%s", value);
}

void cerrar_programa(t_log *logger)
{
    log_destroy(logger);
}

void custorm_log_error(const char *mensaje)
{
    fprintf(stderr, "%s ✖️ [CUSTOM ERROR]:%s %s\n", color_rojo, reset_color, mensaje);
}