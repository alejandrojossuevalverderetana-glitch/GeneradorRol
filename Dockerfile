FROM ubuntu:22.04

# Instalar dependencias
RUN apt-get update && apt-get install -y g++ git curl build-essential

WORKDIR /app

# Copiar tu proyecto
COPY . .

# Descargar Crow con la estructura correcta
RUN git clone https://github.com/CrowCpp/crow.git /tmp/crow && \
    mkdir -p crow && \
    cp -r /tmp/crow/include/* crow/

# Descargar nlohmann/json con la estructura correcta
RUN mkdir -p nlohmann && \
    curl -L https://github.com/nlohmann/json/releases/latest/download/json.hpp -o nlohmann/json.hpp

# Descargar cpp-httplib
RUN mkdir -p cpp-httplib && \
    curl -L https://raw.githubusercontent.com/yhirose/cpp-httplib/master/httplib.h -o cpp-httplib/httplib.h

# Descargar asio
RUN git clone https://github.com/chriskohlhoff/asio.git /tmp/asio && \
    mkdir -p asio && \
    cp /tmp/asio/asio/include/asio.hpp asio/ && \
    cp -r /tmp/asio/asio/include/asio asio/


# Compilar tu proyecto
RUN g++ -std=c++17 \
    Backend/api.cpp Backend/GeneradorRol.cpp Backend/GestorDatos.cpp \
    -Icrow -Inlohmann -Icpp-httplib -Iasio \
    -o api -lpthread

EXPOSE 18080
CMD ["./api"]
