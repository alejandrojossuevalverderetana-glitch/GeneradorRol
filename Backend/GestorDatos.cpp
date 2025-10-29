#include "GestorDatos.hpp"

GestorDatos::GestorDatos(const nlohmann::json& AppData)
    : guias(), salas(), roles(), cambios() // Inicializamos vectores vacíos
{
    int contadorSala = 0;
    // --- Cargar guías ---
    if (AppData.contains("guias") && AppData["guias"].is_array()) {
        for (const auto& item : AppData["guias"]) {
            Guia g;
            g.nombre = item.value("nombre", "");
            g.turno = item.value("turno", "");
            if (item.contains("capacitaciones")) {
            for (const auto& c : item["capacitaciones"])
                g.capacitaciones.push_back(c.get<std::string>());
            }
            
            guias.push_back(g);
        }
    }

    // --- Cargar salas ---
    if (AppData.contains("salas") && AppData["salas"].is_array()) {
        for (const auto& item : AppData["salas"]) {
            Sala s;
            s.nombre = item.value("nombre", "");
            s.capacitacion = item.value("capacitacion", "");
            s.obligatoria = item.value("obligatoria",false);
            s.numero = contadorSala++ ;
            salas.push_back(s);
        }
    }

    // --- Cargar roles ---
    if (AppData.contains("roles") && AppData["roles"].is_array()) {
        for (const auto& item : AppData["roles"]) {
            Rol r;
            r.nombreGuia = item.value("nombreGuia", "");
            r.nombreSala = item.value("nombreSala", "");
            roles.push_back(r);
        }
    }

    // --- Cargar cambios ---
    if (AppData.contains("cambios") && AppData["cambios"].is_array()) {
        for (const auto& item : AppData["cambios"]) {
            Cambio c;
            c.guiaTarde = item.value("guiaTarde", "");
            c.guiaManana = item.value("guiaManana", "");
            cambios.push_back(c);
        }
    }
}
