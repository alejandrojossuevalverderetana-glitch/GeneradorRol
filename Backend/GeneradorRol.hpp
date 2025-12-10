#ifndef GENERADORROL_HPP
#define GENERADORROL_HPP

#include <vector>
#include <string>
#include <unordered_set>
#include "GestorDatos.hpp"

/**
 * @brief Clase encargada de generar los roles diarios entre guías y salas.
 * 
 * La clase GeneradorRol administra la asignación de guías a salas en un día de trabajo.
 * 
 * Funcionalidades principales:
 * - Generación de rol inicial con rotación de guías según turno.
 * - Consideración de operadores: los operadores no pueden ser asignados a salas ni usados como reemplazo.
 * - Exclusión automática de operadores en todas las validaciones y búsquedas.
 * - Aplicación de cambios internos cuando un guía no cumple requisitos de capacitación.
 * - Posibilidad de reflejar cambios externos (otros turnos) de manera separada.
 * - Consulta de roles generados.
 */
class GeneradorRol {
private:
    /** @brief Lista interna con los roles generados. */
    std::vector<GestorDatos::RolGenerado> rolesGenerados;

    /**
     * @brief Valida que existan suficientes guías para cubrir las salas requeridas.
     * 
     * La validación considera:
     * - El turno actual (solo se toma en cuenta guías del turno seleccionado).
     * - Exclusión de los operadores, ya que no pueden asignarse a salas.
     * 
     * @param guias Vector con los guías disponibles.
     * @param salas Vector con las salas que deben cubrirse.
     * @param turno String que indica el turno que se va a generar.
     * @return true si la cantidad de guías válidos es suficiente, false en caso contrario.
     */
    bool ValidarDisponibilidad(const std::vector<GestorDatos::Guia>& guias,
                               const std::vector<GestorDatos::Sala>& salas,
                               const std::string& turno );

    /**
     * @brief Comprueba si las asignaciones actuales cumplen los requisitos.
     * 
     * Filtra automáticamente:
     * - Guías que no pertenecen al turno actual.
     * - Operadores, quienes NO pueden ser asignados a ninguna sala.
     * 
     * @param guias Vector de guías disponibles.
     * @param salas Vector de salas asignadas.
     * @param roles Vector de roles generados.
     * @param turno Turno actual utilizado para filtrar guías válidos.
     * @return Vector con los roles inválidos (vacío si todos son válidos).
     */
    std::vector<GestorDatos::RolGenerado> ComprobarAsignacion(
        const std::vector<GestorDatos::Guia>& guias,
        const std::vector<GestorDatos::Sala>& salas,
        const std::vector<GestorDatos::RolGenerado>& roles,
        const GestorDatos::Operadores& operadores,
        const std::string& turno
    );

    /**
     * @brief Busca guías que puedan ser asignados a una sala específica.
     * 
     * La búsqueda:
     * - Excluye automáticamente a los operadores.
     * - Excluye guías que no pertenecen al turno actual.
     * - Considera guías ya asignados para evitar duplicaciones.
     * 
     * @param sala Sala que requiere un guía.
     * @param guias Lista completa de guías.
     * @param rolesGenerados Roles ya generados para considerar asignaciones previas.
     * @param operadores Conjunto de operadores que deben excluirse de la asignación.
     * @param salas Lista completa de salas para verificar requisitos.
     * @return Vector con los guías válidos para la sala.
     */
    std::vector<GestorDatos::Guia> BuscarGuiasValidos(
        const GestorDatos::Sala& sala,
        const std::vector<GestorDatos::Guia>& guias,
        const std::vector<GestorDatos::RolGenerado>& rolesGenerados,
        const GestorDatos::Operadores& operadores,
        const std::vector<GestorDatos::Sala>& salas,
        const std::string& turno);

    /**
     * @brief Genera la asignación inicial de guías a salas.
     * 
     * Consideraciones nuevas:
     * - Los operadores no pueden ser asignados ni contados en la rotación.
     * - Solo se incluyen guías del turno actual en el proceso.
     * 
     * Lógica:
     * 1) Construcción del vector con la posición actual de cada guía según el rol previo.
     * 2) Rotación de guías según cantidadRotacion.
     * 3) Exclusión de operadores y guías fuera del turno.
     * 4) Asignación final respetando requisitos de cada sala.
     * 
     * @param guias Vector de guías disponibles.
     * @param salas Vector de salas.
     * @param roles Roles previos para considerar rotación.
     * @param cantidadRotacion Cantidad de posiciones a rotar.
     * @param operadores Conjunto de operadores que deben excluirse.
     * @param turno Turno para filtrar guías disponibles ("manana", "tarde", etc.).
     */
    void AsignarGuias(const std::vector<GestorDatos::Guia>& guias,
                      const std::vector<GestorDatos::Sala>& salas,
                      const std::vector<GestorDatos::Rol>& roles,
                      const int cantidadRotacion,
                      const GestorDatos::Operadores& operadores,
                      const std::string& turno);

    /**
     * @brief Reasigna guías a salas si no cumplen requisitos de capacitación.
     * 
     * Los operadores también se excluyen aquí:
     * - No pueden ser movidos entre salas.
     * - No pueden ser usados como reemplazo.
     * 
     * Aplica cambios internos:
     * - Se mueven guías de salas no críticas a salas obligatorias.
     * - Se intercambian guías sin capacitación con guías válidos.
     */
    void AplicarCambiosInternos(
        const std::vector<GestorDatos::Guia>& guias,
        const std::vector<GestorDatos::Sala>& salas,
        const GestorDatos::Operadores& operadores,
        const std::string& turno);

public:
    /** @brief Constructor por defecto. */
    GeneradorRol() = default;

    /**
     * @brief Genera los roles del día actual usando los datos del sistema.
     * 
     * Flujo general:
     * 1) Generación del rol desplazado según rotación y turno.
     * 2) Exclusión de operadores durante todas las fases.
     * 3) Aplicación de cambios internos (capacitación, salas obligatorias).
     * 4) Posibilidad de reflejar cambios externos (otros turnos) si es necesario.
     * 
     * @param datos Gestor que contiene toda la información de guías, salas, operadores y roles previos.
     * @return Vector con los roles generados válidos para el día.
     */
    std::vector<GestorDatos::RolGenerado> generarRoles(const GestorDatos& datos);
};

#endif // GENERADORROL_HPP