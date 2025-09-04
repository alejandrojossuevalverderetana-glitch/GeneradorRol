#include "GeneradorRol.hpp"
#include <fstream>
#include <algorithm>
#include <random>
#include <ctime>

GeneradorRol::GeneradorRol(const std::vector<Guia>& guias, const std::vector<Sala>& salas)
    : guias(guias), salas(salas)
{
    // Inicializar orden de guías en orden natural
    for (int i = 0; i < guias.size(); ++i) {
        ordenGuias.push_back(i);
    }
}

void GeneradorRol::generarPrimerDia() {
    std::shuffle(ordenGuias.begin(), ordenGuias.end(), std::default_random_engine(std::time(0)));
    asignarAlmuerzos();
    ++diaActual;
}

void GeneradorRol::generarDiaSiguiente() {
    // Rotar todos los guías 3 posiciones
    std::rotate(ordenGuias.begin(), ordenGuias.begin() + 3, ordenGuias.end());
    asignarAlmuerzos();
    ++diaActual;
}

void GeneradorRol::asignarAlmuerzos() {
    // Aquí haces asignación de guías a salas según las reglas

    std::vector<bool> usados(guias.size(), false);

    for (Sala& sala : salas) {
        Guia guiaAsignado = seleccionarGuiaValido(sala, usados);
        // Enlazar esa sala con ese guía en una estructura que puedas luego exportar
        // Por ejemplo, podrías guardar en un `std::map<int, int>` (idSala -> idGuia)
    }

    // Asignar horas de almuerzo a las salas (balanceado)
    // Puedes contar cuántas hay por hora y distribuir (12, 13, 14)
}

bool GeneradorRol::guiaCumple(const Guia& guia, const Sala& sala) {
    if (sala.getRequiereCapacitacion()) {
        if (sala.getNombre() == "Radio" && !guia.tieneCapacitacionRadio()) return false;
        if (sala.getNombre() == "Steam" && !guia.tieneCapacitacionSteam()) return false;
        if (sala.getNombre() == "Tele" && !guia.tieneCapacitacionTele()) return false;
    }

    if (sala.getRestriccionHoras()) {
        if (guia.getHorasTrabajo() == 17) return false;
    }

    return true;
}

Guia GeneradorRol::seleccionarGuiaValido(const Sala& sala, std::vector<bool>& usados) {
    std::vector<int> candidatos;

    for (int i = 0; i < ordenGuias.size(); ++i) {
        int idx = ordenGuias[i];
        if (!usados[idx] && guiaCumple(guias[idx], sala)) {
            candidatos.push_back(idx);
        }
    }

    if (!candidatos.empty()) {
        int elegido = candidatos[rand() % candidatos.size()];
        usados[elegido] = true;
        return guias[elegido];
    }

    // Si no hay nadie disponible, devuelves alguno por defecto o lanzas error
    return guias[0]; // Fallback de emergencia
}

void GeneradorRol::guardarRolEnArchivo(const std::string& nombreArchivo) {
    std::ofstream archivo(nombreArchivo);

    if (!archivo.is_open()) return;

    archivo << "Rol del día " << diaActual << "\n";

    // Escribir la asignación real aquí (según cómo decidas guardarla)
    // archivo << "Sala 1: Guia Alejandro\n";

    archivo.close();
}
