#include "GeneradorRol.hpp"
#include <algorithm>
#include <random>
#include <iostream>
#include <unordered_set>

bool GeneradorRol::ValidarDisponibilidad(const std::vector<GestorDatos::Guia>& guias,
                                         const std::vector<GestorDatos::Sala>& salas,
                                         const std::string& turno)
{
    // Contar salas obligatorias
    int obligatorias = 0;
    for (const auto& s : salas) {
        if (s.obligatoria)
            obligatorias++;
    }

    // Contar guías válidos:
    int guiasValidos = 0;
    for (const auto& g : guias) {
        if (GuiaValidoParaTurno(g.turno, turno)) {
            guiasValidos++;
        }
    }

    return guiasValidos > obligatorias;
}

bool GeneradorRol::CambioInternoValido(
    const std::string& guia,
    const std::string& salaNombre,
    const std::vector<GestorDatos::Guia>& guias,
    const std::vector<GestorDatos::Sala>& salas)
{
    auto itSala = std::find_if(
        salas.begin(), salas.end(),
        [&](const GestorDatos::Sala& s) { return s.nombre == salaNombre; });

    if (itSala == salas.end() || itSala->capacitacion.empty())
        return true;

    auto itGuia = std::find_if(
        guias.begin(), guias.end(),
        [&](const GestorDatos::Guia& g) { return g.nombre == guia; });

    if (itGuia == guias.end())
        return false;

    return std::find(
        itGuia->capacitaciones.begin(),
        itGuia->capacitaciones.end(),
        itSala->capacitacion
    ) != itGuia->capacitaciones.end();
}


/**
 * @brief Helper: verifica si un guía es operador o está en vacaciones
 */
bool GeneradorRol::EsOperadorOVacacion(
    const std::string& nombreGuia,
    const GestorDatos::Operadores& operadores,
    const GestorDatos::Vacaciones& vacaciones)
{
    return (nombreGuia == operadores.operador1 ||
            nombreGuia == operadores.operador2 ||
            nombreGuia == vacaciones.vacacion1 ||
            nombreGuia == vacaciones.vacacion2);
}

/**
 * @brief Helper: verifica si un guía está en su sala correcta (si es operador/vacación)
 */
bool GeneradorRol::EstaEnSalaCorrecta(
    const std::string& nombreGuia,
    const std::string& nombreSala,
    const GestorDatos::Operadores& operadores,
    const GestorDatos::Vacaciones& vacaciones)
{
    if (nombreGuia == operadores.operador1)
        return nombreSala == "operador1";
    if (nombreGuia == operadores.operador2)
        return nombreSala == "operador2";
    if (nombreGuia == vacaciones.vacacion1)
        return nombreSala == "vacaciones1";
    if (nombreGuia == vacaciones.vacacion2)
        return nombreSala == "vacaciones2";
    
    return false;
}
bool GeneradorRol::GuiaValidoParaTurno(
        const std::string& turnoGuia,
        const std::string& turnoActual){
            // Dividir el turno actual en palabras (espacios)
            std::istringstream ss(turnoActual);
            std::string palabra;
            while (ss >> palabra) {
                if (turnoGuia.find(palabra) != std::string::npos) {
                    return true; // Coincide al menos una palabra
                }
            }
            return false; // No coincidió ninguna
        }

/**
 * @brief Busca guías que puedan ser asignados a una sala específica.
 * 
 * La búsqueda:
 * - Excluye automáticamente a los operadores.
 * - Excluye guías que no pertenecen al turno actual.
 * - Considera guías ya asignados para evitar duplicaciones.
 * - Verifica la capacitación requerida por la sala.
 */
