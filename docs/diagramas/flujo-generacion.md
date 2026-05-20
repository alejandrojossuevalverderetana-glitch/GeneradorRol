# Flujo de generacion

## Flujo principal del backend

```mermaid
flowchart TD
    A[Recibir JSON] --> B[Construir GestorDatos]
    B --> C[AsignarGuias]
    C --> D[ComprobarAsignacion]
    D --> E{Hay asignaciones invalidas?}
    E -- No --> H[Devolver roles]
    E -- Si --> F[AplicarCambiosInternos]
    F --> G[SegundaRevisionCambiosInternos]
    G --> H[Devolver roles]
```

## Flujo de decision por cada sala

```mermaid
flowchart TD
    A[Tomar rol o sala] --> B{Tiene guia asignado?}
    B -- No --> C{Sala obligatoria o especial activa?}
    C -- Si --> D[Marcar invalido]
    C -- No --> Z[Continuar]
    B -- Si --> E[Buscar sala correspondiente]
    E --> F{Sala requiere capacitacion?}
    F -- No --> Z
    F -- Si --> G{Guia tiene capacitacion?}
    G -- Si --> Z
    G -- No --> D
```

## Resumen funcional

1. Se genera una rotacion base.
2. Se valida cada asignacion.
3. Si una sala queda mal cubierta, se busca un reemplazo valido.
4. El cambio interno queda registrado sin perder trazabilidad del rol original.
