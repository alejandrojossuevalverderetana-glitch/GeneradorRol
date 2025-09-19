#include "Guia.hpp"

// Constructor
Guia::Guia(std::string nombre, int id, std::string turno) :
      nombre(nombre), id(id),
      capacitacion_radio(false), capacitacion_steam(false), capacitacion_tele(false),
      turno(turno) {}

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

bool Guia::tieneCapacitacionOperador() const {
    return capacitacion_operador;
}

std::string Guia::getTurno() const {
    return turno;
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

void Guia::setCapacitacionOperador(bool valor) {
    capacitacion_operador = valor;
}

void Guia::setTurno(std::string _turno) {
    turno = _turno;
}