std::vector<GestorDatos::Guia> GeneradorRol::BuscarGuiasValidos(
    const GestorDatos::Sala& sala,
    const std::vector<GestorDatos::Guia>& guias,
    const std::vector<GestorDatos::RolGenerado>& rolesGenerados,
    const GestorDatos::Operadores& operadores,
    const GestorDatos::Vacaciones& vacaciones,
    const std::vector<GestorDatos::Sala>& salas,
    const std::string& turno)
{
    std::vector<GestorDatos::Guia> validos;

    for (const auto& g : guias)
    {
        // 1️⃣ Excluir operadores y vacaciones (excepto si es su sala correcta)
        if (EsOperadorOVacacion(g.nombre, operadores, vacaciones))
        {
            if (!EstaEnSalaCorrecta(g.nombre, sala.nombre, operadores, vacaciones))
                continue;
        }

        // 2️⃣ Verificar turno del guía vs turno de la sala
        if (!GuiaValidoParaTurno(g.turno, turno)) 
            continue;

        // 3️⃣ Verificar capacitación necesaria
       bool requiereCap = !sala.capacitacion.empty();
        // Si la sala no requiere capacitación, cualquier guía es válido

        if (requiereCap)
        {
            bool tieneCap = std::find(
                g.capacitaciones.begin(),
                g.capacitaciones.end(),
                sala.capacitacion) != g.capacitaciones.end();

            if (!tieneCap)
                continue;
        }


        // 4️⃣ Verificar si el guía ya está asignado a otra sala con capacitación
        bool yaAsignado = false;
        for (const auto& rol : rolesGenerados)
        {
            if (rol.nombreGuia == g.nombre)
            {
                auto itSala = std::find_if(salas.begin(), salas.end(),
                    [&](const GestorDatos::Sala& s){
                        return s.nombre == rol.nombreSala;
                    });

                // Si esta sala asignada requiere capacitación → no puede asignarse
                if (itSala != salas.end() && !itSala->capacitacion.empty())
                {
                    yaAsignado = true;
                    break;
                }
            }
            // Si ya se realizó un cambio con ese guia no puede asignarse
            if (rol.nombreCambioInterno == g.nombre)
            {
                yaAsignado = true;    
                break;
            }
        }

        if (yaAsignado)
            continue;

        // Si pasó todos los filtros, es válido
        validos.push_back(g);
    }

    return validos;
}

void GeneradorRol::AsignarGuias(const std::vector<GestorDatos::Guia>& guias,
                                const std::vector<GestorDatos::Sala>& salas,
                                const std::vector<GestorDatos::Rol>& rolesPrevios,
                                const int cantidadRotacion,
                                const std::string& turno)
{
    size_t nSalas = salas.size();
    rolesGenerados.clear();
    rolesGenerados.reserve(nSalas);

    // ============================
    // 1) Filtrar guías válidos
    // ============================
    std::vector<std::string> guiasValidos;
    for (const auto& g : guias)
    {
        if (!GuiaValidoParaTurno(g.turno, turno)) continue;
        guiasValidos.push_back(g.nombre);
    }

    if (guiasValidos.empty())
    {
        // No hay guías válidos → generar roles vacíos
        for (const auto& sala : salas)
        {
            GestorDatos::RolGenerado r;
            r.nombreSala = sala.nombre;
            rolesGenerados.push_back(r);
        }
        return;
    }

    // ============================
    // 2) Posición actual según roles previos
    //   (solo entre guías válidos)
    // ============================
    // Mapa: salaIndex → nombreGuiaPrevio (si aplica)
    std::vector<std::string> posicionesPrevias(nSalas, "");

    for (const auto& rolPrev : rolesPrevios)
    {
        // Encontrar la sala donde estaba asignado
        auto itSala = std::find_if(salas.begin(), salas.end(),
            [&](const GestorDatos::Sala& s) { return s.nombre == rolPrev.nombreSala; });

        if (itSala == salas.end()) continue;

        size_t idxSala = std::distance(salas.begin(), itSala);

        // Verificar si el guía existe y sigue en el turno
        auto itGuia = std::find(guiasValidos.begin(), guiasValidos.end(), rolPrev.nombreGuia);
        if (itGuia != guiasValidos.end())
        {
            posicionesPrevias[idxSala] = *itGuia;
        }
    }

    // ============================
    // 3) Realizar la rotación
    // ============================
    std::vector<std::string> posicionesRotadas(nSalas, "");
    int rot = (nSalas > 0) ? (cantidadRotacion % static_cast<int>(nSalas)) : 0;

    for (size_t i = 0; i < nSalas; ++i)
    {
        if (posicionesPrevias[i].empty()) continue;

        size_t destino = (i + rot) % nSalas;

        // Buscar una posición vacía
        size_t intentos = 0;
        while (!posicionesRotadas[destino].empty() && intentos < nSalas)
        {
            destino = (destino + 1) % nSalas;
            ++intentos;
        }

        posicionesRotadas[destino] = posicionesPrevias[i];
    }

    // ============================
    // 4) Agregar guías nuevos que no estaban antes
    // ============================
    std::unordered_set<std::string> asignados;

    for (const auto& g : posicionesRotadas)
        if (!g.empty()) asignados.insert(g);

    size_t idxSalaLibre = 0;

    for (const auto& nombreGuia : guiasValidos)
    {
        if (asignados.count(nombreGuia)) continue; // ya está asignado

        // Buscar una sala libre
        while (idxSalaLibre < nSalas && !posicionesRotadas[idxSalaLibre].empty())
            ++idxSalaLibre;

        if (idxSalaLibre < nSalas)
        {
            posicionesRotadas[idxSalaLibre] = nombreGuia;
            asignados.insert(nombreGuia);
        }
    }

    // ============================
    // 5) Construir salida final
    // ============================
    for (size_t i = 0; i < nSalas; ++i)
    {
        GestorDatos::RolGenerado rol;
        rol.nombreSala = salas[i].nombre;
        rol.nombreGuia = posicionesRotadas[i];
        rol.nombreCambioInterno = "";
        rol.nombreCambioAprobado = "";
        rolesGenerados.push_back(std::move(rol));
    }
}

