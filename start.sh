#!/bin/bash

# --- Configuración ---
UTILS_DIR="./utils"
MEMORIA_DIR="../memoria"
CPU_DIR="../cpu"
KERNEL_DIR="../kernel"
IO_DIR="../io"
TMUX_SESSION_NAME="module_session"

# --- Verificamos si tmux está instalado ---
if ! command -v tmux >/dev/null 2>&1; then
  echo "tmux no está instalado. Intentando instalar..."
  sudo apt update
  if sudo apt install tmux -y; then
    echo "tmux instalado correctamente."
  else
    echo "Error: no se pudo instalar tmux. Instalalo a mano con:"
    echo "sudo apt update && sudo apt install tmux -y"
    exit 1
  fi
fi

# --- Compilar ---
echo "Compilando utilidades..."
cd "$UTILS_DIR" && make

echo "Compilando módulos..."
cd "$MEMORIA_DIR" && make
cd "$CPU_DIR" && make
cd "$KERNEL_DIR" && make
cd "$IO_DIR" && make
wait

# --- Reiniciar sesión si ya existe ---
if tmux has-session -t "$TMUX_SESSION_NAME" 2>/dev/null; then
  echo "Eliminando sesión tmux anterior '$TMUX_SESSION_NAME'..."
  tmux kill-session -t "$TMUX_SESSION_NAME"
fi

echo "Creando nueva sesión tmux '$TMUX_SESSION_NAME' con layout 2x2..."

# Crear sesión con primer pane en memoria
tmux new-session -d -s "$TMUX_SESSION_NAME" -n "Modules" -c "$MEMORIA_DIR"
tmux send-keys -t "$TMUX_SESSION_NAME:0.0" "./bin/memoria" Enter
tmux select-pane -t "$TMUX_SESSION_NAME:0.0" -T "memoria"

# Dividir derecha (pane 1: cpu)
tmux split-window -h -t "$TMUX_SESSION_NAME:0.0" -c "$CPU_DIR"
tmux send-keys -t "$TMUX_SESSION_NAME:0.1" "./bin/cpu" Enter
tmux select-pane -t "$TMUX_SESSION_NAME:0.1" -T "cpu"

# Dividir abajo cpu (pane 2: kernel)
tmux select-pane -t "$TMUX_SESSION_NAME:0.1"
tmux split-window -v -c "$KERNEL_DIR"
tmux send-keys -t "$TMUX_SESSION_NAME:0.2" "./bin/kernel ejemplo_1.pseudo 126" Enter
tmux select-pane -t "$TMUX_SESSION_NAME:0.2" -T "kernel"

# Esperar 10 segundos para simular Enter en CPU
echo "Esperando 10 segundos para simular conexión CPU-Kernel..."
sleep 10
tmux send-keys -t "$TMUX_SESSION_NAME:0.1" "" Enter

# Dividir abajo kernel (pane 3: io)
tmux select-pane -t "$TMUX_SESSION_NAME:0.2"
tmux split-window -v -c "$IO_DIR"
tmux send-keys -t "$TMUX_SESSION_NAME:0.3" "./bin/io io_nro_1" Enter
tmux select-pane -t "$TMUX_SESSION_NAME:0.3" -T "io"

# Aplicar layout de grilla 2x2
tmux select-layout -t "$TMUX_SESSION_NAME" tiled

# Mensaje final
echo "¡Módulos corriendo en una sola ventana tmux dividida en 4 paneles!"
echo "Podés verlos con:"
echo "tmux attach -t $TMUX_SESSION_NAME"
