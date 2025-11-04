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
  const state = {
    guias: [ { nombre: "Ana", turno: "mañana", capacitaciones: ["Tele", "Operador"] }, { nombre: "Pedro", turno: "mañana", capacitaciones: ["Tele", "Operador"] }, { nombre: "Juan", turno: "mañana", capacitaciones: ["Tele", "Operador"] }, { nombre: "Salem", turno: "tarde", capacitaciones: ["Tele", "Operador"] }, { nombre: "Luis", turno: "tarde", capacitaciones: ["Tele", "Radio"] }, { nombre: "María", turno: "tarde", capacitaciones: ["Tele", "Radio"] } ], 
    salas: [ { nombre: "Universo", capacitacion: "" }, { nombre: "Tierra", capacitacion: "" }, { nombre: "Costa Rica", capacitacion: "" }, { nombre: "Estadio", capacitacion: "" }, { nombre: "Radio", capacitacion: "Radio" }, { nombre: "Television", capacitacion: "Tele" }, { nombre: "Steam", capacitacion: "Steam" } ],
    capacitaciones: ["Tele", "Radio", "Steam", "Operador"],
    roles: [],
    actual: {
      guia: null,
      sala: null,
      capacitacion: null
    },
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
        <th>Guía Mañana</th>
        <th>Guía Tarde</th>
      </tr>
      ${filas}
    `;

    tabla.classList.remove("hidden");
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
      const turno = dom.editTurno.value;

      let operadores;

      if (turno.startsWith("fines")) {
        const partes = turno.split("-");
        const subTurno = partes[1];
        operadores = state.guias.filter(g => g.capacitaciones.includes("Operador") && g.turno.includes(subTurno));
      }else {
        operadores = state.guias.filter(g => g.capacitaciones.includes("Operador") && g.turno === turno || g.turno.includes("TC"));
      }

      operadores.forEach(op => {
        const label = document.createElement("label");
        label.innerHTML = `<input type="checkbox" value="${op.nombre}"> ${op.nombre}`;
        container.appendChild(label);
      });

      dom.rolEditPage.classList.remove("hidden");
      render.cambios(state.cambios, dom.cambiosContainer)
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
      if (!nombre) return alert("El nombre no puede estar vacío");

      if (state.actual.sala) {
        state.actual.sala.nombre = nombre;
        state.actual.sala.capacitacion = capacitacion;
      } else state.salas.push({ nombre, capacitacion });

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
            guias: state.guias,   // lista de guías
            salas: state.salas,   // lista de salas
            roles: state.roles,   // roles actuales, si aplica
            cambios: state.cambios // cambios si los tienes
        };

        // Petición POST al endpoint /generar de tu API
        const response = await fetch("http://araxus.ddns.net:18080/generar", {
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
            nombreGuia: r.nombreGuia
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
    let csvContent = "\uFEFFSala;Guía\n";
    state.roles.forEach(r => csvContent += `${r.sala};${r.guia}\n`);

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
  const guiasMañana = state.guias.filter(g => g.turno.includes("mañana"));
  const guiasTarde = state.guias.filter(g => g.turno.includes("tarde"));

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

    dom.guardarRol.onclick= () => {  dom.output.classList.remove("hidden");dom.rolEditPage.classList.add("hidden"); generarRoles(); };
    dom.exportBtn.onclick = exportarCSV;
    dom.cancelBtn.onclick = () => { dom.rolesContainer.classList.add("hidden"); dom.btnRol.classList.remove("hidden"); };

    dom.rolSlider.oninput = () => {
      const val = dom.rolSlider.value; 
      dom.sliderValue.textContent = val;

      const porcentaje = (val / dom.rolSlider.max) * 100;
      dom.rolSlider.style.background = `linear-gradient(to right, #FFCC33 ${porcentaje-10}%, #ddd ${porcentaje}%)`;
    };

    dom.editTurno.onchange = () => {forms.abrirRol()}

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
      initEvents();
      console.log("✅ App iniciada");
    }
  };
})();

App.init();
