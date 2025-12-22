#include "GestorDatos.hpp"

GestorDatos::GestorDatos(const nlohmann::json& AppData)
    : guias(), salas(), roles(), cambios() 
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
    // --- Salas adicionales fijas ---
    auto agregarSalaFija = [&](const std::string& nombre) {
        Sala s;
        s.nombre = nombre;
        s.capacitacion = "";      // No requieren capacitación
        s.obligatoria = false;    // No son obligatorias
        s.numero = contadorSala++;
        salas.push_back(s);
    };

    agregarSalaFija("operador1");
    agregarSalaFija("operador2");
    agregarSalaFija("vacaciones1");
    agregarSalaFija("vacaciones2");

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
    // --- Cargar rotacion ---
    if (AppData.contains("valor") && AppData["valor"].is_number_integer()) {
    valorRotacion = AppData["valor"].get<int>();
    }
    // --- Cargar operadores ---
    if (AppData.contains("operadores") && AppData["operadores"].is_object()) {
        const auto& item = AppData["operadores"]; 
        operadores.operador1 = item.value("operador1", "");
        operadores.operador2 = item.value("operador2", "");
    }
    // --- Cargar turno ---
    if (AppData.contains("turno") && AppData["turno"].is_string()) {
        turno = AppData["turno"].get<std::string>();
    }   
    // --- Cargar vacaciones ---
    if (AppData.contains("vacaciones") && AppData["vacaciones"].is_object()) {
        const auto& item = AppData["vacaciones"]; 
        vacaciones.vacacion1 = item.value("vacaciones1", "");
        vacaciones.vacacion2 = item.value("vacaciones2", "");
    } 
}
