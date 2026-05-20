# UML de clases

## Clases principales

```mermaid
classDiagram
    class GestorDatos {
        +vector~Guia~ guias
        +vector~Sala~ salas
        +vector~Rol~ roles
        +vector~Cambio~ cambios
        +Operadores operadores
        +Vacaciones vacaciones
        +string turno
        +int valorRotacion
        +GestorDatos(json AppData)
    }

    class Guia {
        +string nombre
        +string turno
        +vector~string~ capacitaciones
    }

    class Sala {
        +string nombre
        +string capacitacion
        +bool obligatoria
        +int numero
    }

    class Rol {
        +string nombreGuia
        +string nombreSala
    }

    class RolGenerado {
        +string nombreGuia
        +string nombreSala
        +string nombreCambioInterno
        +string nombreCambioAprobado
    }

    class Cambio {
        +string guiaTarde
        +string guiaManana
    }

    class Operadores {
        +string operador1
        +string operador2
    }

    class Vacaciones {
        +string vacacion1
        +string vacacion2
    }

    class GeneradorRol {
        -vector~RolGenerado~ rolesGenerados
        -bool CambioInternoValido(...)
        -bool EsOperadorOVacacion(...)
        -bool EstaEnSalaCorrecta(...)
        -bool ValidarDisponibilidad(...)
        -vector~RolGenerado~ ComprobarAsignacion(...)
        -vector~Guia~ BuscarGuiasValidos(...)
        -void AsignarGuias(...)
        -void AplicarCambiosInternos(...)
        -void SegundaRevisionCambiosInternos(...)
        +static bool GuiaValidoParaTurno(...)
        +vector~RolGenerado~ generarRoles(datos)
    }

    GestorDatos *-- Guia
    GestorDatos *-- Sala
    GestorDatos *-- Rol
    GestorDatos *-- Cambio
    GestorDatos *-- Operadores
    GestorDatos *-- Vacaciones
    GeneradorRol ..> GestorDatos
    GeneradorRol ..> RolGenerado
```

## Lectura rapida

- `GestorDatos` es el punto de entrada del modelo.
- `GeneradorRol` es el servicio de dominio que orquesta la asignacion.
- `RolGenerado` extiende el concepto de `Rol` con trazabilidad de cambios.


