# Decisiones tecnicas

## 1. Backend en C++ con Crow

Se eligio un backend compilado con una API HTTP minima.

Ventajas:

- Motor de reglas rapido y controlado.
- Dependencias relativamente livianas.
- Buen encaje para logica algoritimica.

Costo:

- Mayor friccion de build que una API interpretada.
- Menor ergonomia para pruebas y observabilidad si no se agregan herramientas.

## 2. Contrato JSON entre frontend y backend

La UI y el motor se integran por JSON.

Ventajas:

- Bajo acoplamiento.
- Facil reemplazar frontend o sumar otro cliente.
- Facil capturar payloads para pruebas futuras.

Costo:

- Si no se versiona el contrato, pueden aparecer incompatibilidades silenciosas.

## 3. Persistencia temporal en localStorage

El frontend guarda estado local sin depender de una base de datos.

Ventajas:

- Rapido para prototipar.
- Sin infraestructura extra.

Costo:

- Sin sincronizacion multiusuario.
- Sin historial confiable.
- Riesgo de perdida o inconsistencia por navegador/dispositivo.

## 4. Operadores y vacaciones como salas especiales

En lugar de modelarlos como otro sistema aparte, el codigo agrega las salas:

- `operador1`
- `operador2`
- `vacaciones1`
- `vacaciones2`

Ventajas:

- Simplifica parte del flujo de asignacion.
- Permite procesarlos dentro de la misma estructura de roles.

Costo:

- Mezcla conceptos de negocio distintos bajo la abstraccion de "sala".
- Puede complicar reportes o persistencia futura.

## 5. Correccion posterior a la rotacion

Primero se rota, luego se valida y corrige.

Ventajas:

- La rotacion base es explicable y trazable.
- Las excepciones no contaminan el algoritmo base.

Costo:

- La solucion puede ser valida localmente pero no globalmente optima.

## 6. Dependencias vendorizadas o descargadas

El repo incluye librerias copiadas o descargadas localmente.

Ventajas:

- Facilita compilar sin gestor de paquetes complejo.

Costo:

- Aumenta el tamano del repo.
- Exige disciplina para actualizar dependencias.

## 7. SSL embebido en el binario

El backend intenta iniciar con certificados ya definidos.

Ventajas:

- Prepara un escenario de despliegue real.

Costo:

- Hace mas delicada la ejecucion local.
- Conviene parametrizar rutas y puertos.
