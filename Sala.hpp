#ifndef SALA_HPP
#define SALA_HPP

#include <string>

class Sala {
private:
    std::string nombre;
    int id;
    bool requiereCapacitacion;
    bool restriccionHoras;
    bool segura;
    bool cerrable;
    int horaAlmuerzo; // Puede ser 12, 13 o 14

public:
    // Constructor
    Sala(std::string nombre, int id);

    // Getters
    std::string getNombre() const;
    int getId() const;
    bool getRequiereCapacitacion() const;
    bool getRestriccionHoras() const;
    bool esSegura() const;
    bool esCerrable() const;
    int getHoraAlmuerzo() const;

    // Setters
    void setNombre(const std::string& nuevoNombre);
    void setId(int nuevoId);
    void setRequiereCapacitacion(bool valor);
    void setRestriccionHoras(bool valor);
    void setSegura(bool valor);
    void setCerrable(bool valor);
    void setHoraAlmuerzo(int hora);
};

#endif
