#include "GeneradorRol.hpp"
#include <algorithm>
#include <random>
#include <iostream>

/**
 * @brief Verifica si hay suficientes guías para cubrir las salas críticas.
 */
bool GeneradorRol::ValidarDisponibilidad(const std::vector<GestorDatos::Guia>& guias,
                                         const std::vector<GestorDatos::Sala>& salas) {
    std::vector<GestorDatos::Sala> salasObligatorias;

    for (const auto& s : salas) {
        if (s.obligatoria) {
            salasObligatorias.push_back(s);
        }
    }

     if ((int)guias.size() < salasObligatorias.size()) {
        return false;
    }

    return true;
}

/**
 * @brief Busca los guías que cumplen los requisitos de una sala.
 */
std::vector<GestorDatos::Guia> GeneradorRol::BuscarGuiasValidos(
    const GestorDatos::Sala& sala,
    const std::vector<GestorDatos::Guia>& guias,
    const std::vector<GestorDatos::RolGenerado>& rolesGenerados,
    const std::vector<GestorDatos::Sala>& salas)
{
    std::vector<GestorDatos::Guia> validos;

    for (const auto& g : guias) {
        // 1️⃣ Verificar si el guía tiene la capacitación necesaria para la sala actual
        bool tieneCapacitacion = std::find(
            g.capacitaciones.begin(),
            g.capacitaciones.end(),
            sala.capacitacion
        ) != g.capacitaciones.end();

        if (!tieneCapacitacion)
            continue;

        // 2️⃣ Verificar si el guía ya está asignado a una sala que requiere capacitación
        bool yaAsignadoConCapacitacion = false;
        for (const auto& rol : rolesGenerados) {
            if (rol.nombreGuia == g.nombre) {
                // Buscar la sala correspondiente a ese rol
                auto itSala = std::find_if(salas.begin(), salas.end(),
                    [&](const GestorDatos::Sala& s) {
                        return s.nombre == rol.nombreSala;
                    });

                // Si la sala existe y requiere capacitación
                if (itSala != salas.end() && !itSala->capacitacion.empty()) {
                    yaAsignadoConCapacitacion = true;
                    break;
                }
            }
        }

        if (!yaAsignadoConCapacitacion) {
            validos.push_back(g);
        }
    }

    return validos;
}


/**
 * @brief Asigna guías a salas.
 *
 * Ahora:
 * - La rotación desplaza a cada guía a través del índice de salas (wrap sobre salas.size()).
 * - Solo se consideran guías cuyo turno == turnoRol.
 * - Se quita la lógica de salas obligatorias (será manejada por otro módulo).
 */
void GeneradorRol::AsignarGuias(const std::vector<GestorDatos::Guia>& guias,
                                const std::vector<GestorDatos::Sala>& salas,
                                const std::vector<GestorDatos::Rol>& roles,
                                const int cantidadRotacion,
                                const std::string& turnoRol)
{
    size_t nSalas = salas.size();

    // 1) Construir un vector que mapea cada índice de sala -> nombreGuia (según roles previos),
    //    respetando el orden actual de 'salas'.
    std::vector<std::string> guiasPorSala(nSalas, ""); // posiciones vacías por defecto

    for (const auto& rolPrev : roles) {
        // localizar índice de la sala de ese rol en el vector 'salas'
        auto itSalaIdx = std::find_if(salas.begin(), salas.end(),
            [&](const GestorDatos::Sala& s){ return s.nombre == rolPrev.nombreSala; });

        if (itSalaIdx == salas.end()) continue; // sala ya no existe en la lista actual

        size_t idx = std::distance(salas.begin(), itSalaIdx);

        // verificar que el guía existe y pertenece al turno solicitado
        bool guiaValido = false;
        for (const auto& g : guias) {
            if (g.nombre == rolPrev.nombreGuia && g.turno == turnoRol) {
                guiaValido = true;
                break;
            }
        }
        if (guiaValido) {
            guiasPorSala[idx] = rolPrev.nombreGuia;
        }
        // si el guía no existe o no coincide el turno, dejamos la posición vacía
    }

    // 2) Rotar cada guía a través del espacio de salas (desplazamiento por sala, wrap)
    //    Para evitar sobreescrituras, colocamos en un nuevo vector y si hay colisión,
    //    buscamos el siguiente hueco libre (circular).
    std::vector<std::string> guiasPorSalaRotada(nSalas, "");
    int rot = 0;
    if (nSalas > 0) rot = cantidadRotacion % static_cast<int>(nSalas);
    for (size_t i = 0; i < nSalas; ++i) {
        if (guiasPorSala[i].empty()) continue;
        size_t target = (i + rot) % nSalas;

        // si la posición target está ocupada, buscar siguiente hueco
        size_t attempt = 0;
        while (!guiasPorSalaRotada[target].empty() && attempt < nSalas) {
            target = (target + 1) % nSalas;
            ++attempt;
        }
        // si intentamos nSalas veces y no hay hueco, se sobrescribe (caso muy raro cuando
        // hay más guías que posiciones libres en la rotada). Aquí simplemente sobrescribimos.
        guiasPorSalaRotada[target] = guiasPorSala[i];
    }

    // 3) Añadir guías nuevos (del listado 'guias') que no estén ya asignados, y que
    //    cumplan el turno. Rellenamos las posiciones vacías en orden.
    //    Primero construimos un conjunto de nombres ya asignados para evitar duplicados.
    std::unordered_set<std::string> asignados;
    for (const auto& name : guiasPorSalaRotada) {
        if (!name.empty()) asignados.insert(name);
    }

    // Lista de guías válidas por turno
    std::vector<std::string> guiasValidasPorTurno;
    guiasValidasPorTurno.reserve(guias.size());
    for (const auto& g : guias) {
        if (g.turno == turnoRol) {
            if (asignados.find(g.nombre) == asignados.end()) {
                guiasValidasPorTurno.push_back(g.nombre);
            }
        }
    }

    // Rellenar huecos de guiasPorSalaRotada con guías nuevas válidas (en el orden que vienen)
    size_t idxNewGuide = 0;
    for (size_t i = 0; i < nSalas && idxNewGuide < guiasValidasPorTurno.size(); ++i) {
        if (guiasPorSalaRotada[i].empty()) {
            guiasPorSalaRotada[i] = guiasValidasPorTurno[idxNewGuide++];
        }
    }

    // 4) Construir rolesGenerados (uno por sala, asignando la guía correspondiente o vacío)
    std::vector<GestorDatos::RolGenerado> rolesGenerados;
    rolesGenerados.reserve(nSalas);

    for (size_t i = 0; i < nSalas; ++i) {
        GestorDatos::RolGenerado nuevoRol;
        nuevoRol.nombreSala = salas[i].nombre;
        nuevoRol.nombreGuia = guiasPorSalaRotada[i]; // puede ser "" si no hay guía
        nuevoRol.nombreCambioInterno = "";
        nuevoRol.nombreCambioAprobado = "";
        rolesGenerados.push_back(std::move(nuevoRol));
    }

    // 5) Guardar los roles generados
    this->rolesGenerados = std::move(rolesGenerados);
}



