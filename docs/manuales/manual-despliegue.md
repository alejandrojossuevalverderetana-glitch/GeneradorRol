# Manual de despliegue

## Componentes

- Frontend estatico en `Frontend/`
- Backend C++ en `Backend/`
- Dependencias locales o descargadas por `Dockerfile`

## Despliegue del backend

### Compilacion esperada

El `Dockerfile` compila:

```bash
g++ -std=c++17 Backend/api.cpp Backend/GeneradorRol.cpp Backend/GestorDatos.cpp -Icrow -Inlohmann -Icpp-httplib -Iasio -o api -lpthread -lssl -lcrypto -DCROW_ENABLE_SSL
```

### Requisitos

- Compilador con soporte C++17
- OpenSSL
- Headers de Crow
- Header de `nlohmann/json`
- Header de `cpp-httplib`
- Asio

### Consideraciones actuales

- El servidor se inicia sobre `0.0.0.0`.
- El codigo llama `.port(8080)`.
- El `Dockerfile` expone `18080`.
- El backend espera certificados en:
  - `/app/certs/fullchain.pem`
  - `/app/certs/privkey.pem`

## Despliegue del frontend

Como es estatico, basta con servir `Frontend/index.html` y sus recursos adjuntos.

Opciones:

- Nginx
- Apache
- GitHub Pages o equivalente
- Cualquier servidor estatico simple

## Ajuste de endpoint

Actualmente `Frontend/script.js` apunta a:

```text
https://araxus.ddns.net:18080/generar
```

Para despliegue serio conviene:

- Moverlo a variable de entorno o archivo de configuracion
- Evitar hardcodear dominio y puerto
- Unificar protocolo, puerto y certificados

## Checklist de despliegue

- [ ] Confirmar certificados SSL
- [ ] Confirmar puerto externo e interno
- [ ] Confirmar endpoint consumido por el frontend
- [ ] Probar `POST /generar` con payload real
- [ ] Verificar CORS
- [ ] Validar exportacion CSV desde la UI

## Recomendacion

La siguiente mejora de infraestructura deberia ser parametrizar configuracion de red y seguridad para separar:

- Desarrollo local
- Staging
- Produccion
