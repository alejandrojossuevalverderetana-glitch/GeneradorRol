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
std::vector<GestorDatos::Guia> GeneradorRol::BuscarGuiasValidos(const GestorDatos::Sala& sala, const std::vector<GestorDatos::Guia>& guias) {
   std::vector<GestorDatos::Guia> validos;

    for (const auto& g : guias) {
        if (std::find(g.capacitaciones.begin(), g.capacitaciones.end(), sala.capacitacion)
            != g.capacitaciones.end())
        {
            validos.push_back(g);
        }
    }

    return validos;
}

/**
 * @brief Asigna guías a salas.
 */
void GeneradorRol::AsignarGuias(const std::vector<GestorDatos::Guia>& guias,
                                const std::vector<GestorDatos::Sala>& salas,
                                const std::vector<GestorDatos::Rol>& roles,
                                int cantidadRotacion)
{
    std::vector<std::string> guiasRol;

    // Obtener nombres de guías que estaban en roles anteriores
    for (const auto& rol : roles) {
        guiasRol.push_back(rol.nombreGuia);
    }

    // Crear nueva lista de guías actualizada (rotada y sincronizada)
    std::vector<std::string> guiasActualizados = guiasRol;

    if (!guiasActualizados.empty()) {
        // Rotar 3 espacios (o la cantidad indicada)
        int rotacion = std::min<int>(cantidadRotacion, guiasActualizados.size());
        std::rotate(guiasActualizados.begin(),
                    guiasActualizados.end() - rotacion,
                    guiasActualizados.end());
    }

    // Eliminar guías que ya no existen en la lista general
    guiasActualizados.erase(
        std::remove_if(guiasActualizados.begin(), guiasActualizados.end(),
            [&](const std::string& nombre) {
                return std::none_of(guias.begin(), guias.end(),
                    [&](const GestorDatos::Guia& g) { return g.nombre == nombre; });
            }),
        guiasActualizados.end()
    );

    // Agregar al final los guías nuevos (que están en la lista general pero no en la rotada)
    for (const auto& g : guias) {
        if (std::find(guiasActualizados.begin(), guiasActualizados.end(), g.nombre) == guiasActualizados.end()) {
            guiasActualizados.push_back(g.nombre);
        }
    }

    // Asignación de guías a salas (en orden circular usando la lista actualizada)
    std::vector<GestorDatos::Rol> rolesGenerados;
for (size_t i = 0; i < salas.size(); ++i) {
    GestorDatos::Rol nuevoRol;
    if (i < guiasActualizados.size()) {
        nuevoRol.nombreGuia = guiasActualizados[i];
    } else {
        nuevoRol.nombreGuia = ""; // Guía vacía para salas sin guía
    }
    nuevoRol.nombreSala = salas[i].nombre;
    rolesGenerados.push_back(nuevoRol);
}


    // Ajuste para asegurar que todas las salas obligatorias tengan guía
    for (const auto& salaObligatoria : salas) {
        if (!salaObligatoria.obligatoria) continue;

        // Verificar si ya hay un guía asignado a esta sala
        auto it = std::find_if(rolesGenerados.begin(), rolesGenerados.end(),
                            [&](const GestorDatos::Rol& r) {
                                return r.nombreSala == salaObligatoria.nombre;
                            });

        if (it == rolesGenerados.end()) {
            // Sala obligatoria sin guía, buscar una sala no obligatoria para mover su guía
            auto itNoObligatoria = std::find_if(rolesGenerados.begin(), rolesGenerados.end(),
                                                [&](const GestorDatos::Rol& r) {
                                                    auto salaIt = std::find_if(salas.begin(), salas.end(),
                                                        [&](const GestorDatos::Sala& s) {
                                                            return s.nombre == r.nombreSala;
                                                        });
                                                    return salaIt != salas.end() && !salaIt->obligatoria;
                                                });
            if (itNoObligatoria != rolesGenerados.end()) {
                // Mover el guía de la sala no obligatoria a la obligatoria
                itNoObligatoria->nombreSala = salaObligatoria.nombre;
            } else {
                // Si no hay ninguna sala no obligatoria con guía, simplemente no asignar
                std::cerr << "[ADVERTENCIA] No hay guías disponibles para cubrir la sala obligatoria '"
                        << salaObligatoria.nombre << "'.\n";
            }
        }
    }

    // Guardar los roles generados
    this->rolesGenerados = rolesGenerados;
}



/**
 * @brief Reasigna guías entre salas si no cumplen requisitos.
 */
void GeneradorRol::ReasignarGuias(GestorDatos::Rol& rol1, GestorDatos::Rol& rol2) {
    // MEJORAR
     std::swap(rol1.nombreGuia, rol2.nombreGuia);
}

/**
 * @brief Verifica que cada guía esté asignado a una sala compatible.
 */
std::vector<GestorDatos::Rol> GeneradorRol::ComprobarAsignacion(
    const std::vector<GestorDatos::Guia>& guias,
    const std::vector<GestorDatos::Sala>& salas,
    const std::vector<GestorDatos::Rol>& roles)
{
    std::vector<GestorDatos::Rol> invalidos;

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
 * @brief Genera un conjunto de roles válidos para el día.
 */
std::vector<GestorDatos::Rol> GeneradorRol::generarRoles(const GestorDatos& datos) {
    // Generar los roles asignando guías a salas
    AsignarGuias(datos.guias, datos.salas, datos.roles, 3);

    // Retornar el resultado generado
    return this->rolesGenerados;
}