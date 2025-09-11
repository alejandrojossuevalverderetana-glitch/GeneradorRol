#ifndef GUIA_HPP
#define GUIA_HPP
#include <string>

/**
 * @brief Representa un guía con sus datos y capacitaciones.
 * 
 * La clase Guia almacena información básica de un guía, incluyendo su nombre, 
 * ID, turno de trabajo y capacitaciones disponibles.
 */
class Guia{
    private:
    /// Nombre del guía
    std::string nombre;

    /// Identificador único del guía
    int id;

    /// Indica si el guía tiene capacitación en radio
    bool capacitacion_radio;

    /// Indica si el guía tiene capacitación en Steam
    bool capacitacion_steam;

    /// Indica si el guía tiene capacitación en televisión
    bool capacitacion_tele;

    /// Turno de trabajo del guía: "Fines", "Manana", "Tarde", "FullTime" o "FullTimeFines"
    std::string turno;



public:
    /**
     * @brief Constructor de la clase Guia
     * 
     * @param nombre Nombre del guía
     * @param id Identificador único
     * @param turno Turno de trabajo
     */
    Guia(std::string nombre, int id, std::string turno);

    // Getters

    /**
     * @brief Obtiene el nombre del guía
     * @return std::string Nombre
     */
    std::string getNombre() const;
/**
     * @brief Obtiene el ID del guía
     * @return int Identificador
     */
    int getId() const;

    /**
     * @brief Verifica si el guía tiene capacitación en radio
     * @return true si tiene capacitación, false en caso contrario
     */
    bool tieneCapacitacionRadio() const;

    /**
     * @brief Verifica si el guía tiene capacitación en Steam
     * @return true si tiene capacitación, false en caso contrario
     */
    bool tieneCapacitacionSteam() const;

    /**
     * @brief Verifica si el guía tiene capacitación en televisión
     * @return true si tiene capacitación, false en caso contrario
     */
    bool tieneCapacitacionTele() const;

    /**
     * @brief Obtiene el turno del guía
     * @return std::string Turno
     */
    std::string getTurno() const;

    // Setters

    /**
     * @brief Cambia el nombre del guía
     * @param nuevoNombre Nuevo nombre
     */
    void setNombre(const std::string& nuevoNombre);

    /**
     * @brief Cambia el ID del guía
     * @param nuevoId Nuevo ID
     */
    void setId(int nuevoId);

    /**
     * @brief Establece si el guía tiene capacitación en radio
     * @param valor true si tiene capacitación, false en caso contrario
     */
    void setCapacitacionRadio(bool valor);

    /**
     * @brief Establece si el guía tiene capacitación en Steam
     * @param valor true si tiene capacitación, false en caso contrario
     */
    void setCapacitacionSteam(bool valor);

    /**
     * @brief Establece si el guía tiene capacitación en televisión
     * @param valor true si tiene capacitación, false en caso contrario
     */
    void setCapacitacionTele(bool valor);

    /**
     * @brief Cambia el turno del guía
     * @param horas Nuevo turno
     */
    void setTurno(std::string horas);
};

#endif
    



