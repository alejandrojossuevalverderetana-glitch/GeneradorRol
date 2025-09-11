#ifndef GESTOR_DATOS_HPP
#define GESTOR_DATOS_HPP
#include "Guia.hpp"
#include "Sala.hpp"

#include <string>
#include <vector>
/**
 * @brief Se encarga de toda la gestion de datos almacenada fuera del programa
 * 
 * Carga los datos de los guias y de las salas desde una base de datos externa
 * o desde un archivo, además de encargarse de guardarlos en el mismo lugar tambien
 */
class Gestor{
    private:
        /// Tipo de almacenamiento disponible
        enum class TipoAlmacenamiento { Archivos, BaseDatos };
        TipoAlmacenamiento tipoAlmacenamiento; 

        /// Datos para almacenamiento en archivos
        struct ConfigArchivo {
            std::string nombreArchivo;
        } configArchivo;  

        /// Datos para almacenamiento en base de datos
        struct ConfigBaseDatos {
            std::string host;
            std::string usuario;
            std::string contrasena;
            std::string nombreBD;
        } configBaseDatos;  

    public:
        /**
         * @brief Constructor del Gestor para trabajar con archivos.
         *
         * Inicializa el gestor utilizando un archivo como medio de almacenamiento.
         *
         * @param archivo Nombre del archivo desde donde se obtendrán los datos.
         */
        Gestor(const std::string& archivo) {
            tipoAlmacenamiento = TipoAlmacenamiento::Archivos;
            configArchivo.nombreArchivo = archivo;
        }

        /**
         * @brief Constructor del Gestor para trabajar con base de datos.
         *
         * Inicializa el gestor utilizando una base de datos como medio de almacenamiento.
         *
         * @param host Dirección del servidor de la base de datos.
         * @param usuario Nombre de usuario para acceder a la base de datos.
         * @param contrasena Contraseña correspondiente al usuario.
         * @param nombreBD Nombre de la base de datos a la cual se conectará el gestor.
         */
        Gestor(const std::string& host, const std::string& usuario,
            const std::string& contrasena, const std::string& nombreBD) {
            tipoAlmacenamiento = TipoAlmacenamiento::BaseDatos;
            configBaseDatos.host = host;
            configBaseDatos.usuario = usuario;
            configBaseDatos.contrasena = contrasena;
            configBaseDatos.nombreBD = nombreBD;
        }
        /**
         * @brief Carga la lista de guías desde la fuente de datos.
         *
         * Dependiendo de la implementación, puede leer desde un archivo
         * (por ejemplo .xlsx) o desde una base de datos.
         *
         * @return std::vector<Guia> Lista de guías cargadas.
         */
        std::vector<Guia> CargarGuias();

        /**
         * @brief Carga la lista de salas desde la fuente de datos.
         *
         * Dependiendo de la implementación, puede leer desde un archivo
         * (por ejemplo .xlsx) o desde una base de datos.
         *
         * @return std::vector<Sala> Lista de salas cargadas.
         */
        std::vector<Sala> CargarSalas();

        /**
         * @brief Guarda la lista de guías en la fuente de datos.
         *
         * Persiste la información actualizada de los guías. 
         * La implementación concreta puede ser en archivo o base de datos.
         *
         * @param guias Vector de guías a guardar.
         */
        void GuardarGuias(const std::vector<Guia>& guias);

        /**
         * @brief Guarda la lista de salas en la fuente de datos.
         *
         * Persiste la información actualizada de las salas. 
         * La implementación concreta puede ser en archivo o base de datos.
         *
         * @param salas Vector de salas a guardar.
         */
        void GuardarSalas(const std::vector<Sala>& salas);


    };

#endif