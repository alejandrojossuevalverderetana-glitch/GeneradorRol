#include "Guia.hpp"

// Constructor
Guia::Guia(std::string nombre, int id) :
      nombre(nombre), id(id),
      capacitacion_radio(false), capacitacion_steam(false), capacitacion_tele(false),
      horas_trabajo(0) {}

// Getters
std::string Guia::getNombre() const {
    return nombre;
}

int Guia::getId() const {
    return id;
}

bool Guia::tieneCapacitacionRadio() const {
    return capacitacion_radio;
}

bool Guia::tieneCapacitacionSteam() const {
    return capacitacion_steam;
}

bool Guia::tieneCapacitacionTele() const {
    return capacitacion_tele;
}

int Guia::getHorasTrabajo() const {
    return horas_trabajo;
}

// Setters
void Guia::setNombre(const std::string& nuevoNombre) {
    nombre = nuevoNombre;
}

void Guia::setId(int nuevoId) {
    id = nuevoId;
}

void Guia::setCapacitacionRadio(bool valor) {
    capacitacion_radio = valor;
}

void Guia::setCapacitacionSteam(bool valor) {
    capacitacion_steam = valor;
}

void Guia::setCapacitacionTele(bool valor) {
    capacitacion_tele = valor;
}

void Guia::setHorasTrabajo(int horas) {
    horas_trabajo = horas;
}
