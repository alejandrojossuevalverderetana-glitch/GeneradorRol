// ====================================
// 📌 SISTEMA DE ADMINISTRACIÓN DE GUÍAS, SALAS Y CAPACITACIONES
// ====================================

const App = (() => {
  const state = {
    guias: [
      { nombre: "Ana", turno: "mañana", capacitaciones: ["Tele", "Radio"] },
      { nombre: "Luis", turno: "tarde", capacitaciones: ["Tele", "Radio"] },
      { nombre: "María", turno: "mañana", capacitaciones: ["Tele", "Radio"] }
    ],
    salas: [
      { nombre: "Universo", capacitacion: "" },
      { nombre: "Tierra", capacitacion: "" },
      { nombre: "Costa Rica", capacitacion: "" },
      { nombre: "Estadio", capacitacion: "" },
      { nombre: "Radio", capacitacion: "Radio" },
      { nombre: "Television", capacitacion: "Tele" },
      { nombre: "Steam", capacitacion: "Steam" }
    ],
    capacitaciones: ["Tele", "Radio", "Steam", "Operador"],
    roles: [],
    actual: {
      guia: null,
      sala: null,
      capacitacion: null
    }
  };

  const dom = {
    guiaEditPage: document.getElementById("guiaEditPage"),
    salaEditPage: document.getElementById("salaEditPage"),
    capacitacionEditPage: document.getElementById("capacitacionEditPage"),

    btnGuias: document.getElementById("btnGuias"),
    btnSalas: document.getElementById("btnSalas"),
    btnCapacitaciones: document.getElementById("btnCapacitaciones"),
    btnRol: document.getElementById("btnRol"),

    guiasPage: document.getElementById("guiasPage"),
    salasPage: document.getElementById("salasPage"),
    capacitacionesPage: document.getElementById("capacitacionesPage"),
    output: document.getElementById("output"),

    rolesContainer: document.getElementById("rolesContainer"),
    tablaRoles: document.getElementById("tablaRoles"),
    exportBtn: document.getElementById("exportBtn"),
    cancelBtn: document.getElementById("cancelBtn"),

    agregarGuia: document.getElementById("agregarGuia"),
    agregarSala: document.getElementById("agregarSala"),
    agregarCapacitacion: document.getElementById("agregarCapacitacion"), // CORREGIDO
    guardarGuia: document.getElementById("guardarGuia"),
    guardarSala: document.getElementById("guardarSala"),
    guardarCapacitacion: document.getElementById("guardarCapacitacion"), // CORREGIDO

    cerrarBtns: document.querySelectorAll(".cerrarPage")
  };

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
          <td>${r.sala}</td>
          <td>${r.guia}</td>
        `;
        tbody.appendChild(tr);
      });
    }
  };

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
    }
  };

  const crud = {
    guardarGuia: () => {
      const nombre = document.getElementById("editNombreGuia").value.trim();
      const turno = document.getElementById("editTurno").value;
      const caps = Array.from(document.querySelectorAll("#editCapacitaciones input[type='checkbox']:checked")).map(i => i.value);
      if (!nombre) return alert("El nombre no puede estar vacío");

      if (state.actual.guia) {
        state.actual.guia.nombre = nombre;
        state.actual.guia.turno = turno;
        state.actual.guia.capacitaciones = caps;
      } else state.guias.push({ nombre, turno, capacitaciones: caps });

      dom.guiaEditPage.classList.add("hidden");
      render.guias();
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
    }
  };

  const generarRoles = () => {
    if (state.guias.length === 0 || state.salas.length === 0) {
      dom.output.innerHTML = "<p style='color:red;'>No hay suficientes guías o salas para generar roles.</p>";
      return;
    }
    const shuffledGuias = [...state.guias].sort(() => Math.random() - 0.5);
    state.roles = state.salas.map((s, i) => ({ sala: s.nombre, guia: shuffledGuias[i] ? shuffledGuias[i].nombre : "" }));

    dom.rolesContainer.classList.remove("hidden");
    dom.btnRol.classList.add("hidden");
    render.roles();
  };

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

  const initEvents = () => {
    dom.agregarGuia.onclick = () => { state.actual.guia = null; forms.abrirGuia(); };
    dom.agregarSala.onclick = () => { state.actual.sala = null; forms.abrirSala(); };
    dom.agregarCapacitacion.onclick = () => { state.actual.capacitacion = null; forms.abrirCapacitacion(); };

    dom.guardarGuia.onclick = crud.guardarGuia;
    dom.guardarSala.onclick = crud.guardarSala;
    dom.guardarCapacitacion.onclick = crud.guardarCapacitacion;

    dom.btnGuias.onclick = () => { dom.guiasPage.classList.remove("hidden"); render.guias(); };
    dom.btnSalas.onclick = () => { dom.salasPage.classList.remove("hidden"); render.salas(); };
    dom.btnCapacitaciones.onclick = () => { dom.capacitacionesPage.classList.remove("hidden"); render.capacitaciones(); };

    dom.cerrarBtns.forEach(btn => btn.onclick = () => btn.closest(".adminPage").classList.add("hidden"));

    dom.btnRol.onclick = generarRoles;
    dom.exportBtn.onclick = exportarCSV;
    dom.cancelBtn.onclick = () => { dom.rolesContainer.classList.add("hidden"); dom.btnRol.classList.remove("hidden"); };

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
        }
      }
    });
  };

  return {
    init: () => {
      initEvents();
      console.log("✅ App iniciada");
    }
  };
})();

App.init();
