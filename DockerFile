FROM debian:bookworm-slim

# Instalar dependencias
RUN apt-get update && apt-get install -y g++ make git curl && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copiar todo el proyecto
COPY . .

# Descargar Crow
RUN git clone https://github.com/CrowCpp/crow.git /tmp/crow && \
    mkdir -p crow && \
    cp -r /tmp/crow/include/crow/* crow/

# Descargar JSON
RUN mkdir -p nlohmann && \
    curl -L https://github.com/nlohmann/json/releases/latest/download/json.hpp -o nlohmann/json.hpp

# Compilar: ruta correcta de los archivos
RUN g++ -std=c++17 Backend/api.cpp Backend/GeneradorRol.cpp -o api \
    -Icrow -Inlohmann

EXPOSE 8080

CMD ["./api"]

