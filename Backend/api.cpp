#include "crow.h"
#include "GestorDatos.hpp"
#include "GeneradorRol.hpp"
#include <nlohmann/json.hpp>
#include <iostream>

int main() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/generar").methods("POST"_method)
    ([](const crow::request& req) {
        try {
            // Parsear el cuerpo del POST como JSON
            auto jsonInput = nlohmann::json::parse(req.body);

            // Crear el gestor de datos con la información recibida
            GestorDatos gestor(jsonInput);

            // Crear el generador de roles
            GeneradorRol generador;

            // Generar los nuevos roles
            auto nuevosRoles = generador.generarRoles(gestor);

            // Convertir los roles a JSON para devolverlos
            nlohmann::json respuesta;
            for (const auto& rol : nuevosRoles) {
                respuesta["roles"].push_back({
                    {"nombreGuia", rol.nombreGuia},
                    {"nombreSala", rol.nombreSala}
                });
            }

            // Respuesta exitosa
            crow::response res;
            res.code = 200;
            res.set_header("Content-Type", "application/json");
            res.body = respuesta.dump(4);
            return res;
        }
        catch (const std::exception& e) {
            // En caso de error, devolver mensaje de error
            crow::response res;
            res.code = 400;
            res.set_header("Content-Type", "application/json");
            res.body = nlohmann::json({
                {"error", e.what()}
            }).dump(4);
            return res;
        }
    });

    std::cout << "🟢 Servidor iniciado en http://localhost:8080/generar" << std::endl;
    app.port(8080).multithreaded().run();
}
