#include "Rol.hpp"

// ----------------- Constructor -----------------
Rol::Rol(Guia* guia, Sala* sala) : guia(guia), sala(sala) {}

// ----------------- Getters -----------------
Guia* Rol::getGuia() const {
    return guia;
}

Sala* Rol::getSala() const {
    return sala;
}

// ----------------- Setters -----------------
void Rol::setGuia(Guia* nuevoGuia) {
    guia = nuevoGuia;
}

void Rol::setSala(Sala* nuevaSala) {
    sala = nuevaSala;
}
