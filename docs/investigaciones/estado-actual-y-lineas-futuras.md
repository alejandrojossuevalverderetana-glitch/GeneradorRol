# Estado actual y lineas futuras

## Hallazgos del repo

### Lo que ya esta resuelto

- Existe una UI operativa para administrar datos.
- Existe persistencia local en navegador.
- Existe un endpoint real de generacion.
- El motor contempla turnos, capacitaciones, operadores y vacaciones.
- Ya hay exportacion CSV en frontend.

### Lo que aun no esta consolidado

- No hay base de datos real.
- No hay suite de tests.
- No hay configuracion local evidente para el endpoint del frontend.
- Docker y backend necesitan alineacion de puertos/certificados.
- Los cambios externos existen en modelo/UI, pero aun no forman parte del algoritmo principal.

## Riesgos tecnicos identificados

### Acoplamiento por strings

La logica de turnos depende de comparar fragmentos de texto. Eso funciona para el estado actual, pero es fragil si crecen los tipos de turno.

### Trazabilidad parcial de cambios

El motor registra `nombreCambioInterno`, pero aun no parece existir un historial estructurado de decisiones o motivos.

### Persistencia local

`localStorage` es util para prototipo, pero no sirve como fuente confiable si el proyecto escala o requiere auditoria.

## Recomendaciones evolutivas

1. Definir un esquema de datos versionado para la API.
2. Parametrizar endpoint, puerto y SSL por variables de entorno.
3. Anadir casos de prueba con payloads representativos.
4. Persistir catalogos y roles historicos en una BD.
5. Modelar cambios internos y externos como eventos o movimientos explicitamente auditables.

## Preguntas utiles para una siguiente iteracion

- Debe el sistema optimizar cobertura total o solo corregir invalidos?
- Hay prioridad entre salas obligatorias?
- Se necesita historial por fecha y por turno?
- Los operadores pueden cubrir emergencias o deben quedar totalmente excluidos?
- Como se resuelven empates o multiples candidatos equivalentes?
