#ifndef INTERFAZGRAFICA_HPP
#define INTERFAZGRAFICA_HPP

#include "Guia.hpp"
#include "Sala.hpp"
#include "Rol.hpp"
#include "GeneradorRol.hpp"
#include "GestorDatos.hpp"
#include <vector>
#include <string>

/**
 * @brief Clase encargada de manejar la interfaz gráfica de usuario.
 * 
 * La clase InterfazGrafica permite al usuario:
 * - Gestionar guías (listar, buscar, agregar, eliminar).
 * - Gestionar salas (listar, agregar, eliminar).
 * - Aplicar cambios de turnos entre guías en un día específico.
 * - Generar y mostrar roles mediante el GeneradorRol.
 */
class InterfazGrafica {
private:
    /// Referencia al gestor de datos para cargar y guardar la información.
    GestorDatos* gestor;

    /// Instancia del generador de roles.
    GeneradorRol* generador;

public:
    /**
     * @brief Constructor de la interfaz gráfica.
     * 
     * @param gestor Puntero al gestor de datos.
     * @param generador Puntero al generador de roles.
     */
    InterfazGrafica(GestorDatos* gestor, GeneradorRol* generador);

    // -------------------------------
    // Gestión de guías
    // -------------------------------

    /**
     * @brief Muestra la lista de guías en un scroll.
     */
    void mostrarListaGuias();

    /**
     * @brief Busca guías por nombre y actualiza dinámicamente la vista.
     * 
     * @param nombre Nombre o fragmento de nombre a buscar.
     */
    void buscarGuiaPorNombre(const std::string& nombre);

    /**
     * @brief Agrega un nuevo guía al sistema.
     * 
     * @param guia Puntero al nuevo guía a registrar.
     */
    void agregarGuia(Guia* guia);

    /**
     * @brief Elimina un guía existente.
     * 
     * @param id Identificador único del guía a eliminar.
     */
    void eliminarGuia(int id);

    // -------------------------------
    // Gestión de salas
    // -------------------------------

    /**
     * @brief Muestra la lista de salas en un scroll.
     */
    void mostrarListaSalas();

    /**
     * @brief Agrega una nueva sala al sistema.
     * 
     * @param sala Puntero a la nueva sala a registrar.
     */
    void agregarSala(Sala* sala);

    /**
     * @brief Elimina una sala existente.
     * 
     * @param id Identificador único de la sala a eliminar.
     */
    void eliminarSala(int id);

    // -------------------------------
    // Gestión de cambios por día
    // -------------------------------

    /**
     * @brief Aplica un cambio de turno entre dos guías para un día específico.
     * 
     * @param dia Número del día en el calendario.
     * @param g1 Puntero al primer guía (turno mañana).
     * @param g2 Puntero al segundo guía (turno tarde).
     */
    void aplicarCambioTurno(int dia, Guia* g1, Guia* g2);

    // -------------------------------
    // Generación de roles
    // -------------------------------

    /**
     * @brief Genera los roles del día actual y los muestra en pantalla.
     */
    void generarYMostrarRol();
};

#endif
