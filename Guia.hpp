#ifndef GUIA_HPP
#define GUIA_HPP
#include <string>

class Guia{
    private:
    // HORAS_TRABAJO pueden ser 17, 27, 44 que son las jornadas de trabajo que existen
        std::string nombre;
        int id;
        bool capacitacion_radio;
        bool capacitacion_steam;
        bool capacitacion_tele;
        int horas_trabajo; 


public:
    // Constructor
    Guia(std::string nombre, int id);

    // Getters
    std::string getNombre() const;
    int getId() const;
    bool tieneCapacitacionRadio() const;
    bool tieneCapacitacionSteam() const;
    bool tieneCapacitacionTele() const;
    int getHorasTrabajo() const;

    // Setters
    void setNombre(const std::string& nuevoNombre);
    void setId(int nuevoId);
    void setCapacitacionRadio(bool valor);
    void setCapacitacionSteam(bool valor);
    void setCapacitacionTele(bool valor);
    void setHorasTrabajo(int horas);
};
#endif
    



