#ifndef GENERADORROL_HPP
#define GENERADORROL_HPP

#include <vector>
#include <string>
#include "GestorDatos.hpp"

/**
 * @brief Clase encargada de generar los roles diarios entre guías y salas.
 * 
 * La clase GeneradorRol administra la asignación de guías a salas en un día de trabajo.
 * 
 * - Verifica que existan suficientes guías disponibles para cubrir las salas.
 * - Asigna guías a salas siguiendo una rotación o aleatoriedad.
 * - Valida restricciones de capacitación y turnos.
 * - Permite consultar los roles generados.
 */
class GeneradorRol {
private:
    /** @brief Lista interna con los roles generados. */
    std::vector<GestorDatos::Rol> rolesGenerados;

    /**
     * @brief Valida que existan suficientes guías para cubrir las salas requeridas.
     * 
     * @param guias Vector con los guías disponibles.
     * @param salas Vector con las salas que deben cubrirse.
     * @return true si la cantidad de guías es suficiente, false en caso contrario.
     */
    bool ValidarDisponibilidad(const std::vector<GestorDatos::Guia>& guias,
                               const std::vector<GestorDatos::Sala>& salas);

    /**
     * @brief Comprueba si las asignaciones actuales cumplen los requisitos.
     * 
     * @param guias Vector de guías disponibles.
     * @param salas Vector de salas asignadas.
     * @param roles Vector de roles generados.
     * @return Vector con los roles inválidos (vacío si todos son válidos).
     */
    std::vector<GestorDatos::Rol> ComprobarAsignacion(
        const std::vector<GestorDatos::Guia>& guias,
        const std::vector<GestorDatos::Sala>& salas,
        const std::vector<GestorDatos::Rol>& roles);

    /**
     * @brief Busca guías que puedan ser asignados a una sala específica.
     * 
     * @param sala Sala que requiere un guía.
     * @param guias Lista completa de guías.
     * @return Vector con los guías que cumplen los requisitos.
     */
    std::vector<GestorDatos::Guia> BuscarGuiasValidos(
        const GestorDatos::Sala& sala,
        const std::vector<GestorDatos::Guia>& guias);

    /**
     * @brief Genera nuevas asignaciones entre guías y salas.
     * 
     * Aplica una lógica de rotación distribuir el personal.
     */
    void AsignarGuias(const std::vector<GestorDatos::Guia>& guias,
                      const std::vector<GestorDatos::Sala>& salas,
                      const std::vector<GestorDatos::Rol>& roles,
                      const int cantidadRotacion);

    /**
     * @brief Reasigna dos guías intercambiando sus salas.
     * 
     * @param rol1 Primer rol a intercambiar.
     * @param rol2 Segundo rol a intercambiar.
     */
    void ReasignarGuias(GestorDatos::Rol& rol1, GestorDatos::Rol& rol2);

public:
    /**
     * @brief Constructor por defecto del generador de roles.
     */
    GeneradorRol() = default;

    /**
     * @brief Genera los roles del día actual usando los datos del sistema.
     * 
     * @param datos Gestor que contiene toda la información del sistema.
     * @return Vector con los roles generados válidos.
     */
    std::vector<GestorDatos::Rol> generarRoles(const GestorDatos& datos);
};

#endif // GENERADORROL_HPP
