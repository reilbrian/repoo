# Module Connection Integration - Setup and Execution Guide

This document outlines the steps to clone, build, and run the interconnected CPU, Memory, Kernel, and IO modules.

## Prerequisites

* **Git:** For cloning the repository.
* **Make:** For building the modules.
* **A terminal or command prompt:** You will need at least four terminal windows to run each module concurrently.

There are two ways to run this integration...
1. Run a bash script (This bash installs tmux)
2. Manual

# Automatic setup (with bash)
1.  **Clone the Repository:**
    Open your terminal and run the following command to clone the project repository:
    ```bash
    git clone https://github.com/sisoputnfrba/tp-2025-1c-Magios.git
    ```
The repository has a `start.sh` file. You need to give write access with the following command:
```bash
cd tp-2025-1c-Magios
chmod +x start.sh
```

Later, run the script:
```bash
./start.sh
```
To see the result
```bash
tmux attach -t module_session
```
> [!TIP]
> To navigate between windows in tmux, press Ctrl + b (release the keys), then use the arrow keys to move between the windows.

# Manual setup
## Setup

1.  **Clone the Repository:**
    Open your terminal and run the following command to clone the project repository:
    ```bash
    git clone https://github.com/sisoputnfrba/tp-2025-1c-Magios.git
    ```

## Building the Modules

1.  **Build Utilities:**
    Navigate to the `utils` directory and build the utility library:
    ```bash
    cd tp-2025-1c-Magios/utils
    make
    ```

2.  **Build Memory Module:**
    Navigate to the `memoria` directory and build the memory module:
    ```bash
    cd tp-2025-1c-Magios/memoria
    make
    ```

3.  **Build CPU Module:**
    Navigate to the `cpu` directory and build the CPU module:
    ```bash
    cd tp-2025-1c-Magios/cpu
    make
    ```

4.  **Build Kernel Module:**
    Navigate to the `kernel` directory and build the kernel module:
    ```bash
    cd tp-2025-1c-Magios/kernel
    make
    ```

5.  **Build IO Module:**
    Navigate to the `io` directory and build the IO module:
    ```bash
    cd tp-2025-1c-Magios/io
    make
    ```

## Running the Modules and Verifying Connections

Open **four separate terminal windows** and follow these steps in order:

**Terminal 1 (Memory):**

1.  Navigate to the `memoria` directory:
    ```bash
    cd tp-2025-1c-Magios/memoria
    ```
2.  Run the memory module:
    ```bash
    ./bin/memoria
    ```
    You should see output similar to:
    ```bash
    # memoria
    [INFO] 13:19:58:420 MEMORIA/(11825:11825): Logger y módulo Memoria instanciado exitosamente.
    [INFO] 13:19:58:421 MEMORIA/(11825:11825): Configuración del cliente instanciada exitosamente.
    [INFO] 13:19:58:421 MEMORIA/(11825:11825): 👂 Escuchando la IP 127.0.0.1 [8002]
    [INFO] 13:19:58:421 MEMORIA/(11825:11825): Servidor iniciado en 127.0.0.1:8002. Esperando el CPU...
    ```

**Terminal 2 (CPU):**

1.  Navigate to the `cpu` directory:
    ```bash
    cd tp-2025-1c-Magios/cpu
    ```
2.  Run the CPU module:
    ```bash
    ./bin/cpu
    ```
    You should see output similar to:
    ```
    #cpu
    [INFO] 13:20:47:661 CPU/(11915:11915): ✅ Módulo CPU instanciado exitosamente.
    [INFO] 13:20:47:661 CPU/(11915:11915): 🛠️ Configuración del cliente instanciada exitosamente.
    [INFO] 13:20:47:661 CPU/(11915:11915): 🔗 Conectado exitosamente a la IP: 127.0.0.1. [8002]
    [INFO] 13:20:47:661 CPU/(11915:11915): ✅ Conectado a la memoria. [Puerto 8002]
    ~~> ⌛️ Esperando al Kernel... [Enter to continue] <~~
    ```
    The Memory terminal should now show:
    ```bash
    [INFO] 13:20:47:661 MEMORIA/(11825:11825): El modulo CPU se conectó exitosamente.
    [INFO] 13:20:47:661 MEMORIA/(11825:11825): ⌛️ Conectado a la CPU. Esperando el KERNEL...
    ```

**Terminal 3 (Kernel):**

1.  Navigate to the `kernel` directory:
    ```bash
    cd tp-2025-1c-Magios/kernel
    ```