std::vector<GestorDatos::RolGenerado> GeneradorRol::ComprobarAsignacion(
    const std::vector<GestorDatos::Guia>& guias,
    const std::vector<GestorDatos::Sala>& salas,
    const std::vector<GestorDatos::RolGenerado>& roles,
    const GestorDatos::Operadores& operadores,
    const GestorDatos::Vacaciones& vacaciones,
    const std::string& turno)
{
    std::vector<GestorDatos::RolGenerado> invalidos;

    for (const auto& rol : roles)
    {
        bool invalido = false;
        // =============================
        // 1) Buscar información de la sala
        // =============================
        auto itSala = std::find_if(salas.begin(), salas.end(),
                                   [&](const GestorDatos::Sala& s)
                                   { return s.nombre == rol.nombreSala; });
 
        if (itSala == salas.end()) {
            invalidos.push_back(rol);
            continue;
        }

        const auto& sala = *itSala;

        // =============================
        // 2) Verificar si hay guía asignado
        // =============================
        if (rol.nombreGuia.empty())
        {
            // Si NO hay guía y la sala es obligatoria → inválido
            if (sala.obligatoria)
                invalido = true;
            if (invalido)
                invalidos.push_back(rol);

            continue; // si no es obligatoria, un vacío es válido
        }

        // =============================
        // 3) Buscar información del guía
        // =============================
        auto itGuia = std::find_if(guias.begin(), guias.end(),
                                   [&](const GestorDatos::Guia& g)
                                   { return g.nombre == rol.nombreGuia; });

        if (itGuia == guias.end()) {
            invalidos.push_back(rol);
            continue;
        }

        const auto& guia = *itGuia;

        // =============================
        // 4) Verificar operadores y vacaciones
        // =============================
        if (EsOperadorOVacacion(guia.nombre, operadores, vacaciones))
        {
            if (!EstaEnSalaCorrecta(guia.nombre, sala.nombre, operadores, vacaciones))
                invalido = true;
            
        }


        // =============================
        // 5) Verificar capacitación necesaria
        // =============================
        if (!sala.capacitacion.empty())
        {
            bool tieneCap =
                std::find(guia.capacitaciones.begin(), guia.capacitaciones.end(),
                          sala.capacitacion) != guia.capacitaciones.end();

            if (!tieneCap)
                invalido = true;
        }
        if (invalido)
            invalidos.push_back(rol);

    }
    // =============================
    // DEBUG: imprimir asignaciones inválidas
    // =============================
    if (!invalidos.empty())
    {
        std::cout << "\n[DEBUG] Asignaciones inválidas detectadas:\n";

        for (const auto& rol : invalidos)
        {
            std::cout
                << "  Sala: " << (rol.nombreSala.empty() ? "(sin sala)" : rol.nombreSala)
                << " | Guía: " << (rol.nombreGuia.empty() ? "(sin guía)" : rol.nombreGuia)
                << '\n';
        }
    }
    else
    {
        std::cout << "\n[DEBUG] No se detectaron asignaciones inválidas.\n";
    }
    
    return invalidos;
}

