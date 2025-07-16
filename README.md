# Sistema de Gestión de Eventos Masivos

Proyecto final del curso **Estructuras de Datos y Algoritmos II (EDA II)** desarrollado en **C++**.

Este sistema simula el ingreso ordenado de hasta **50,000 personas** a un evento masivo utilizando estructuras de datos clásicas implementadas manualmente, sin uso de STL. Se divide en tres fases principales:

---

## 🧠 Estructuras implementadas

- ✅ **Tabla Hash:** para registrar a las personas y evitar duplicados (clave = DNI).
- ✅ **MaxHeap:** para organizar a los asistentes por prioridad (personas que llegaron primero o con prioridad especial).
- ✅ **Árbol AVL:** para registrar los accesos finales con zona y hora, y permitir búsquedas eficientes.

---

## 🧪 Archivos de entrada

- `personas.txt`: contiene 50,000 personas con datos como DNI, nombres, apellidos, zona y prioridad.
- `participantes.txt`: contiene 48,000 DNIs que simulan los asistentes que llegaron al evento.

---

## ⚙️ Compilación y ejecución

### ✅ En Windows (con g++)
```bash
g++ main.cpp -o sistema
sistema
