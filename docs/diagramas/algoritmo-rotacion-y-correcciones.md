# Algoritmo de rotacion y correcciones

## Objetivo

Generar un rol diario que conserve la logica de rotacion del dia anterior, pero que tambien respete restricciones operativas.

## Paso 1: normalizacion de datos

`GestorDatos`:

- Carga guias, salas, roles previos y cambios.
- Anade salas especiales para operadores y vacaciones.
- Anade salas `fueraRolN` si hay mas guias validos que salas.

## Paso 2: filtrado por turno

`GeneradorRol::GuiaValidoParaTurno(...)` decide si un guia participa en el turno actual mediante reglas de compatibilidad entre turnos.

Objetivo:

- Permitir compatibilidad entre variantes como `manana`, `tarde`, `entresemana manana`, `entresemana tarde` y `fines`.

## Paso 3: rotacion base

`AsignarGuias(...)` hace lo siguiente:

1. Filtra guias validos para el turno.
2. Ubica las posiciones previas por sala.
3. Aplica una rotacion modular con `cantidadRotacion`.
4. Completa huecos con guias no asignados aun.
5. Construye `rolesGenerados`.

## Paso 4: deteccion de invalidez

`ComprobarAsignacion(...)` revisa:

- Si la sala existe.
- Si una sala obligatoria quedo vacia.
- Si salas especiales activas quedaron vacias.
- Si el guia asignado existe.
- Si la sala especial tiene al guia correcto.
- Si la capacitacion requerida esta presente.

## Paso 5: cambios internos

`AplicarCambiosInternos(...)` intenta corregir las asignaciones invalidas:

1. Busca guias validos para la sala afectada.
2. Prioriza un guia valido que no este asignado.
3. Si no existe, intenta reutilizar uno ya asignado.
4. Registra el cambio en `nombreCambioInterno`.

Importante:

- El cambio es logico, no necesariamente un swap fisico completo.
- Un guia usado como reemplazo no debe reutilizarse multiples veces.

## Paso 6: segunda revision

`SegundaRevisionCambiosInternos(...)` intenta revisar que el cambio interno final siga siendo valido para la sala destino.

## Pseudocodigo resumido

```text
datos <- parsear payload
roles <- rotar roles previos segun turno y valor
invalidos <- validar roles

si invalidos no esta vacio:
    para cada rol invalido:
        candidatos <- buscar guias validos
        elegir reemplazo
        registrar cambio interno

revisar cambios internos
devolver roles
```

## Trade-offs del algoritmo actual

- Ventaja: sencillo de seguir y suficientemente modular.
- Ventaja: la rotacion queda separada de la correccion.
- Riesgo: no garantiza solucion global optima.
- Riesgo: la logica de turnos depende de coincidencias por texto.
- Riesgo: el cambio interno registrado puede requerir reglas adicionales si luego se desea trazabilidad historica mas estricta.
