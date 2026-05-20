# Manual de operacion

## Objetivo

Explicar como usar la aplicacion desde la perspectiva de una persona operadora o administradora.

## Flujo recomendado

1. Abrir el frontend.
2. Revisar o actualizar la lista de guias.
3. Revisar o actualizar las salas.
4. Verificar capacitaciones.
5. Configurar turno, operadores, vacaciones y valor de rotacion.
6. Generar el rol.
7. Revisar la tabla resultante.
8. Exportar CSV.

## Modulos de la UI

### Guias

Permite:

- Agregar guia
- Editar nombre
- Cambiar turno
- Asignar capacitaciones
- Eliminar guia

### Salas

Permite:

- Agregar sala
- Definir capacitacion requerida
- Marcar si es obligatoria
- Eliminar sala

### Capacitaciones

Permite:

- Crear nuevas capacitaciones
- Renombrarlas
- Eliminarlas

### Generacion de rol

Permite:

- Elegir turno
- Elegir operadores del dia
- Elegir vacaciones del dia
- Ajustar valor de rotacion
- Generar el rol final

## Interpretacion de la tabla de salida

- `Sala`: destino operativo.
- `Guia`: asignacion base resultante.
- `Cambio Interno`: guia sugerido o usado para corregir una incompatibilidad.
- `Cambio Externo`: reservado para evolucion futura.

## Buenas practicas de uso

- Mantener actualizadas las capacitaciones antes de generar.
- Revisar el turno seleccionado antes de guardar operadores y vacaciones.
- Exportar el CSV luego de validar el resultado.
- Conservar respaldo externo si el rol necesita auditoria, ya que el estado principal del frontend esta en `localStorage`.
