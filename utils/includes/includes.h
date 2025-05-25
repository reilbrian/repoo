#ifndef UTILS_H_
#define UTILS_H_

/* ─────────────────────────────────────────────────────────── */
/*                         INCLUDES                            */
/* ─────────────────────────────────────────────────────────── */

#include <assert.h>
#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

/* SO: COMMONS -> Doc: https://github.com/sisoputnfrba/so-commons-library */
#include <commons/collections/list.h>
#include <commons/config.h>
#include <commons/log.h>
#include <commons/string.h>
#include <readline/history.h>
#include <readline/readline.h>

/* ─────────────────────────────────────────────────────────── */
/*                        DEFINICIONES                         */
/* ─────────────────────────────────────────────────────────── */

#define PUERTO "4444" // Puerto en el que se va a levantar el servidor
#define CLOCK 1000000 // 1 segundo

/* ─────────────────────────────────────────────────────────── */
/*                         TIPOS                               */
/* ─────────────────────────────────────────────────────────── */

/**
 * op_code: identifica el tipo de mensaje que se recibe.
 * MENSAJE: un string simple.
 * PAQUETE: lista de strings empaquetados.
 */
typedef enum
{
    MENSAJE,
    PAQUETE
} op_code;

typedef struct
{
    int size;
    void *stream;
} t_buffer;

typedef struct
{
    op_code codigo_operacion;
    t_buffer *buffer;
} t_paquete;

/* ─────────────────────────────────────────────────────────── */
/*                VARIABLES GLOBALES EXTERNAS                  */
/* ─────────────────────────────────────────────────────────── */

extern t_log *logger; // Logger global usado en todo el proyecto
void iterator(char *value);
extern const char *color_azul;
extern const char *color_verde;
extern const char *color_rojo;
extern const char *reset_color;

/* ─────────────────────────────────────────────────────────── */
/*                        LOGGERS                              */
/* ─────────────────────────────────────────────────────────── */

t_log *iniciar_logger(char *file, char *name);
void custorm_log_error(const char *mensaje);
void iterator(char *value);
void cerrar_programa(t_log *logger);

/* ─────────────────────────────────────────────────────────── */
/*                    CONFIGURACIONES                          */
/* ─────────────────────────────────────────────────────────── */

t_config *iniciar_config(t_log *logger, char *config_file);

/* ─────────────────────────────────────────────────────────── */
/*                    FUNCIONES DE CONSOLA                     */
/* ─────────────────────────────────────────────────────────── */

void leer_consola(t_log *logger);
void paquete(int);
void terminar_programa(int conexion, t_log *logger, t_config *config);

/* ─────────────────────────────────────────────────────────── */
/*                    FUNCIONES DE SOCKET                      */
/* ─────────────────────────────────────────────────────────── */

int iniciar_servidor(void);
int recibir_operacion(int socket_cliente);
void *recibir_buffer(int *size, int socket_cliente);

/* ─────────────────────────────────────────────────────────── */
/*                  FUNCIONES DE COMUNICACIÓN                  */
/* ─────────────────────────────────────────────────────────── */

void enviar_mensaje(char *mensaje, int socket_cliente);
char *recibir_mensaje(int socket_cliente);
t_list *recibir_paquete(int socket_cliente);

/* ─────────────────────────────────────────────────────────── */
/*                        PAQUETES                             */
/* ─────────────────────────────────────────────────────────── */

t_paquete *crear_paquete(void);
void agregar_a_paquete(t_paquete *paquete, void *valor, int tamanio);
void crear_buffer(t_paquete *paquete);
void *serializar_paquete(t_paquete *paquete, int bytes);
void enviar_paquete(t_paquete *paquete, int socket_cliente);
void eliminar_paquete(t_paquete *paquete);

/* ─────────────────────────────────────────────────────────── */
/*                    CONEXIONES                               */
/* ─────────────────────────────────────────────────────────── */

int create_connection(t_log *logger, char *ip, char *puerto);
void destroy_connection(int *socket_fd);
int listen_server(t_log *logger, int connection, char *module);
int start_server(t_log *logger, char *ip, char *puerto);
int wait_custommer(t_log *logger, int socket_servidor);

#endif /* UTILS_H_ */