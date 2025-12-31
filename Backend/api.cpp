#include "crow.h"
#include "GestorDatos.hpp"
#include "GeneradorRol.hpp"
#include <json.hpp>
#include <iostream>

struct CorsMiddleware {
    struct context {};

    void before_handle(crow::request& req, crow::response& res, context&) {
        res.add_header("Access-Control-Allow-Origin", "*");
        res.add_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
        res.add_header("Access-Control-Allow-Headers", "Content-Type, Authorization, X-Requested-With");

        if (req.method == crow::HTTPMethod::OPTIONS) {
            res.code = 204;
            res.end();
        }
    }

    void after_handle(crow::request&, crow::response& res, context&) {
        // Garantiza que todas las respuestas (incluyendo POST) lleven el header CORS
        res.add_header("Access-Control-Allow-Origin", "*");
        res.add_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
        res.add_header("Access-Control-Allow-Headers", "Content-Type, Authorization, X-Requested-With");
    }
};

int main() {
    crow::App<CorsMiddleware> app;

    CROW_ROUTE(app, "/generar").methods(crow::HTTPMethod::POST)
([](const crow::request& req) {
    try {
        std::cout << "📩 Recibido POST /generar" << std::endl;
        std::cout << "Body: " << req.body << std::endl;

        auto jsonInput = nlohmann::json::parse(req.body);
        std::cout << "✅ JSON parseado correctamente" << std::endl;

        GestorDatos gestor(jsonInput);
        std::cout << "✅ GestorDatos creado" << std::endl;

        GeneradorRol generador;
        auto nuevosRoles = generador.generarRoles(gestor);
        std::cout << "✅ Roles generados: " << nuevosRoles.size() << std::endl;

        nlohmann::json respuesta;
        for (const auto& rol : nuevosRoles) {
            respuesta["roles"].push_back({
                {"nombreGuia", rol.nombreGuia},
                {"nombreSala", rol.nombreSala},
                {"nombreCambioInterno", rol.nombreCambioInterno},
                {"nombreCambioAprobado", rol.nombreCambioAprobado}
            });
        }

        crow::response res(respuesta.dump(4));
        res.code = 200;
        res.set_header("Content-Type", "application/json");
        std::cout << "✅ Respuesta enviada correctamente" << std::endl;
        return res;

    } catch (const std::exception& e) {
        std::cerr << "❌ Error interno: " << e.what() << std::endl;
        nlohmann::json error = {{"error", e.what()}};
        crow::response res(error.dump(4));
        res.code = 400;
        res.set_header("Content-Type", "application/json");
        return res;
    }
});


    std::cout << "🟢 Servidor iniciado en https://0.0.0.0:8080 (localhost o IP pública)" << std::endl ;
    app.bindaddr("0.0.0.0").port(8080)
    .ssl_file("/app/certs/fullchain.pem", "/app/certs/privkey.pem")
    .multithreaded()
    .run();
}