/**
 * @brief Aplica cambios internos a los roles generados si el guía no cumple requisitos.
 */
void GeneradorRol::AplicarCambiosInternos(
    const std::vector<GestorDatos::Guia>& guias,
    const std::vector<GestorDatos::Sala>& salas,
    const GestorDatos::Operadores& operadores,
    const GestorDatos::Vacaciones& vacaciones,
    const std::string& turno)
{
    std::unordered_set<std::string> guiasUsadosComoReemplazo;

    // 1) Obtener roles inválidos con turno y operadores filtrados
    auto invalidos = ComprobarAsignacion(guias, salas, rolesGenerados, operadores, vacaciones, turno);

    // Invertir el orden para primero aplicar cambios en operadores
    std::reverse(invalidos.begin(), invalidos.end());
    // Nada que corregir
    if (invalidos.empty()) return;

    // ================
    // Pre cálculo: Salas obligatorias
    // ================
    std::unordered_set<std::string> salasObligatorias;
    for (const auto& s : salas)
        if (s.obligatoria)
            salasObligatorias.insert(s.nombre);

    // ================
    // Recorrer cada rol inválido para corregirlo
    // ================
    for (auto& rolInvalido : invalidos)
    {
        // 2) Buscar la sala asociada
        auto itSala = std::find_if(
            salas.begin(), salas.end(),
            [&](const GestorDatos::Sala& s) { return s.nombre == rolInvalido.nombreSala; });

        if (itSala == salas.end()) {
            continue;
        }

        const auto& sala = *itSala;

        // 3) Obtener guías válidos (esto ya excluye operadores y fuera de turno)
        auto guiasValidos = BuscarGuiasValidos(
            sala, guias, rolesGenerados, operadores, vacaciones, salas, turno);

        if (guiasValidos.empty())
        {
            // No hay reemplazos → no se puede corregir
            continue;
        }

        // ================
        // 4) Elegir guía para reemplazo
        //    Estrategia:
        //    - Un guía disponible (no asignado)
        //    - Sino, intercambiar con uno no crítico
        // ================

        std::string guiaReemplazo = "";

        // 4A) Buscar un guía NO ASIGNADO a ninguna sala
        for (const auto& g : guiasValidos)
        {
            if (EsOperadorOVacacion(g.nombre, operadores, vacaciones))
                continue;
            
            if (guiasUsadosComoReemplazo.count(g.nombre))
                continue;

            bool yaAsignado = std::any_of(
                rolesGenerados.begin(), rolesGenerados.end(),
                [&](const GestorDatos::RolGenerado& r)
                { return r.nombreGuia == g.nombre; });

            if (!yaAsignado)
            {
                guiaReemplazo = g.nombre;
                break;
            }
        }

        // 4B) Si todos los guías posibles están asignados → intercambiar
        if (guiaReemplazo.empty())
        {
            for (const auto& g : guiasValidos)
            {
                // No operadores ni vacaciones
                if (EsOperadorOVacacion(g.nombre, operadores, vacaciones))
                    continue;
            
                if (guiasUsadosComoReemplazo.count(g.nombre))
                    continue;

                // Evitar intercambiar desde salas obligatorias si estamos corrigiendo una obligatoria
                if (salasObligatorias.count(sala.nombre))
                {
                    // Estamos corrigiendo una sala obligatoria
                    // → permitir tomar guías solo de salas NO obligatorias
                    auto rolOrigen = std::find_if(
                        rolesGenerados.begin(), rolesGenerados.end(),
                        [&](const GestorDatos::RolGenerado& r)
                        { return r.nombreGuia == g.nombre; });

                    if (rolOrigen != rolesGenerados.end())
                    {
                        if (!salasObligatorias.count(rolOrigen->nombreSala))
                        {
                            guiaReemplazo = g.nombre;
                            break;
                        }
                    }
                }
                else
                {
                    // La sala NO es obligatoria → cualquier guía válido sirve
                    guiaReemplazo = g.nombre;
                    break;
                }
            }
        }

        if (guiaReemplazo.empty())
            continue; // no se encontró forma de corregir

        // ======================
        // 5) Aplicar cambio interno lógico (SIN swap real)
        // ======================
        std::string guiaSale  = rolInvalido.nombreGuia;
        std::string guiaEntra = guiaReemplazo;

        GestorDatos::RolGenerado* salaInvalida   = nullptr;
        GestorDatos::RolGenerado* salaReemplazo  = nullptr;

        // Buscar ambas salas involucradas
        for (auto& r : rolesGenerados)
        {
            if (r.nombreSala == rolInvalido.nombreSala)
                salaInvalida = &r;

            if (r.nombreGuia == guiaEntra)
                salaReemplazo = &r;
        }

        // Registrar cambio lógico (sin mover guías)
        if (salaInvalida)
        {
            salaInvalida->nombreCambioInterno = guiaEntra;
            guiasUsadosComoReemplazo.insert(guiaEntra);
        }

        if (salaReemplazo)
        {
            salaReemplazo->nombreCambioInterno = guiaSale;
        }
    }
}

