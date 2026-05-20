# Modelo de datos y ERD logico

## Aclaracion

Hoy no existe una base de datos relacional implementada en el repo. Aun asi, si se quisiera persistir el sistema, este es el modelo logico que surge del codigo actual.

## ERD logico

```mermaid
erDiagram
    GUIA {
        string nombre PK
        string turno
    }

    CAPACITACION {
        string nombre PK
    }

    SALA {
        string nombre PK
        string capacitacion FK
        boolean obligatoria
        int numero
    }

    ROL_PREVIO {
        string nombreGuia FK
        string nombreSala FK
        string turno
    }

    ROL_GENERADO {
        string nombreGuia FK
        string nombreSala FK
        string nombreCambioInterno
        string nombreCambioAprobado
        string turno
        date fecha
    }

    CAMBIO_EXTERNO {
        string guiaManana FK
        string guiaTarde FK
    }

    CONFIG_DIA {
        string turno
        int valorRotacion
        string operador1 FK
        string operador2 FK
        string vacacion1 FK
        string vacacion2 FK
    }

    GUIA ||--o{ ROL_PREVIO : ocupa
    GUIA ||--o{ ROL_GENERADO : ocupa
    SALA ||--o{ ROL_PREVIO : define
    SALA ||--o{ ROL_GENERADO : define
    CAPACITACION ||--o{ SALA : requerida_por
    GUIA }o--o{ CAPACITACION : posee
    CONFIG_DIA ||--o{ CAMBIO_EXTERNO : usa
```

## Correspondencia con el codigo

- `Guia`, `Sala`, `Rol`, `RolGenerado`, `Cambio`, `Operadores` y `Vacaciones` viven en [`Backend/GestorDatos.hpp`](../../Backend/GestorDatos.hpp).
- Las salas especiales `operador1`, `operador2`, `vacaciones1` y `vacaciones2` se agregan dinamicamente en [`Backend/GestorDatos.cpp`](../../Backend/GestorDatos.cpp).
- Tambien se agregan salas `fueraRolN` si sobran guias para el turno.

## Implicaciones

- El modelo actual favorece flexibilidad en memoria.
- Si se migra a BD, sera clave separar catalogos, configuracion diaria y resultados historicos.
