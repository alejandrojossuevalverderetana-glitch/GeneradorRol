#include "GeneradorRol.hpp"
#include <algorithm> 
#include <random>    
#include <iostream>   

// ----------------- Constructor -----------------
GeneradorRol::GeneradorRol(const std::vector<Guia*>& guias, const std::vector<Sala*>& salas)
    : guias(guias), salas(salas), diaActual(0) {}

// ----------------- Validar disponibilidad -----------------
bool GeneradorRol::ValidarDisponibilidad() {
    std::vector<Sala*> salasObligatorias;
    for (Sala* sala : salas) {
    if (!sala->esCerrable() && !sala->esSegura()) {
        salasObligatorias.push_back(sala);
    }
}
    return guias.size() >= salasObligatorias.size();
}

// ----------------- Comprobar asignación -----------------
 std::vector<Rol> GeneradorRol::ComprobarAsignacion() {
    std::vector<Rol> rolRestriccion;
     for (Rol rol : roles) {
        

        Sala* sala = rol.getSala();
        Guia* guia = rol.getGuia();

        if (!sala || !guia) continue;

        
        if (sala->getRestriccionTurno() && guia->getTurno() == "fines") {
            rolRestriccion.push_back(rol);
        }
    }

    return rolRestriccion;
}
    


// ----------------- Buscar guías válidos -----------------
std::vector<Guia*> GeneradorRol::BuscarGuiasValidos(Sala* sala) {
    // En MVP simplemente retornamos todos los guías disponibles
    return guias;
}

// ----------------- Asignar guías -----------------
void GeneradorRol::AsignarGuia() {
    // Barajar los guías aleatoriamente para el día actual
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(guias.begin(), guias.end(), g);

    roles.clear();
    size_t n = std::min(guias.size(), salas.size());
    for (size_t i = 0; i < n; ++i) {
        roles.push_back(Rol(guias[i], salas[i]));
    }
}

// ----------------- Reasignar guías -----------------
void GeneradorRol::ReasignarGuias(Guia* g1, Guia* g2) {
    // Busca los roles donde están los guías y los intercambia
    for (auto& rol : roles) {
        if (rol.getGuia() == g1) rol.setGuia(g2);
        else if (rol.getGuia() == g2) rol.setGuia(g1);
    }
}

// ----------------- Generar roles -----------------
std::vector<Rol> GeneradorRol::generarRoles() {
     

    if (!ValidarDisponibilidad()) {
        std::cerr << "No hay suficientes guías para cubrir las salas." << std::endl;
        roles.clear();
        return roles;
    }

    AsignarGuia();
    std::vector<Rol> rolRestriccion = ComprobarAsignacion();
    std::random_device rd;
    std::mt19937 gen(rd());  
    if (rolRestriccion.size() > 0) {
        for (const Rol& rol : rolRestriccion) {
            Sala* sala = rol.getSala();
            Guia* guia = rol.getGuia();
            std::vector<Guia*> guiasValidos = BuscarGuiasValidos(sala);
                if (!guiasValidos.empty()) {

                std::uniform_int_distribution<> dist(0, guiasValidos.size() - 1);

                int indiceAleatorio = dist(gen);
                Guia* guiaSeleccionado = guiasValidos[indiceAleatorio];
                    ReasignarGuias(guia, guiaSeleccionado);
                }
    }    
        
    
    }
    ++diaActual;
    return roles;
}
