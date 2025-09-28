// ======= Datos de ejemplo =======
let guias = [
  { nombre: "Ana", turno: "mañana",capacitaciones:["Tele, Radio"] },
  { nombre: "Luis", turno: "tarde",capacitaciones:["Tele, Radio"]  },
  { nombre: "María", turno: "mañana",capacitaciones:["Tele, Radio"]  }
];

let salas = [
  { nombre: "Sala 1", horaAlmuerzo: "12:00" },
  { nombre: "Sala 2", horaAlmuerzo: "12:30" },
  { nombre: "Sala 3", horaAlmuerzo: "13:00" }
];

// ======= Manejo de páginas =======
const btnGuias = document.getElementById("btnGuias");
const btnSalas = document.getElementById("btnSalas");
const btnRol = document.getElementById("btnRol");

const guiasPage = document.getElementById("guiasPage");
const salasPage = document.getElementById("salasPage");
const output = document.getElementById("output");

const cerrarBtns = document.querySelectorAll(".cerrarPage");

// ======= Funciones Renderizado con Animación =======
function renderGuias() {
  const tbody = document.querySelector("#tablaGuias tbody");
  tbody.innerHTML = "";
  guias.forEach((g, i) => {
    const tr = document.createElement("tr");
    tr.innerHTML = `
      <td>${g.nombre}</td>
      <td>${g.turno}</td>
      <td>${g.capacitaciones}</td>
      <td>
        <button onclick="editarGuia(${i})">✏️</button>
        <button onclick="eliminarGuia(${i})">🗑️</button>
      </td>
    `;
    tr.classList.add("fade-in");
    tbody.appendChild(tr);
  });
}

function renderSalas() {
  const tbody = document.querySelector("#tablaSalas tbody");
  tbody.innerHTML = "";
    salas.forEach((s, i) => {
    const tr = document.createElement("tr");
    tr.innerHTML = `
      <td>${s.nombre}</td>
      <td>${s.horaAlmuerzo}</td>
      <td>
        <button onclick="editarSala(${i})">✏️</button>
        <button onclick="eliminarSala(${i})">🗑️</button>
      </td>
    `;
    tr.classList.add("fade-in");
    tbody.appendChild(tr);
  });
}

// ======= CRUD Guias =======
function agregarGuia() {
  const nombre = prompt("Nombre de la nueva guía:");
  const turno = prompt("Turno (mañana/tarde/fines):");
  if(nombre && turno) {
    guias.push({ nombre, turno });
    renderGuias();
  }
}

function editarGuia(i) {
  const nombre = prompt("Nuevo nombre:", guias[i].nombre);
  const turno = prompt("Nuevo turno:", guias[i].turno);
  if(nombre && turno) {
    guias[i].nombre = nombre;
    guias[i].turno = turno;
    renderGuias();
  }
}

function eliminarGuia(i) {
  const row = document.querySelectorAll("#tablaGuias tbody tr")[i];
  row.classList.add("fade-out");
  setTimeout(() => {
    guias.splice(i, 1);
    renderGuias();
  }, 300);
}

// ======= CRUD Salas =======
function agregarSala() {
  const nombre = prompt("Nombre de la sala:");
  const hora = prompt("Hora de almuerzo (ej. 12:30):");
  if(nombre && hora) {
    salas.push({ nombre, horaAlmuerzo: hora });
    renderSalas();
  }
}

function editarSala(i) {
  const nombre = prompt("Nuevo nombre:", salas[i].nombre);
  const hora = prompt("Nueva hora de almuerzo:", salas[i].horaAlmuerzo);
  if(nombre && hora) {
    salas[i].nombre = nombre;
    salas[i].horaAlmuerzo = hora;
    renderSalas();
  }
}

function eliminarSala(i) {
  const row = document.querySelectorAll("#tablaSalas tbody tr")[i];
  row.classList.add("fade-out");
  setTimeout(() => {
    salas.splice(i, 1);
    renderSalas();
  }, 300);
}

// ======= Botones Agregar =======
document.getElementById("agregarGuia").onclick = agregarGuia;
document.getElementById("agregarSala").onclick = agregarSala;

// ======= Abrir y cerrar páginas =======
btnGuias.onclick = () => { guiasPage.classList.remove("hidden"); renderGuias(); };
btnSalas.onclick = () => { salasPage.classList.remove("hidden"); renderSalas(); };
cerrarBtns.forEach(btn => btn.onclick = () => btn.closest(".adminPage").classList.add("hidden"));

// ======= Generación de Roles visual =======
btnRol.onclick = () => {
  if(guias.length === 0 || salas.length === 0){
    output.innerHTML = "<p style='color:red;'>No hay suficientes guías o salas para generar roles.</p>";
    return;
  }

  const shuffledGuias = [...guias].sort(() => Math.random() - 0.5);
  const n = Math.min(shuffledGuias.length, salas.length);

  output.innerHTML = "<h3>✨ Roles Generados ✨</h3>";

  // Botón exportar CSV
  const exportBtn = document.createElement("button");
  exportBtn.textContent = "📥 Exportar CSV";
  exportBtn.classList.add("magic-btn");
  output.appendChild(exportBtn);

  let rolesGenerados = [];

  for(let i=0; i<n; i++){
    const rolDiv = document.createElement("div");
    rolDiv.classList.add("rol-card", "fade-in");
    rolDiv.innerHTML = `
      <p>🧑‍🏫 <strong>${shuffledGuias[i].nombre}</strong></p>
      <p>🏠 <strong>${salas[i].nombre}</strong></p>
      <p>🍽 Hora Almuerzo: ${salas[i].horaAlmuerzo}</p>
    `;
    rolDiv.style.background = `hsl(${Math.random()*360}, 70%, 80%)`;
    rolDiv.style.padding = "10px";
    rolDiv.style.margin = "10px 0";
    rolDiv.style.borderRadius = "15px";
    rolDiv.style.boxShadow = "0 5px 15px rgba(0,0,0,0.2)";
    rolDiv.style.transition = "all 0.3s";
    rolDiv.onmouseover = () => rolDiv.style.transform = "scale(1.05)";
    rolDiv.onmouseleave = () => rolDiv.style.transform = "scale(1)";
    output.appendChild(rolDiv);

    rolesGenerados.push({
      sala: salas[i].nombre,
      guia: shuffledGuias[i].nombre,
      horaAlmuerzo: salas[i].horaAlmuerzo
    });
  }

  // Exportar CSV
  exportBtn.onclick = () => {
    let csvContent = "\uFEFF";
    csvContent += "Sala;Guía;Hora Almuerzo\n";
    rolesGenerados.forEach(r => {
      csvContent += `${r.sala};${r.guia};${r.horaAlmuerzo}\n`;
    });
    const blob = new Blob([csvContent], { type: "text/csv;charset=utf-8;" });
    const link = document.createElement("a");
    link.href = URL.createObjectURL(blob);
        link.setAttribute("download", "rol_dia.csv");
    document.body.appendChild(link);
    link.click();
    document.body.removeChild(link);
  };
};