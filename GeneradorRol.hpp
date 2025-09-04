#ifndef GENERADORROL_HPP
#define GENERADORROL_HPP

#include "Guia.hpp"
#include "Sala.hpp"
#include <vector>
#include <map>
#include <string>

class GeneradorRol {
private:
    std::vector<Guia> guias;
    std::vector<Sala> salas;
    int diaActual = 0;

    // Historial de asignaciones para rotar
    std::vector<int> ordenGuias; 

public:
    GeneradorRol(const std::vector<Guia>& guias, const std::vector<Sala>& salas);

    void generarPrimerDia();
    void generarDiaSiguiente();
    void guardarRolEnArchivo(const std::string& nombreArchivo);

private:
    void asignarAlmuerzos();
    bool guiaCumple(const Guia& guia, const Sala& sala);
    Guia seleccionarGuiaValido(const Sala& sala, std::vector<bool>& usados);
};

#endif
