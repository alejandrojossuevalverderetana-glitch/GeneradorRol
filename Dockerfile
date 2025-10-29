# --- Etapa 1: Base y dependencias ---
FROM ubuntu:22.04

# Evitar interacciones al instalar paquetes
ENV DEBIAN_FRONTEND=noninteractive

# Instalar herramientas necesarias
RUN apt-get update && apt-get install -y \
    g++ \
    git \
    curl \
    make \
    cmake \
    libpthread-stubs0-dev \
    && rm -rf /var/lib/apt/lists/*

# --- Etapa 2: Directorio de trabajo ---
WORKDIR /app

# --- Etapa 3: Copiar el código fuente ---
COPY . .

# --- Etapa 4: Descargar Crow ---
RUN git clone https://github.com/CrowCpp/crow.git /tmp/crow && \
    mkdir -p crow && \
    cp -r /tmp/crow/include/crow/* crow/

# --- Etapa 5: Descargar nlohmann/json ---
RUN mkdir -p nlohmann && \
    curl -L https://github.com/nlohmann/json/releases/latest/download/json.hpp -o nlohmann/json.hpp

# --- Etapa 6: Descargar cpp-httplib (si no está en tu repo) ---
# Opcional si ya lo incluís en el repo
RUN mkdir -p cpp-httplib && \
    curl -L https://raw.githubusercontent.com/yhirose/cpp-httplib/master/httplib.h -o cpp-httplib/httplib.h

# --- Etapa 7: Descargar Asio (solo headers) ---
RUN git clone https://github.com/chriskohlhoff/asio.git /tmp/asio && \
    mkdir -p asio && \
    cp -r /tmp/asio/asio/include/asio asio/

# --- Etapa 8: Compilar todos los .cpp ---
RUN g++ -std=c++17 Backend/api.cpp Backend/GeneradorRol.cpp Backend/GestorDatos.cpp \
    Backend/Guia.cpp Backend/Rol.cpp Backend/Sala.cpp \
    -Icrow -Inlohmann -Icpp-httplib -Iasio/asio/include \
    -o api -lpthread

# --- Etapa 9: Puerto expuesto ---
EXPOSE 18080

# --- Etapa 10: Comando por defecto ---
CMD ["./api"]