2.  Run the kernel module:
    ```bash
    ./bin/kernel
    ```
    You should see output similar to:
    ```bash
    #kernel
    [INFO] 13:22:03:312 KERNEL/(12006:12006): Logger y módulo Kernel instanciado exitosamente.
    [INFO] 13:22:03:313 KERNEL/(12006:12006): Configuracion del cliente instanciada exitosamente.
    [INFO] 13:22:03:313 KERNEL/(12006:12006): 🔗 Conectado exitosamente a la IP: 127.0.0.1. [8002]
    [INFO] 13:22:03:313 KERNEL/(12006:12006): 👂 Escuchando la IP 127.0.0.1 [8001]
    [INFO] 13:22:03:313 KERNEL/(12006:12006): 👂 Escuchando la IP 127.0.0.1 [8004]
    [INFO] 13:22:03:313 KERNEL/(12006:12006): 👂 Escuchando la IP 127.0.0.1 [8003]
    ```
    The Memory terminal should now show:
    ```bash
    [INFO] 13:22:03:313 MEMORIA/(11825:11825): El modulo KERNEL se conectó exitosamente.
    ```

**Back to Terminal 2 (CPU):** Press the **Enter** key.

You should see additional output in the CPU terminal:

```bash
[INFO] 13:22:52:610 CPU/(11915:11915): 🛠️ Configuración del cliente instanciada exitosamente.
[INFO] 13:22:52:611 CPU/(11915:11915): 🔗 Conectado exitosamente a la IP: 127.0.0.1. [8001]
[INFO] 13:22:52:611 CPU/(11915:11915): ✅ Conectado al kernel - modo dispatch. [Puerto 8001]
[INFO] 13:22:52:611 CPU/(11915:11915): 🛠️ Configuración del cliente instanciada exitosamente.
[INFO] 13:22:52:611 CPU/(11915:11915): 🔗 Conectado exitosamente a la IP: 127.0.0.1. [8004]
[INFO] 13:22:52:611 CPU/(11915:11915): ✅ Conectado al kernel - modo interrupt. [Puerto 8004]
[CPU DISPATCH] ->
```

And in the Kernel terminal:

```bash
[INFO] 13:22:52:611 KERNEL/(12006:12006): El modulo CPU_DISPATCH se conectó exitosamente.
[INFO] 13:22:52:611 KERNEL/(12006:12006): ✅ Conectado a la CPU - modo dispatch. [Puerto 8001]
[INFO] 13:22:52:611 KERNEL/(12006:12006): El modulo CPU_INTERRUPT se conectó exitosamente.
[INFO] 13:22:52:611 KERNEL/(12006:12006): ✅ Conectado a la CPU - modo interrupt. [Puerto 8004]
```

**Terminal 4 (IO):**

1.  Navigate to the `io` directory:
    ```bash
    cd tp-2025-1c-Magios/io
    ```
2.  Run the IO module:
    ```bash
    ./bin/io
    ```
    You should see output similar to:
    ```bash
    #io
    [INFO] 13:23:53:384 IO/(12069:12069): Logger y módulo IO instanciado exitosamente.
    [INFO] 13:23:53:385 IO/(12069:12069): Configuracion del cliente instanciada exitosamente.
    [INFO] 13:23:53:385 IO/(12069:12069): 🔗 Conectado exitosamente a la IP: 127.0.0.1. [8003]
    [INFO] 13:23:53:385 IO/(12069:12069): Conexión con el Kernel establecida.
    [IO] ->
    ```
    And in the Kernel terminal:
    ```bash
    [INFO] 13:23:53:385 KERNEL/(12006:12006): El modulo IO se conectó exitosamente.
    [INFO] 13:23:53:385 KERNEL/(12006:12006): ✅ Conectado a la IO. [Puerto 8003]
    ```

## Verifying Data Flow

* **CPU Terminal:** Typing input should be prefixed with `[CPU DISPATCH] ->` initially, and later with `[CPU INTERRUPT] ->`.
* **IO Terminal:** Typing input should be prefixed with `[IO] ->`.
* Experiment with sending input from the CPU and IO terminals to observe how the messages are routed and processed by the connected modules.

## Terminating the Processes

1.  In the **IO terminal**, press **Enter**.
2.  In the **CPU terminal**, press **Enter** once (if in dispatch mode) and then again (if in interrupt mode was active).
3.  Finally, you can close the terminal windows for the Kernel and Memory modules.