/**
 * @brief Verifica que cada guía esté asignado a una sala compatible.
 */
std::vector<GestorDatos::RolGenerado> GeneradorRol::ComprobarAsignacion(
    const std::vector<GestorDatos::Guia>& guias,
    const std::vector<GestorDatos::Sala>& salas,
    const std::vector<GestorDatos::RolGenerado>& roles)
{
    std::vector<GestorDatos::RolGenerado> invalidos;

    for (const auto& rol : roles) {
        // Buscar la sala correspondiente
        auto itSala = std::find_if(salas.begin(), salas.end(),
                                   [&](const GestorDatos::Sala& s){ return s.nombre == rol.nombreSala; });

        if (itSala != salas.end() && !itSala->capacitacion.empty()) {
            // Sala requiere capacitación, buscar al guía
            auto itGuia = std::find_if(guias.begin(), guias.end(),
                                       [&](const GestorDatos::Guia& g){ return g.nombre == rol.nombreGuia; });

            if (itGuia != guias.end()) {
                // Verificar si el guía tiene la capacitación requerida
                bool tieneCap = std::find(itGuia->capacitaciones.begin(), itGuia->capacitaciones.end(),
                                          itSala->capacitacion) != itGuia->capacitaciones.end();
                if (!tieneCap) {
                    invalidos.push_back(rol);
                }
            } else {
                // Guía no existe, rol inválido
                invalidos.push_back(rol);
            }
        }
    }

    return invalidos;
}

/**
 * @brief Reasigna guías entre salas si no cumplen requisitos.
 */
void GeneradorRol::AplicarCambiosInternos(
    const std::vector<GestorDatos::Guia>& guias,
    const std::vector<GestorDatos::Sala>& salas)
{
    // Obtener los roles inválidos
    auto rolesInvalidos = ComprobarAsignacion(guias, salas, this->rolesGenerados);

    for (auto& rolInvalido : rolesInvalidos) {
        // Buscar la sala correspondiente
        auto itSala = std::find_if(salas.begin(), salas.end(),
                                   [&](const GestorDatos::Sala& s){ return s.nombre == rolInvalido.nombreSala; });
        if (itSala == salas.end()) continue;

        // Buscar posibles guías válidos para esta sala
        auto guiasValidos = BuscarGuiasValidos(*itSala, guias, rolesGenerados, salas);
        if (guiasValidos.empty()) {
            std::cerr << "[INFO] No hay guías capacitados disponibles para la sala '"
                      << itSala->nombre << "'.\n";
            continue;
        }

        // Escoger un guía que no esté ya asignado
        std::string nuevoGuia = "";
        for (const auto& g : guiasValidos) {
            bool ocupado = std::any_of(rolesGenerados.begin(), rolesGenerados.end(),
                                       [&](const GestorDatos::RolGenerado& r){
                                           return r.nombreGuia == g.nombre;
                                       });
            if (!ocupado) {
                nuevoGuia = g.nombre;
                break;
            }
        }

        // Si no se encontró uno libre, se puede permitir el cambio con alguien ocupado
        if (nuevoGuia.empty()) nuevoGuia = guiasValidos.front().nombre;

        // Aplicar el cambio interno
        for (auto& rol : rolesGenerados) {
            if (rol.nombreSala == rolInvalido.nombreSala) {
                rol.nombreCambioInterno = nuevoGuia;  
                break;
            }
        }
    }
}



/**
 * @brief Genera un conjunto de roles válidos para el día.
 */
std::vector<GestorDatos::RolGenerado> GeneradorRol::generarRoles(const GestorDatos& datos) {
    // Generar los roles asignando guías a salas
    AsignarGuias(datos.guias, datos.salas, datos.roles, datos.valor, "manana");

    // Retornar el resultado generado
    return this->rolesGenerados;
}