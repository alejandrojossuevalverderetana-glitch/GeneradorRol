# GeneradorRol 🚀

**GeneradorRol** es una herramienta para organizar roles y turnos de guías en distintas salas. Ahora implementada como página web, mantiene la lógica de asignación inteligente, pero con interfaz interactiva que permite visualizar y editar roles en tiempo real.

Este proyecto está en desarrollo y algunos aspectos aún están pendientes de implementar, reflejados como TODO.

---

## ✨ Características principales

- **Asignación inteligente de guías:**  
  Cada guía se asigna a una sala considerando:
  - Capacitación requerida por la sala (Radio, Steam, Tele)
  - Horas de trabajo disponibles
  - Balance de horarios de almuerzo

- **Visualización web:**  
  - Roles y turnos se muestran en tablas dinámicas
  - Ajustes interactivos mediante sliders, checkboxes y botones
  - Interfaz responsiva para dispositivos móviles

- **Estructura modular:**  
  Las funciones están organizadas por componentes (`tabla`, `cambios`, `formulario`) para facilitar futuras ampliaciones.

---

## 🔧 TODO (pendientes)

- Guardado de cambios directamente en la base de datos o archivo
- Validación completa de formularios antes de aplicar cambios
- Rotación automática de guías para múltiples días
- Exportación de roles a CSV desde la versión web
- Manejo de errores en la interfaz (inputs inválidos, conflictos de horarios)
- Optimización de estilos y animaciones en móviles
- Agregar notificaciones de confirmación al guardar o cancelar cambios
