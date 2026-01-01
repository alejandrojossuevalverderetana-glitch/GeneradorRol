// ====================================
// SISTEMA DE ADMINISTRACIÓN DE GUÍAS, SALAS Y CAPACITACIONES
// ====================================

// ++++++++++++++++++++++++++++++++++++
// TODO:
// 1. Generar rol mediante API privada
// 2. Validacion
// +++++++++++++++++++++++++++++++++++++
// +++++++++++++++++++++++++++++++++++++
// NEXT RELEASES
// 1. Bases de datos
// +++++++++++++++++++++++++++++++++++++
const App = (() => {
// =====================================================
// 🗂 Estado de la aplicación
// Se almacenan datos principales: guías, salas, capacitaciones, roles generados y elementos seleccionados actualmente
// =====================================================
  const state =   {
  guias: [
    // ======================
    // TURNO MAÑANA
    // ======================
    { nombre: "Ana",     turno: "mañana", capacitaciones: ["Tele", "Operador"] },
    { nombre: "Pedro",   turno: "mañana", capacitaciones: ["Radio"] },
    { nombre: "Juan",    turno: "mañana", capacitaciones: ["Tele"] },
    { nombre: "Lucía",   turno: "mañana", capacitaciones: [] },
    { nombre: "Carlos",  turno: "mañana", capacitaciones: ["Steam"] },
    { nombre: "Sofía",   turno: "mañana", capacitaciones: ["Radio", "Tele"] },
    { nombre: "Diego",   turno: "mañana", capacitaciones: [] },
    { nombre: "Valeria", turno: "mañana", capacitaciones: ["Tele"] },
    { nombre: "Andrés",  turno: "mañana", capacitaciones: ["Radio"] },

    // ======================
    // TURNO TARDE
    // ======================
    { nombre: "Salem",   turno: "tarde", capacitaciones: ["Tele", "Operador"] },
    { nombre: "Luis",    turno: "tarde", capacitaciones: ["Radio"] },
    { nombre: "María",   turno: "tarde", capacitaciones: ["Tele"] },
    { nombre: "Paula",   turno: "tarde", capacitaciones: [] },
    { nombre: "Jorge",   turno: "tarde", capacitaciones: ["Steam"] },
    { nombre: "Elena",   turno: "tarde", capacitaciones: ["Radio", "Tele"] },
    { nombre: "Ricardo", turno: "tarde", capacitaciones: [] },
    { nombre: "Camila",  turno: "tarde", capacitaciones: ["Tele"] }
  ],

    salas: [
      {
        nombre: "Universo",
        capacitacion: "",
        obligatoria: true
      },
      {
        nombre: "Tierra",
        capacitacion: "",
        obligatoria: true
      },
      {
        nombre: "Costa Rica",
        capacitacion: "",
        obligatoria: false
      },
      {
        nombre: "Estadio",
        capacitacion: "",
        obligatoria: false
      },
      {
        nombre: "Radio",
        capacitacion: "Radio",
        obligatoria: true
      },
      {
        nombre: "Television",
        capacitacion: "Tele",
        obligatoria: true
      },
      {
        nombre: "Steam",
        capacitacion: "Steam",
        obligatoria: false
      }
    ],

    capacitaciones: ["Tele", "Radio", "Steam", "Operador"],

    // Roles temporales
    roles: [
    ],
    // Roles anteriores, por turno
    rolesAnteriores: {
      manana: [],
      tarde: [],
      finesManana: [],
      finesTarde: []
    },

    // Coincide con GestorDatos::Operadores
    operadores: {
      operador1: "",
      operador2: ""
    },

    // Coincide con GestorDatos::Vacaciones
    vacaciones: {
      vacacion1: "",
      vacacion2: ""
    },

    // Rotación
    valor: 2,

    // Turno actual
    turno: "mañana",

    // Estado UI
    actual: {
      guia: null,
      sala: null,
      capacitacion: null
    },

    // Cambios externos (futuro)
    cambios: []
  };

  // =====================================================
// 💾 Persistencia de datos
// =====================================================
const storage = {
  key: "appData",

  guardar: () => {
    const data = {
      guias: state.guias,
      salas: state.salas,
      capacitaciones: state.capacitaciones,
      roles: state.roles,
      cambios: state.cambios,
      vacaciones: state.vacaciones,
      operadores: state.operadores,
      valor: state.valor    ,
      turno : state.turno  

      };
    localStorage.setItem(storage.key, JSON.stringify(data));
    console.log("✅ Datos guardados en localStorage");
  },

  cargar: () => {
    const raw = localStorage.getItem(storage.key);
    if (!raw) return;
    try {
      const data = JSON.parse(raw);
      state.guias = data.guias || [];
      state.salas = data.salas || [];
      state.capacitaciones = data.capacitaciones || [];
      state.roles = data.roles || [];
      state.cambios = data.cambios || [];
      state.vacaciones = {
        vacacion1: data.vacaciones?.vacacion1 || "",
        vacacion2: data.vacaciones?.vacacion2 || ""
      };

      state.operadores = {
        operador1: data.operadores?.operador1 || "",
        operador2: data.operadores?.operador2 || ""
      };

      state.valor = data.valor ?? 5;
      state.turno = data.turno || "mañana";
      console.log("✅ Datos cargados desde localStorage");
    } catch (e) {
      console.error("⚠️ Error cargando datos:", e);
    }
  }
};

// =====================================================
// 🌐 Referencias al DOM
// Contiene todos los elementos HTML que se manipulan, como botones, páginas de edición y contenedores de datos
// =====================================================
  const dom = {
    guiaEditPage: document.getElementById("guiaEditPage"),
    salaEditPage: document.getElementById("salaEditPage"),
    capacitacionEditPage: document.getElementById("capacitacionEditPage"),
    rolEditPage:document.getElementById("rolEditPage"),
    cambiosForm:document.getElementById("cambiosForm"),
    editTurno:document.getElementById("editTurno"),

    btnGuias: document.getElementById("btnGuias"),
    btnSalas: document.getElementById("btnSalas"),
    btnCapacitaciones: document.getElementById("btnCapacitaciones"),
    btnRol: document.getElementById("btnRol"),

    guiasPage: document.getElementById("guiasPage"),
    salasPage: document.getElementById("salasPage"),
    capacitacionesPage: document.getElementById("capacitacionesPage"),
    output: document.getElementById("output"),

    sliderValue: document.getElementById("valor"),
    rolSlider: document.getElementById("rolSlider"),
    rolesContainer: document.getElementById("rolesContainer"),
    tablaRoles: document.getElementById("tablaRoles"),
    exportBtn: document.getElementById("exportBtn"),
    cancelBtn: document.getElementById("cancelBtn"),
    cambiosContainer: document.getElementById("cambiosContainer"),

    agregarGuia: document.getElementById("agregarGuia"),
    agregarSala: document.getElementById("agregarSala"),
    agregarCapacitacion: document.getElementById("agregarCapacitacion"),
    agregarCambio: document.getElementById("agregarCambio"),

    guardarGuia: document.getElementById("guardarGuia"),
    guardarSala: document.getElementById("guardarSala"),
    guardarCapacitacion: document.getElementById("guardarCapacitacion"), 
    guardarCambio : document.getElementById("guardarCambio"),
    guardarRol: document.getElementById("guardarRol"),

    cancelarRol: document.getElementById("cancelarRol"),
    cancelarCambio: document.getElementById("cancelarCambio"),

    cerrarBtns: document.querySelectorAll(".cerrarPage")
    
  };
// =====================================================
// 🖌 Funciones de renderizado
// Se encargan de actualizar las tablas de guías, salas, capacitaciones y roles en el HTML
// =====================================================
  const render = {
    guias: () => {
      const tbody = document.querySelector("#tablaGuias tbody");
      tbody.innerHTML = "";
      state.guias.forEach((g, i) => {
        const tr = document.createElement("tr");
        tr.innerHTML = `
          <td>${g.nombre}</td>
          <td>${g.turno}</td>
          <td>${g.capacitaciones.join(", ")}</td>
          <td>
            <button class="editar" data-index="${i}" data-type="guia">✏️</button>
            <button class="eliminar" data-index="${i}" data-type="guia">🗑️</button>
          </td>
        `;
        tbody.appendChild(tr);
      });
    },
    salas: () => {
      const tbody = document.querySelector("#tablaSalas tbody");
      tbody.innerHTML = "";
      state.salas.forEach((s, i) => {
        const tr = document.createElement("tr");
        tr.innerHTML = `
          <td>${s.nombre}</td>
          <td>${s.capacitacion}</td>
          <td style="text-align:center; font-size: 1.2em;">
            ${s.obligatoria ? "✅" : "❌"}
          </td>
          <td>
            <button class="editar" data-index="${i}" data-type="sala">✏️</button>
            <button class="eliminar" data-index="${i}" data-type="sala">🗑️</button>
          </td>
        `;
        tbody.appendChild(tr);
      });
    },
    capacitaciones: () => {
      const tbody = document.querySelector("#tablaCapacitaciones tbody");
      tbody.innerHTML = "";
      state.capacitaciones.forEach((c, i) => {
        const tr = document.createElement("tr");
        tr.innerHTML = `
          <td>${c}</td>
          <td>
            <button class="editar" data-index="${i}" data-type="capacitacion">✏️</button>
            <button class="eliminar" data-index="${i}" data-type="capacitacion">🗑️</button>
          </td>
        `;
        tbody.appendChild(tr);
      });
    },
    roles: () => {
      const tbody = dom.tablaRoles.querySelector("tbody");
      tbody.innerHTML = "";
      state.roles.forEach(r => {
        const tr = document.createElement("tr");
        tr.innerHTML = `
          <td>${r.nombreSala}</td>
          <td>${r.nombreGuia}</td>
          <td>${r.nombreCambioInterno}</td>
          <td>${r.nombreCambioExterno || ""}</td>
        `;
        tbody.appendChild(tr);
      });
    },
  cambios: (cambios, tabla) => {
    if (!Array.isArray(cambios) || !tabla) return;

    const filas = cambios.map(c => `
      <tr>
        <td>${c.guiaMañana}</td>
        <td>${c.guiaTarde}</td>
      </tr>
    `).join("");

    tabla.innerHTML = `
      <tr>
        <th> (PROXIMAMENTE) </th>
      </tr>
      ${filas}
    `;

    // tabla.classList.remove("hidden");
  },
  vacaciones: (turno) => {
    const container = document.getElementById("editVacaciones");
      container.innerHTML = "";

      const guiasTurno = state.guias.filter(g =>
        turno.startsWith("fines")
          ? g.turno.includes(turno.split(" ")[1])
          : g.turno === turno || g.turno.includes("entreSemana")
      );

      guiasTurno.forEach(g => {
        const checked =
          g.nombre === state.vacaciones.vacacion1 ||
          g.nombre === state.vacaciones.vacacion2;

          const label = document.createElement("label");
          label.classList.add("checkbox-label"); // Clase para aplicar estilos

          const input = document.createElement("input");
          input.type = "checkbox";
          input.value = g.nombre;
          if (checked) input.checked = true;
          input.classList.add("checkbox-input"); // Clase para aplicar estilos

          label.appendChild(input);
          label.appendChild(document.createTextNode(g.nombre));
          container.appendChild(label);
      }); 
      container.onchange = () => {
        const checked = container.querySelectorAll("input:checked");
        if (checked.length > 2) {
          checked[checked.length - 1].checked = false;
          alert("Solo se pueden seleccionar 2 guías en vacaciones.");
        }
      };
  }


  };
// =====================================================
// 📄 Manejo de formularios
// Abre formularios de edición o creación de guías, salas y capacitaciones, rellenando datos existentes si aplica
// =====================================================
  const forms = {
    abrirGuia: () => {
      const g = state.actual.guia;
      document.getElementById("editNombreGuia").value = g ? g.nombre : "";
      document.getElementById("editTurno").value = g ? g.turno : "mañana";

      const container = document.getElementById("editCapacitaciones");
      container.innerHTML = "";
      state.capacitaciones.forEach(cap => {
        const checked = g && g.capacitaciones.includes(cap) ? "checked" : "";
        container.innerHTML += `<label><input type="checkbox" value="${cap}" ${checked}> ${cap}</label>`;
      });

      dom.guiaEditPage.classList.remove("hidden");
    },
    abrirSala: () => {
      const s = state.actual.sala;
      document.getElementById("editNombreSala").value = s ? s.nombre : "";
      document.getElementById("editSalaObligatoria").checked = s?.obligatoria ?? false;

      const select = document.getElementById("editCapacitacion");
      select.innerHTML = '<option value="">N/A</option>';
      state.capacitaciones.forEach(cap => {
        const selected = s && s.capacitacion === cap ? "selected" : "";
        select.innerHTML += `<option value="${cap}" ${selected}>${cap}</option>`;
      });

      dom.salaEditPage.classList.remove("hidden");
    },
    abrirCapacitacion: () => {
      const c = state.actual.capacitacion;
      document.getElementById("editNombreCapacitacion").value = c || "";
      dom.capacitacionEditPage.classList.remove("hidden");
    },
    abrirRol: () => {
      const container = document.getElementById("editOperadores");
      container.innerHTML = ""; 
      const turno = state.turno;

      let operadores;

      if (turno.startsWith("fines")) {
        const partes = turno.split("-");
        const subTurno = partes[1];
        operadores = state.guias.filter(g => g.capacitaciones.includes("Operador") && g.turno.includes(subTurno));
      }else {
        operadores = state.guias.filter(
          g =>
          g.capacitaciones.includes("Operador") &&
          (g.turno === turno || g.turno.includes("entreSemana"))
        );
      }

      operadores.forEach(op => {
        const checked =
          op.nombre === state.operadores.operador1 ||
          op.nombre === state.operadores.operador2;

        const label = document.createElement("label");
        label.innerHTML = `
          <input type="checkbox" value="${op.nombre}" ${checked ? "checked" : ""}>
          ${op.nombre}
        `;
        container.appendChild(label);
        });
        container.addEventListener("change", () => {
          const checked = container.querySelectorAll("input:checked");
          if (checked.length > 2) {
            checked[checked.length - 1].checked = false;
            alert("Solo se pueden seleccionar 2 operadores.");
          }
        });

      dom.rolEditPage.classList.remove("hidden");
      render.cambios(state.cambios, dom.cambiosContainer)
      render.vacaciones(turno)
    }


  };
// =====================================================
// 🛠 Funciones CRUD (Crear, Leer, Actualizar, Eliminar)
// Guardan los cambios realizados en guías, salas y capacitaciones desde los formularios
// =====================================================
  const crud = {
    guardarGuia: () => {
      const nombre = document.getElementById("editNombreGuia").value.trim();
      const turno = document.getElementById("editTurnoGuia").value;
      const caps = Array.from(document.querySelectorAll("#editCapacitaciones input[type='checkbox']:checked")).map(i => i.value);
      if (!nombre) return alert("El nombre no puede estar vacío");

      if (state.actual.guia) {
        state.actual.guia.nombre = nombre;
        state.actual.guia.turno = turno;
        state.actual.guia.capacitaciones = caps;
      } else state.guias.push({ nombre, turno, capacitaciones: caps });

      dom.guiaEditPage.classList.add("hidden");
      render.guias();
      storage.guardar();
    },
    guardarSala: () => {
      const nombre = document.getElementById("editNombreSala").value.trim();
      const capacitacion = document.getElementById("editCapacitacion").value;
      const obligatoria = document.getElementById("editSalaObligatoria").checked;
      if (!nombre) return alert("El nombre no puede estar vacío");

      if (state.actual.sala) {
        state.actual.sala.nombre = nombre;
        state.actual.sala.capacitacion = capacitacion;
        state.actual.sala.obligatoria = obligatoria;
      } 
      else state.salas.push({ 
        nombre, 
        capacitacion, 
        obligatoria
      });

      dom.salaEditPage.classList.add("hidden");
      render.salas();
      storage.guardar();
    },
    guardarCapacitacion: () => {
      const nombre = document.getElementById("editNombreCapacitacion").value.trim();
      if (!nombre) return alert("El nombre no puede estar vacío");

      if (state.actual.capacitacion !== null) {
        const index = state.capacitaciones.indexOf(state.actual.capacitacion);
        if (index !== -1) state.capacitaciones[index] = nombre;
      } else state.capacitaciones.push(nombre);

      dom.capacitacionEditPage.classList.add("hidden");
      render.capacitaciones();
      storage.guardar();
    },
    guardarCambio: () => {
  const selects = dom.cambiosForm.querySelectorAll("select");
  const guiaMañana = selects[0]?.value?.trim() || "";
  const guiaTarde = selects[1]?.value?.trim() || "";

    if (!guiaMañana || !guiaTarde) {
    alert("Debes seleccionar ambos guías antes de guardar el cambio.");
    return;
  }

  const cambio = { guiaMañana, guiaTarde };
  state.cambios.push(cambio);


  dom.cambiosForm.classList.add("hidden");
  dom.agregarCambio.classList.remove("hidden");
  storage.guardar();
  
}
  };

// Función para generar roles desde la API
async function generarRoles() {
    try {
        // Preparar los datos que enviarás al servidor (puedes ajustarlo según tu estructura)
        const payload = {
            turno: state.turno,
            valor: state.valor,
            guias: state.guias,   // lista de guías
            salas: state.salas,   // lista de salas
            roles: state.rolesAnteriores[state.turno],   // roles actuales, si aplica
            cambios: state.cambios, // cambios si los tienes
            operadores: {
              operador1: state.operadores.operador1 || "",
              operador2: state.operadores.operador2 || ""
            },

            vacaciones: {
              vacacion1: state.vacaciones.vacacion1 || "",
              vacacion2: state.vacaciones.vacacion2 || ""
            }  
        };

        // Petición POST al endpoint /generar de tu API
        const response = await fetch("https://araxus.ddns.net:18080/generar", {
            method: "POST",
            headers: { "Content-Type": "application/json" },
            body: JSON.stringify(payload)
        });

        // Procesar la respuesta JSON
        if (!response.ok) {
            const errorData = await response.json();
            dom.output.innerHTML = `<p style="color:red;">Error: ${errorData.error}</p>`;
            return;
        }

        const data = await response.json();

        // Guardar los roles generados en el estado
        state.roles = data.roles.map(r => ({
            nombreSala: r.nombreSala,
            nombreGuia: r.nombreGuia,
            nombreCambioInterno: r.nombreCambioInterno,
            nombreCambioExterno: r.nombreCambioExterno
        }));

        // Mostrar roles en el DOM
        dom.rolesContainer.classList.remove("hidden");
        dom.btnRol.classList.add("hidden");
        render.roles();

    } catch (error) {
        dom.output.innerHTML = `<p style="color:red;">Error al generar roles: ${error.message}</p>`;
    }
}


  const exportarCSV = () => {
    let csvContent = "\uFEFFSala;Guía;Cambio Interno;Cambio Externo\n";
    state.roles.forEach(r => csvContent += `${r.nombreSala};${r.nombreGuia};${r.nombreCambioInterno};${r.nombreCambioExterno}\n`);

    const blob = new Blob([csvContent], { type: "text/csv;charset=utf-8;" });
    const link = document.createElement("a");
    link.href = URL.createObjectURL(blob);
    link.setAttribute("download", "rol_dia.csv");
    document.body.appendChild(link);
    link.click();
    document.body.removeChild(link);
  };
// =====================================================
// 🚀 Inicialización de la aplicación
// Llama a la función de eventos y confirma en consola que la app está lista
// =====================================================
  const initEvents = () => {
    dom.agregarGuia.onclick = () => { state.actual.guia = null; forms.abrirGuia(); };
    dom.agregarSala.onclick = () => { state.actual.sala = null; forms.abrirSala(); };
    dom.agregarCapacitacion.onclick = () => { state.actual.capacitacion = null; forms.abrirCapacitacion(); };
    dom.agregarCambio.onclick = () => {
  // Ocultar el botón y mostrar el formulario
  dom.agregarCambio.classList.add("hidden");
  dom.cambiosForm.classList.remove("hidden");

  // Obtener los select dentro del formulario
  const selects = dom.cambiosForm.querySelectorAll("select");
  const selectMañana = selects[0];
  const selectTarde = selects[1];

  // Limpiar opciones anteriores
  selectMañana.innerHTML = '<option value="" disabled selected>Guía mañana</option>';
  selectTarde.innerHTML = '<option value="" disabled selected>Guía tarde</option>';

  // Filtrar guías por turno
  const guiasMañana = state.guias.filter(g => g.turno.includes("mañana") || g.turno.includes("entreSemana"));
  const guiasTarde = state.guias.filter(g => g.turno.includes("tarde") || g.turno.includes("entreSemana"));

  // Agregar las opciones de mañana
  guiasMañana.forEach(g => {
    const option = document.createElement("option");
    option.value = g.nombre;
    option.textContent = g.nombre;
    selectMañana.appendChild(option);
  });

  // Agregar las opciones de tarde
  guiasTarde.forEach(g => {
    const option = document.createElement("option");
    option.value = g.nombre;
    option.textContent = g.nombre;
    selectTarde.appendChild(option);
  });
};
    
    dom.btnRol.onclick = ()  => { dom.output.classList.add("hidden"); forms.abrirRol(); };

    dom.guardarGuia.onclick = crud.guardarGuia;
    dom.guardarSala.onclick = crud.guardarSala;
    dom.guardarCapacitacion.onclick = crud.guardarCapacitacion;
    dom.guardarCambio.onclick = crud.guardarCambio;
    dom.cancelarCambio.onclick = () => {dom.cambiosForm.classList.add("hidden"); dom.agregarCambio.classList.remove("hidden");}
    dom.cancelarRol.onclick = () => {dom.output.classList.remove("hidden"); dom.rolEditPage.classList.add("hidden");}

    dom.btnGuias.onclick = () => { dom.guiasPage.classList.remove("hidden"); render.guias(); };
    dom.btnSalas.onclick = () => { dom.salasPage.classList.remove("hidden"); render.salas(); };
    dom.btnCapacitaciones.onclick = () => { dom.capacitacionesPage.classList.remove("hidden"); render.capacitaciones(); };

    dom.cerrarBtns.forEach(btn => btn.onclick = () => btn.closest(".adminPage").classList.add("hidden"));

    dom.guardarRol.onclick = () => {

      // ==========================
      // OPERADORES (máx 2)
      // ==========================
      const ops = Array.from(
        document.querySelectorAll("#editOperadores input[type='checkbox']:checked")
      ).map(c => c.value);

      state.operadores.operador1 = ops[0] || "";
      state.operadores.operador2 = ops[1] || "";

      // ==========================
      // VACACIONES (máx 2)
      // ==========================
      const vacs = Array.from(
        document.querySelectorAll("#editVacaciones input[type='checkbox']:checked")
      ).map(c => c.value);

      state.vacaciones.vacacion1 = vacs[0] || "";
      state.vacaciones.vacacion2 = vacs[1] || "";

      // ==========================
      // CERRAR + GENERAR
      // ==========================
      dom.output.classList.remove("hidden");
      dom.rolEditPage.classList.add("hidden");

      storage.guardar();
      generarRoles();
    };
    dom.exportBtn.onclick = () => {
      state.rolesAnteriores[state.turno] = JSON.parse(JSON.stringify(state.roles));
      exportarCSV();
      dom.output.classList.add("hidden")
    }
      
      
    dom.cancelBtn.onclick = () => { dom.rolesContainer.classList.add("hidden"); dom.btnRol.classList.remove("hidden"); };

    dom.rolSlider.oninput = () => {
      const val = Number(dom.rolSlider.value);

      // 1️⃣ Guardar en el estado
      state.valor = val;

      // 2️⃣ Actualizar UI
      dom.sliderValue.textContent = val;

      const porcentaje = (val / dom.rolSlider.max) * 100;
      dom.rolSlider.style.background =
        `linear-gradient(to right, #FFCC33 ${porcentaje - 10}%, #ddd ${porcentaje}%)`;
    };

    dom.editTurno.onchange = () => {
      state.turno = dom.editTurno.value;
      forms.abrirRol()}

    document.addEventListener("click", e => {
      if (e.target.matches(".editar")) {
        const i = parseInt(e.target.dataset.index);
        const type = e.target.dataset.type;
        state.actual[type] = type === "capacitacion" ? state.capacitaciones[i] : state[type + "s"][i];
        if (type === "guia") forms.abrirGuia();
        if (type === "sala") forms.abrirSala();
        if (type === "capacitacion") forms.abrirCapacitacion();
      }
      if (e.target.matches(".eliminar")) {
        const i = parseInt(e.target.dataset.index);
        const type = e.target.dataset.type;
        if (confirm(`¿Deseas eliminar ${type}?`)) {
          if (type === "capacitacion") state.capacitaciones.splice(i, 1);
          else state[type + "s"].splice(i, 1);
          render[type + "s"] ? render[type + "s"]() : render.capacitaciones();
          storage.guardar();
        }
      }
    });
  };

  return {
    init: () => {
      storage.cargar();
      dom.editTurno.value = state.turno;
      dom.rolSlider.value = state.valor;
      dom.sliderValue.textContent = state.valor;

      const porcentaje = (state.valor / dom.rolSlider.max) * 100;
      dom.rolSlider.style.background =
        `linear-gradient(to right, #FFCC33 ${porcentaje - 10}%, #ddd ${porcentaje}%)`;

      initEvents();
      console.log("✅ App iniciada");
    }
  };
})();

App.init();