void GeneradorRol::SegundaRevisionCambiosInternos(
    const std::vector<GestorDatos::Guia>& guias,
    const std::vector<GestorDatos::Sala>& salas,
    const GestorDatos::Operadores& operadores,
    const GestorDatos::Vacaciones& vacaciones,
    const std::string& turno)
{
    for (auto& rol : rolesGenerados)
    {
        if (rol.nombreCambioInterno.empty())
            continue;

        if (CambioInternoValido(
                rol.nombreCambioInterno,
                rol.nombreSala,
                guias,
                salas))
            continue;

        // Buscar reemplazo válido
        auto guiasValidos = BuscarGuiasValidos(
            *std::find_if(salas.begin(), salas.end(),
                [&](const GestorDatos::Sala& s){ return s.nombre == rol.nombreSala; }),
            guias,
            rolesGenerados,
            operadores,
            vacaciones,
            salas,
            turno);

        for (const auto& g : guiasValidos)
        {
            if (g.nombre == rol.nombreGuia)
                continue;

            // swap SOLO de cambios internos
            for (auto& otro : rolesGenerados)
            {
                if (otro.nombreGuia == g.nombre)
                {
                    std::swap(rol.nombreCambioInterno,
                              otro.nombreCambioInterno);
                    break;
                }
            }

            // basta con un swap válido
            if (CambioInternoValido(
                    rol.nombreCambioInterno,
                    rol.nombreSala,
                    guias,
                    salas))
                break;
        }
    }
}


/**
 * @brief Genera el rol completo del día, aplicando rotación y cambios internos.
 * 
 * Flujo:
 * 1) Generación del rol desplazado según rotación y turno.
 * 2) Exclusión de operadores en toda la asignación.
 * 3) Aplicación de cambios internos (capacitación, salas obligatorias).
 * 4) Posibilidad de incluir lógica para cambios externos.
 */
std::vector<GestorDatos::RolGenerado> GeneradorRol::generarRoles(const GestorDatos& datos) 
{
    // ===============================
    // 1️⃣ Asignación inicial con rotación
    // ===============================
    // Los operadores TIENEN que ser excluidos aquí (ya se manejan dentro de la función).
    AsignarGuias(
        datos.guias,
        datos.salas,
        datos.roles,
        datos.valorRotacion,
        datos.turno
    );

    // ===============================
    // 2️⃣ Cambios internos por capacitación y salas obligatorias
    // ===============================
    // - Busca guías que no cumplen requisitos.
    // - Intenta reemplazarlos con guías válidos.
    // - Jamás usa operadores como reemplazo.
    AplicarCambiosInternos(datos.guias, datos.salas, datos.operadores, datos.vacaciones, datos.turno);
    SegundaRevisionCambiosInternos(datos.guias, datos.salas, datos.operadores, datos.vacaciones, datos.turno);
    // ===============================
    // 3️⃣ Cambios externos (futuro)
    // ===============================
    // Aquí eventualmente podrás:
    // - Comparar contra el rol del turno "tarde".
    // Por ahora no se modifica nada.

    // ===============================
    // 4️⃣ Devolver rol final
    // ===============================
    return rolesGenerados;
}