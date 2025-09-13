#ifndef GENERADORROL_HPP
#define GENERADORROL_HPP

#include "Guia.hpp"
#include "Sala.hpp"
#include "Rol.hpp"
#include <vector>
#include <string>

/**
 * @brief Clase encargada de generar los roles diarios entre guías y salas.
 * 
 * La clase GeneradorRol administra la asignación de guías a salas en un día de trabajo.
 * 
 * - Verifica que existan suficientes guías disponibles para cubrir las salas 
 *   que no pueden quedar cerradas ni solas.
 * - Asigna guías a salas de forma aleatoria o siguiendo una rotación entre días.
 * - Reasigna guías en caso de que no cumplan con los requisitos de capacitación o turno.
 * - Permite consultar los roles generados en un día.
 */
class GeneradorRol {
private:
    /// Lista de guías disponibles para asignación (punteros a instancias existentes).
    std::vector<Guia*> guias;

    /// Lista de salas a cubrir en la jornada (punteros a instancias existentes).
    std::vector<Sala*> salas;

    /// Roles generados en la asignación (cada uno relaciona un guía con una sala).
    std::vector<Rol> roles;

    /// Contador del día actual (útil para aplicar rotaciones de asignación).
    int diaActual = 0;

    /**
     * @brief Valida que existan suficientes guías para cubrir las salas críticas.
     * 
     * Las salas críticas son aquellas que no son cerrables ni seguras.
     * 
     * @return true si la cantidad de guías es suficiente, false en caso contrario.
     */
    bool ValidarDisponibilidad();

    /**
     * @brief Comprueba que la asignación actual cumpla con los requisitos de cada sala.
     * 
     * Revisa restricciones de capacitación y turnos.
     * 
     * @return true si todas las asignaciones son válidas, false si alguna sala queda mal asignada.
     */
    bool ComprobarAsignacion();

    /**
     * @brief Busca guías que puedan ser asignados a salas específicas.
     * 
     * @return Un vector con los guías que cumplen los requisitos.
     */
    std::vector<Guia*> BuscarGuiasValidos();

    /**
     * @brief Asigna guías a salas según la lógica definida.
     * 
     * - Si es el primer día, la asignación se hace de forma aleatoria.
     * - En días posteriores, se rota a los guías bajándolos 3 posiciones.
     */
    void AsignarGuia();

    /**
     * @brief Intercambia las salas asignadas entre dos guías.
     * 
     * @param g1 Puntero al primer guía.
     * @param g2 Puntero al segundo guía.
     */
    void ReasignarGuias(Guia* g1, Guia* g2);

public:
    /**
     * @brief Constructor de la clase GeneradorRol.
     * 
     * @param guias Vector de punteros a guías disponibles.
     * @param salas Vector de punteros a salas que deben cubrirse.
     */
    GeneradorRol(const std::vector<Guia*>& guias, const std::vector<Sala*>& salas);

    /**
     * @brief Genera los roles del día actual.
     * 
     * @return Un vector de roles válidos que relacionan guías y salas.
     */
    std::vector<Rol> generarRoles();
};

#endif
