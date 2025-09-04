#include "Sala.hpp"

// Constructor
Sala::Sala(std::string nombre, int id)
    : nombre(nombre), id(id),
      requiereCapacitacion(false), restriccionHoras(false),
      segura(false), cerrable(false), horaAlmuerzo(12) {}

// Getters
std::string Sala::getNombre() const {
    return nombre;
}

int Sala::getId() const {
    return id;
}

bool Sala::getRequiereCapacitacion() const {
    return requiereCapacitacion;
}

bool Sala::getRestriccionHoras() const {
    return restriccionHoras;
}

bool Sala::esSegura() const {
    return segura;
}

bool Sala::esCerrable() const {
    return cerrable;
}

int Sala::getHoraAlmuerzo() const {
    return horaAlmuerzo;
}

// Setters
void Sala::setNombre(const std::string& nuevoNombre) {
    nombre = nuevoNombre;
}

void Sala::setId(int nuevoId) {
    id = nuevoId;
}

void Sala::setRequiereCapacitacion(bool valor) {
    requiereCapacitacion = valor;
}

void Sala::setRestriccionHoras(bool valor) {
    restriccionHoras = valor;
}

void Sala::setSegura(bool valor) {
    segura = valor;
}

void Sala::setCerrable(bool valor) {
    cerrable = valor;
}

void Sala::setHoraAlmuerzo(int hora) {
    horaAlmuerzo = hora;
}
