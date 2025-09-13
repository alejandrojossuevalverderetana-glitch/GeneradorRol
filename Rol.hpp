#ifndef ROL_HPP
#define ROL_HPP

#include "Guia.hpp"
#include "Sala.hpp"

/**
 * @brief Representa un rol que asocia un guía con una sala.
 * 
 * La clase Rol almacena la relación entre un guía y una sala mediante punteros,
 * lo cual permite manejar asignaciones dinámicas sin duplicar objetos.
 */
class Rol {
private:
    /// Puntero al guía asignado
    Guia* guia;

    /// Puntero a la sala asignada
    Sala* sala;

public:
    /**
     * @brief Constructor de la clase Rol.
     * 
     * @param guia Puntero a un guía
     * @param sala Puntero a una sala
     */
    Rol(Guia* guia, Sala* sala);

    // Getters

    /**
     * @brief Obtiene el guía asignado al rol.
     * @return Puntero al guía
     */
    Guia* getGuia() const;

    /**
     * @brief Obtiene la sala asignada al rol.
     * @return Puntero a la sala
     */
    Sala* getSala() const;

    // Setters

    /**
     * @brief Cambia el guía asignado al rol.
     * @param nuevoGuia Nuevo puntero al guía
     */
    void setGuia(Guia* nuevoGuia);

    /**
     * @brief Cambia la sala asignada al rol.
     * @param nuevaSala Nuevo puntero a la sala
     */
    void setSala(Sala* nuevaSala);
};

#endif
