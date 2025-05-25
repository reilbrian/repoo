#include "conexiones.h"

/**
 * @brief Crea una conexión con un servidor TCP.
 *
 * Esta función establece una conexión con el servidor especificado
 * usando la dirección IP y el puerto proporcionado.
 * Se utiliza el logger para registrar los errores y el estado de la conexión.
 *
 * @param logger Logger para registrar mensajes e información de errores.
 * @param ip Dirección IP o hostname del servidor.
 * @param puerto Puerto al cual conectar.
 * @return int Descriptor del socket, o -1 si hubo un error.
 */
int create_connection(t_log *logger, char *port, char *ip)
{
    // Paso 1: Validación de los campos
    if (ip == NULL || strlen(ip) == 0)
    {
        log_error(logger, "La IP es inválida o vacía.");
        return -1;
    }

    if (port == NULL || strlen(port) == 0)
    {
        log_error(logger, "El puerto es inválido o vacío.");
        return -1;
    }

    // Paso 2: Configurar la guia del getaddrinfo
    struct addrinfo hints, *serverinfo;

    // Inicializando hints
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    int resultado = getaddrinfo(ip, port, &hints, &serverinfo);
    if (resultado != 0)
    {
        log_error(logger, "getaddrinfo falló: %s", gai_strerror(resultado));
        return -1;
    }

    // Creamos el socket
    int socket_cliente = socket(serverinfo->ai_family, serverinfo->ai_socktype, serverinfo->ai_protocol);
    if (socket_cliente == -1)
    {
        log_error(logger, "No se pudo crear el socket");
        freeaddrinfo(serverinfo);
        return -1;
    }

    // Creamos la connección
    if (connect(socket_cliente, serverinfo->ai_addr, serverinfo->ai_addrlen) == -1)
    {
        log_error(logger, "✖️ [UTILS ERROR]: No se pudo conectar al servidor.");
        close(socket_cliente);    // Cerramos el socket si no se pudo conectar
        freeaddrinfo(serverinfo); // Liberamos la memoria de `serverinfo`
        return -1;
    }

    // Liberamos los recursos
    freeaddrinfo(serverinfo); // Liberamos los recursos de `getaddrinfo`
    log_info(logger, "🔗 Conectado exitosamente a la IP: %s. [%s]", ip, port);
    return socket_cliente;
}

/**
 * @brief Cierra y destruye una conexión de socket.
 *
 * Esta función cierra la conexión de socket indicada por el descriptor de socket
 * proporcionado y establece el descriptor a -1.
 *
 * @param socket_fd Puntero al descriptor de socket a cerrar.
 *                  Se asignará a -1 luego de cerrar.
 */
void destroy_connection(int *socket_fd)
{
    if (socket_fd == NULL || *socket_fd < 0)
        return;

    close(*socket_fd);
    *socket_fd = -1;
}

/**
 * @brief Espera y acepta una conexión entrante en el socket del servidor.
 *
 * Esta función bloquea el programa hasta que un cliente intente conectarse al servidor.
 * Al aceptar una conexión, se devuelve un descriptor de socket para la comunicación
 * con el cliente.
 *
 * @param logger Logger para registrar mensajes.
 * @param socket_servidor Descriptor del socket servidor que escucha conexiones.
 * @return int Descriptor del socket del cliente aceptado, o -1 si falló.
 */
int wait_custommer(t_log *logger, int socket_servidor)
{
    struct sockaddr_in dir_cliente;
    socklen_t tam_direccion = sizeof(struct sockaddr_in);

    int socket_cliente = accept(socket_servidor, (void *)&dir_cliente, &tam_direccion);

    return socket_cliente;
}

int listen_server(t_log *logger, int connection, char *module)
{
    int client = wait_custommer(logger, connection);
    if (client != -1)
    {
        log_trace(logger, " ✔️ [UTILS]: El modulo %s se conectó exitosamente.", module);
    }
    return client; // ← ¡acá está la clave!
}

int start_server(t_log *logger, char *ip, char *puerto)
{
    int socket_servidor;
    struct addrinfo hints, *servinfo, *p;
    int resultado;

    // Inicializando hints para la búsqueda de direcciones
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;     // IPv4 o IPv6
    hints.ai_socktype = SOCK_STREAM; // Socket TCP
    hints.ai_flags = AI_PASSIVE;     // Usar la dirección del host local

    // Obtener información de la dirección
    resultado = getaddrinfo(ip, puerto, &hints, &servinfo);
    if (resultado != 0)
    {
        log_error(logger, "getaddrinfo falló: %s", gai_strerror(resultado));
        return -1; // Error al obtener las direcciones
    }

    bool conecto = false;

    // Intentar conectar con cada dirección devuelta por getaddrinfo
    for (p = servinfo; p != NULL; p = p->ai_next)
    {
        // Crear el socket
        socket_servidor = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (socket_servidor == -1)
        {
            log_error(logger, "Error al crear el socket");
            continue; // Intentar con el siguiente p
        }

        int opt = 1;
        setsockopt(socket_servidor, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

        // Intentar bind
        if (bind(socket_servidor, p->ai_addr, p->ai_addrlen) == -1)
        {
            log_error(logger, "Error al hacer bind");
            close(socket_servidor);
            continue; // Intentar con el siguiente p
        }

        // Si el bind tiene éxito, salir del ciclo
        conecto = true;
        break;
    }

    // Si no se logró conectar con ninguna de las direcciones
    if (!conecto)
    {
        log_error(logger, "No se pudo enlazar el socket a ninguna dirección.");
        freeaddrinfo(servinfo);
        return -1;
    }

    // Escuchar conexiones entrantes (hasta SOMAXCONN conexiones simultáneas)
    if (listen(socket_servidor, SOMAXCONN) == -1)
    {
        log_error(logger, "Error al escuchar en el socket");
        close(socket_servidor);
        freeaddrinfo(servinfo);
        return -1;
    }

    // Obtener el nombre de la dirección para el log
    char host[NI_MAXHOST];
    if (getnameinfo(servinfo->ai_addr, servinfo->ai_addrlen, host, sizeof(host), NULL, 0, NI_NUMERICHOST) != 0)
    {
        strcpy(host, "Desconocido");
    }

    // Avisar al logger que el servidor está escuchando
    log_info(logger, " ✔️ Escuchando la IP %s (%s)...", ip, puerto);

    // Liberar recursos de addrinfo
    freeaddrinfo(servinfo);

    // Devolver el socket del servidor
    return socket_servidor;
};