#include "crow.h"
#include "GestorDatos.hpp"
#include "GeneradorRol.hpp"
#include <json.hpp>
#include <iostream>

int main() {
    crow::SimpleApp app;

    // --------------------------
    // 🔹 Endpoint único para POST y OPTIONS con CORS
    // --------------------------
    CROW_ROUTE(app, "/generar")
    ([](const crow::request& req){
        crow::response res;

        // Cabeceras CORS para cualquier método
        res.add_header("Access-Control-Allow-Origin", "*");
        res.add_header("Access-Control-Allow-Methods", "POST, GET, OPTIONS");
        res.add_header("Access-Control-Allow-Headers", "Content-Type");

        if(req.method == crow::HTTPMethod::OPTIONS) {
            // Preflight: responder OK sin cuerpo
            res.code = 200;
            res.end();
            return res;
        }

        // POST normal: generar roles
        try {
            auto jsonInput = nlohmann::json::parse(req.body);
            GestorDatos gestor(jsonInput);
            GeneradorRol generador;
            auto nuevosRoles = generador.generarRoles(gestor);

            nlohmann::json respuesta;
            for (const auto& rol : nuevosRoles) {
                respuesta["roles"].push_back({
                    {"nombreGuia", rol.nombreGuia},
                    {"nombreSala", rol.nombreSala}
                });
            }

            res.code = 200;
            res.set_header("Content-Type", "application/json");
            res.body = respuesta.dump(4);
            return res;
        }
        catch(const std::exception& e) {
            res.code = 400;
            res.set_header("Content-Type", "application/json");
            res.body = nlohmann::json({ {"error", e.what()} }).dump(4);
            return res;
        }
    });

    std::cout << "🟢 Servidor iniciado en http://localhost:8080/generar" << std::endl;
    app.port(8080).multithreaded().run();
}
