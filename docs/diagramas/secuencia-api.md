# Secuencia API

## Secuencia entre frontend y backend

```mermaid
sequenceDiagram
    actor Admin
    participant UI as Frontend
    participant LS as localStorage
    participant API as API /generar
    participant GD as GestorDatos
    participant GR as GeneradorRol

    Admin->>UI: Configura guias, salas, turno, operadores, vacaciones
    UI->>LS: Guarda estado local
    Admin->>UI: Solicita generar rol
    UI->>API: POST /generar con JSON
    API->>GD: Parsear y estructurar datos
    GD-->>API: Estructuras de dominio
    API->>GR: generarRoles(datos)
    GR-->>API: vector<RolGenerado>
    API-->>UI: JSON con roles
    UI->>UI: Renderizar tabla
    Admin->>UI: Exportar CSV
```

## Notas

- El frontend mantiene memoria local de roles previos por turno.
- El backend no persiste informacion entre solicitudes.
- El contrato entre ambas capas es enteramente JSON.
