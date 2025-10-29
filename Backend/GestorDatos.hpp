#ifndef GESTOR_DATOS_HPP
#define GESTOR_DATOS_HPP

#include "httplib.h"
#include <nlohmann/json.hpp>
#include <vector>


/**
 * @brief Clase encargada de gestionar los datos del sistema.
 * 
 * Esta clase se encarga de manejar los datos de guías, salas, roles y cambios,
 * cargándolos desde un archivo JSON externo y manteniéndolos en estructuras
 * de datos fáciles de usar. Los datos son de solo lectura desde fuera de la clase.
 */
class GestorDatos {
public:
    /**
     * @brief Representa un guía con nombre e identificador único.
     */
    struct Guia {
        std::string nombre; /**< Nombre del guía */
        std::string turno;
        std::vector<std::string> capacitaciones; 
    };

    /**
     * @brief Representa una sala con nombre y su capacitacion necesaria.
     */
    struct Sala {
        std::string nombre; /**< Nombre de la sala */
        std::string capacitacion;    
        bool obligatoria;
        int numero;
    };

    /**
     * @brief Representa un rol asignando un guía a una sala.
     */
    struct Rol {
        std::string nombreGuia; /**< Nombre del guía asignado */
        std::string nombreSala; /**< Nombre de la sala asignada */
    };

    /**
     * @brief Representa un cambio de guías entre turnos.
     */
    struct Cambio {
        std::string guiaTarde;  /**< Guía del turno de la tarde */
        std::string guiaManana; /**< Guía del turno de la mañana */
    };

    /**
     * @brief Constructor del GestorDatos.
     * 
     * Inicializa las estructuras internas a partir de un JSON con la información
     * de guías, salas, roles y cambios.
     * 
     * @param AppData JSON que contiene toda la información del sistema.
     */
    GestorDatos(const nlohmann::json& AppData);

    /** @brief Lista de guías cargadas*/
     std::vector<Guia> guias;

    /** @brief Lista de salas cargadas*/
     std::vector<Sala> salas;

    /** @brief Lista de roles cargados*/
     std::vector<Rol> roles;

    /** @brief Lista de cambios cargados*/
     std::vector<Cambio> cambios;
};

#endif // GESTOR_DATOS_HPP
