#include "GeneradorRol.hpp"
#include <algorithm>
#include <random>
#include <iostream>
#include <unordered_set>

/**
 * @brief Verifica si hay suficientes guías para cubrir las salas obligatorias.
 */
bool GeneradorRol::ValidarDisponibilidad(const std::vector<GestorDatos::Guia>& guias,
                                         const std::vector<GestorDatos::Sala>& salas) 
{
    int countObligatorias = 0;
    for (const auto& s : salas) {
        if (s.obligatoria) countObligatorias++;
    }
    return static_cast<int>(guias.size()) >= countObligatorias;
}

/**
 * @brief Busca los guías que cumplen los requisitos de una sala.
 */
std::vector<GestorDatos::Guia> GeneradorRol::BuscarGuiasValidos(
    const GestorDatos::Sala& sala,
    const std::vector<GestorDatos::Guia>& guias,
    const std::vector<GestorDatos::RolGenerado>& rolesActuales,
    const std::vector<GestorDatos::Sala>& salas)
{
    std::vector<GestorDatos::Guia> validos;

    for (const auto& g : guias) {
        // 1️⃣ Verificar si el guía tiene la capacitación necesaria
        bool tieneCap = std::find(g.capacitaciones.begin(), g.capacitaciones.end(),
                                  sala.capacitacion) != g.capacitaciones.end();
        if (!tieneCap) continue;

        // 2️⃣ Verificar si el guía ya está asignado a otra sala con capacitación
        bool yaAsignado = false;
        for (const auto& rol : rolesActuales) {
            if (rol.nombreGuia == g.nombre) {
                auto itSala = std::find_if(salas.begin(), salas.end(),
                    [&](const GestorDatos::Sala& s){ return s.nombre == rol.nombreSala; });
                if (itSala != salas.end() && !itSala->capacitacion.empty()) {
                    yaAsignado = true;
                    break;
                }
            }
        }

        if (!yaAsignado) validos.push_back(g);
    }

    return validos;
}

/**
 * @brief Genera la asignación inicial de guías a salas (rol desplazado).
 */
void GeneradorRol::AsignarGuias(const std::vector<GestorDatos::Guia>& guias,
                                const std::vector<GestorDatos::Sala>& salas,
                                const std::vector<GestorDatos::Rol>& rolesPrevios,
                                const int cantidadRotacion,
                                const std::string& turno)
{
    size_t nSalas = salas.size();
    std::vector<std::string> guiasPorSala(nSalas, "");

    // Mapear roles previos
    for (const auto& rolPrev : rolesPrevios) {
        auto itSala = std::find_if(salas.begin(), salas.end(),
            [&](const GestorDatos::Sala& s){ return s.nombre == rolPrev.nombreSala; });
        if (itSala == salas.end()) continue;

        size_t idx = std::distance(salas.begin(), itSala);
        for (const auto& g : guias) {
            if (g.nombre == rolPrev.nombreGuia && g.turno == turno) {
                guiasPorSala[idx] = g.nombre;
                break;
            }
        }
    }

    // Rotación de guías
    std::vector<std::string> guiasRotadas(nSalas, "");
    int rot = nSalas > 0 ? cantidadRotacion % static_cast<int>(nSalas) : 0;
    for (size_t i = 0; i < nSalas; ++i) {
        if (guiasPorSala[i].empty()) continue;
        size_t target = (i + rot) % nSalas;
        size_t attempt = 0;
        while (!guiasRotadas[target].empty() && attempt < nSalas) {
            target = (target + 1) % nSalas;
            ++attempt;
        }
        guiasRotadas[target] = guiasPorSala[i];
    }

    // Agregar guías nuevas
    std::unordered_set<std::string> asignados;
    for (const auto& name : guiasRotadas) if (!name.empty()) asignados.insert(name);

    size_t idxNew = 0;
    for (const auto& g : guias) {
        if (g.turno == turno && asignados.find(g.nombre) == asignados.end()) {
            while (idxNew < nSalas && !guiasRotadas[idxNew].empty()) ++idxNew;
            if (idxNew < nSalas) {
                guiasRotadas[idxNew++] = g.nombre;
                asignados.insert(g.nombre);
            }
        }
    }

    // Construir roles generados
    rolesGenerados.clear();
    rolesGenerados.reserve(nSalas);
    for (size_t i = 0; i < nSalas; ++i) {
        GestorDatos::RolGenerado rol;
        rol.nombreSala = salas[i].nombre;
        rol.nombreGuia = guiasRotadas[i];
        rol.nombreCambioInterno = "";
        rol.nombreCambioAprobado = "";
        rolesGenerados.push_back(std::move(rol));
    }
}

