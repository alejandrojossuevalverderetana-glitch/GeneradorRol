# GeneradorRol 🚀

**GeneradorRol** es una herramienta automatizada para organizar roles y turnos de guías en distintas salas. Diseñada en C++, permite asignar a cada guía su sala correspondiente considerando su capacitación, horarios y restricciones, optimizando la distribución de tareas y turnos.

Este proyecto está en desarrollo y combina lógica de programación orientada a objetos con algoritmos de rotación y asignación inteligente.

---

## ✨ Características principales

- **Asignación inteligente de guías:**  
  Cada guía se asigna a una sala considerando:
  - Capacitación requerida por la sala (Radio, Steam, Tele)
  - Horas de trabajo disponibles
  - Balance de horarios de almuerzo

- **Rotación de roles:**  
  Se puede rotar el orden de las guías para los días siguientes, asegurando variedad y cobertura eficiente.

- **Exportación de resultados:**  
  Permite guardar los roles generados en archivos de texto para su consulta o impresión.

- **Estructura modular y escalable:**  
  Gracias a las clases `Guia`, `Sala` y `GeneradorRol`, agregar nuevas reglas, salas o guías es sencillo.

---

## 🔧 Funcionalidades en desarrollo

- Completar el **main** del programa para generar roles de múltiples días automáticamente  
- Balancear de manera óptima las **horas de almuerzo** entre guías y salas  
- Exportar todas las asignaciones de forma completa y ordenada en archivos  
- Mejorar la **rotación de guías** para evitar repeticiones y asegurar cobertura equitativa  
- Agregar validaciones robustas y manejo de errores  
