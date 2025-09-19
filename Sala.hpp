#ifndef SALA_HPP
#define SALA_HPP

#include <string>

/**
 * @brief Representa una sala con sus características y restricciones.
 * 
 * La clase Sala almacena información sobre una sala específica, incluyendo 
 * su nombre, ID, si requiere capacitación, restricciones de turno, seguridad, 
 * posibilidad de cerrar y la hora de almuerzo.
 */
class Sala {
private:
    /// Nombre de la sala
    std::string nombre;

    /// Identificador único de la sala
    int id;

    /// Indica si la sala requiere que el personal esté capacitado
    bool requiereCapacitacion;

    /// Indica si hay restricciones de turno para acceder a la sala
    bool restriccionTurno;

    /// Indica si la sala puede quedarse sola durante el almuerzo
    bool segura;

    /// Indica si la sala puede cerrarse durante el almuerzo
    bool cerrable;

    /// Indica si la sala solo abre fines de semana: "Fines", "EntreSemana",
    bool soloAbreFines;

    /// Hora a la que se sale a almorzar: 12, 13 o 14
    int horaAlmuerzo;

public:
    /**
     * @brief Constructor de la clase Sala
     * 
     * @param nombre Nombre de la sala
     * @param id Identificador único
     */
    Sala(std::string nombre, int id);

    // Getters

    /**
     * @brief Obtiene el nombre de la sala
     * @return std::string Nombre
     */
    std::string getNombre() const;

    /**
     * @brief Obtiene el ID de la sala
     * @return int Identificador
     */
    int getId() const;

    /**
     * @brief Verifica si la sala requiere capacitación
     * @return true si requiere capacitación, false en caso contrario
     */
    bool getRequiereCapacitacion() const;

    /**
     * @brief Verifica si hay restricción de turno para la sala
     * @return true si hay restricción, false en caso contrario
     */
    bool getRestriccionTurno() const;

    /**
     * @brief Verifica si la sala se puede dejar sola
     * @return true si es segura, false en caso contrario
     */
    bool esSegura() const;

    /**
     * @brief Verifica si la sala se puede cerrar
     * @return true si es cerrable, false en caso contrario
     */
    bool esCerrable() const;

    /**
     * @brief Verifica si la sala solo abre en fines de semana
     * @return true si solo abre fines de semana, false en caso contrario
     */

    /**
     * @brief Obtiene la hora de almuerzo de la sala
     * @return int Hora de almuerzo
     */
    int getHoraAlmuerzo() const;

    // Setters

    /**
     * @brief Cambia el nombre de la sala
     * @param nuevoNombre Nuevo nombre
     */
    void setNombre(const std::string& nuevoNombre);

    /**
     * @brief Cambia el ID de la sala
     * @param nuevoId Nuevo ID
     */
    void setId(int nuevoId);

    /**
     * @brief Establece si la sala requiere capacitación
     * @param valor true si requiere, false si no
     */
    void setRequiereCapacitacion(bool valor);

    /**
     * @brief Establece si la sala tiene restricción de turno
     * @param valor true si hay restricción, false si no
     */
    void setRestriccionTurno(bool valor);

    /**
     * @brief Establece si la sala se puede dejar sola
     * @param valor true si es segura, false si no
     */
    void setSegura(bool valor);

    /**
     * @brief Establece si la sala se puede cerrar
     * @param valor true si es cerrable, false si no
     */
    void setCerrable(bool valor);

    /**
     * @brief Cambia la hora de almuerzo de la sala
     * @param hora Nueva hora de almuerzo (12, 13 o 14)
     */
    void setHoraAlmuerzo(int hora);
};

#endif