/**
 * @brief Verifica que cada guía cumpla requisitos de capacitación en su sala.
 */
std::vector<GestorDatos::RolGenerado> GeneradorRol::ComprobarAsignacion(
    const std::vector<GestorDatos::Guia>& guias,
    const std::vector<GestorDatos::Sala>& salas,
    const std::vector<GestorDatos::RolGenerado>& roles)
{
    std::vector<GestorDatos::RolGenerado> invalidos;
    for (const auto& rol : roles) {
        auto itSala = std::find_if(salas.begin(), salas.end(),
                                   [&](const GestorDatos::Sala& s){ return s.nombre == rol.nombreSala; });
        if (itSala != salas.end() && !itSala->capacitacion.empty()) {
            auto itGuia = std::find_if(guias.begin(), guias.end(),
                                       [&](const GestorDatos::Guia& g){ return g.nombre == rol.nombreGuia; });
            if (itGuia == guias.end() ||
                std::find(itGuia->capacitaciones.begin(), itGuia->capacitaciones.end(),
                          itSala->capacitacion) == itGuia->capacitaciones.end())
            {
                invalidos.push_back(rol);
            }
        }
    }
    return invalidos;
}

/**
 * @brief Aplica cambios internos a los roles generados si el guía no cumple requisitos.
 */
void GeneradorRol::AplicarCambiosInternos(
    const std::vector<GestorDatos::Guia>& guias,
    const std::vector<GestorDatos::Sala>& salas)
{
    auto invalidos = ComprobarAsignacion(guias, salas, rolesGenerados);

    for (auto& rol : invalidos) {
        auto itSala = std::find_if(salas.begin(), salas.end(),
                                   [&](const GestorDatos::Sala& s){ return s.nombre == rol.nombreSala; });
        if (itSala == salas.end()) continue;

        auto guiasValidos = BuscarGuiasValidos(*itSala, guias, rolesGenerados, salas);
        if (guiasValidos.empty()) continue;

        std::string nuevoGuia = "";
        for (const auto& g : guiasValidos) {
            bool ocupado = std::any_of(rolesGenerados.begin(), rolesGenerados.end(),
                                       [&](const GestorDatos::RolGenerado& r){ return r.nombreGuia == g.nombre; });
            if (!ocupado) {
                nuevoGuia = g.nombre;
                break;
            }
        }
        if (nuevoGuia.empty()) nuevoGuia = guiasValidos.front().nombre;

        for (auto& r : rolesGenerados) {
            if (r.nombreSala == rol.nombreSala) {
                r.nombreCambioInterno = nuevoGuia;
                break;
            }
        }
    }
}

/**
 * @brief Genera el rol completo del día, aplicando rotación y cambios internos.
 */
std::vector<GestorDatos::RolGenerado> GeneradorRol::generarRoles(const GestorDatos& datos) 
{
    // 1️⃣ Rol desplazado según turno "manana"
    AsignarGuias(datos.guias, datos.salas, datos.roles, datos.valor, "manana");

    // 2️⃣ Aplicar cambios internos por capacitación
    AplicarCambiosInternos(datos.guias, datos.salas);

    // 3️⃣ En este momento se pueden reflejar cambios externos si se implementa lógica adicional

    return rolesGenerados;
}