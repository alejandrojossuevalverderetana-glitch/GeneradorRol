#include "crow.h"
#include "GestorDatos.hpp"
#include "GeneradorRol.hpp"
#include <json.hpp>
#include <iostream>

struct CorsMiddleware {
    struct context {};

    void before_handle(crow::request& req, crow::response& res, context&) {
        // Cabeceras CORS globales
        res.add_header("Access-Control-Allow-Origin", "*");
        res.add_header("Access-Control-Allow-Methods", "POST, GET, OPTIONS");
        res.add_header("Access-Control-Allow-Headers", "Content-Type");

        // Responder OPTIONS de inmediato
        if (req.method == crow::HTTPMethod::OPTIONS) {
            res.code = 200;
            res.end();
        }
    }

    void after_handle(crow::request&, crow::response&, context&) {}
};

int main() {
    crow::App<CorsMiddleware> app;

    // Endpoint POST para generar roles
    CROW_ROUTE(app, "/generar").methods("POST"_method)
    ([](const crow::request& req){
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

            crow::response res;
            res.code = 200;
            res.set_header("Content-Type", "application/json");
            res.body = respuesta.dump(4);
            return res;
        }
        catch(const std::exception& e) {
            crow::response res;
            res.code = 400;
            res.set_header("Content-Type", "application/json");
            res.body = nlohmann::json({ {"error", e.what()} }).dump(4);
            return res;
        }
    });

    std::cout << "🟢 Servidor iniciado en http://localhost:8080/generar" << std::endl;
    app.port(8080).multithreaded().run();
}
