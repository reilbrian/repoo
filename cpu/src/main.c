#include "main.h"

// Función que libera todos los recursos de CPU
void cleanup_cpu(int conexion_memoria, int conexion_kernel_dispatch, int conexion_kernel_interrupt, t_log *logger, char *mensaje)
{
	if (mensaje != NULL)
	{
		free(mensaje);
		log_warning(logger, "🗑️ Liberando mensaje de la cpu.");
	}
	if (conexion_memoria != -1)
	{
		destroy_connection(&conexion_memoria);
		log_warning(logger, "🗑️ Liberando conexion con la memoria.");
	}
	if (conexion_kernel_dispatch != -1)
	{
		destroy_connection(&conexion_kernel_dispatch);
		log_warning(logger, "🗑️ Liberando conexion con el kernel.");
	}
	if (conexion_kernel_interrupt != -1)
	{
		destroy_connection(&conexion_kernel_interrupt);
		log_warning(logger, "🗑️ Liberando conexion con el kernel - interrupt.");
	}
	if (logger != NULL)
	{
		log_warning(logger, "🗑️ Liberando el logger y terminando programa.");
		log_destroy(logger);
	}
}

int main(void)
{
	// Variables de conexión y configuración
	int conexion_memoria = -1;
	int conexion_kernel_dispatch = -1;
	int conexion_kernel_interrupt = -1;
	char *mensaje = NULL;
	int dispatch_vacio = 0, interrupt_vacio = 0;

	// Instanciar el logger
	t_log *logger = iniciar_logger("cpu.log", "CPU");
	if (logger == NULL)
	{
		fprintf(stderr, "No se pudo iniciar el logger.\n");
		return EXIT_FAILURE;
	}
	log_info(logger, "✅ Módulo CPU instanciado exitosamente.");

	// Conexión cpu ->  memoria
	conexion_memoria = connect_client_8002(logger);
	if (conexion_memoria == -1)
	{
		log_error(logger, "No se pudo conectar con la memoria.");
		cleanup_cpu(conexion_memoria, conexion_kernel_dispatch, conexion_kernel_interrupt, logger, mensaje);
		return EXIT_FAILURE;
	}
	log_info(logger, "✅ Conectado a la memoria. [Puerto 8002]");
	mensaje = readline("~~> ⌛️ Esperando al Kernel... [Enter to continue] <~~");

	// Conexión cpu ->  kernel
	conexion_kernel_dispatch = connect_client_8001(logger);
	if (conexion_kernel_dispatch == -1)
	{
		log_error(logger, "No se pudo conectar con el kernel [DISPATCH].");
		cleanup_cpu(conexion_memoria, conexion_kernel_dispatch, conexion_kernel_interrupt, logger, mensaje);
		return EXIT_FAILURE;
	}
	log_info(logger, "✅ Conectado al kernel - modo dispatch. [Puerto 8001]");

	// Conexión cpu ->  kernel
	conexion_kernel_interrupt = connect_client_8004(logger);
	if (conexion_kernel_interrupt == -1)
	{
		log_error(logger, "No se pudo conectar con el kernel [INTERRUPT].");
		cleanup_cpu(conexion_memoria, conexion_kernel_dispatch, conexion_kernel_interrupt, logger, mensaje);
		return EXIT_FAILURE;
	}
	log_info(logger, "✅ Conectado al kernel - modo interrupt. [Puerto 8004]");

	// Loop principal de lectura y envío de mensajes
	while (true)
	{
		// Enviar mensaje al kernel - modo dispatch
		free(mensaje);
		mensaje = readline("[CPU DISPATCH] -> ");
		if (mensaje == NULL)
		{
			log_error(logger, "Error al leer input del usuario.");
			break;
		}

		if (strcmp(mensaje, "") == 0)
		{
			dispatch_vacio = 1;
		}
		else
		{
			dispatch_vacio = 0;
			enviar_mensaje(mensaje, conexion_memoria);
			enviar_mensaje(mensaje, conexion_kernel_dispatch);
		}

		// Enviar mensaje al kernel - modo interrupt
		free(mensaje);
		mensaje = readline("[CPU INTERRUPT] -> ");
		if (mensaje == NULL)
		{
			log_error(logger, "Error al leer input del usuario.");
			break;
		}

		if (strcmp(mensaje, "") == 0)
		{
			interrupt_vacio = 1;
		}
		else
		{
			interrupt_vacio = 0;
			enviar_mensaje(mensaje, conexion_memoria);
			enviar_mensaje(mensaje, conexion_kernel_interrupt);
		}

		// Comprobar si ambos inputs fueron vacíos
		if (dispatch_vacio && interrupt_vacio)
		{
			log_info(logger, "Ambos inputs vacíos recibidos. Finalizando...");
			break;
		}
	}

	// Liberar todos los recursos antes de salir
	cleanup_cpu(conexion_memoria, conexion_kernel_dispatch, conexion_kernel_interrupt, logger, mensaje);
	return EXIT_SUCCESS;
}
