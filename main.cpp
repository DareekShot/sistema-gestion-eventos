#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

struct Persona {
  int dni;
  string nombre;
  string tipo;
  string zona;
  int prioridad;

  Persona() {
    dni = -1;
    nombre = "";
    tipo = "";
    zona = "";
    prioridad = 0;
  }

  Persona(int i, string n, string t, string z) {
    dni = i;
    nombre = n;
    tipo = t;
    zona = z;
    prioridad = obtenerPrioridad(t);
  }

  static int obtenerPrioridad(string tipo) {
    if (tipo == "publico general") {
      return 1;
    }
    if (tipo == "discapacitado") {
      return 2;
    }
    if (tipo == "seguridad") {
      return 3;
    }
    if (tipo == "medico") {
      return 4;
    }
    if (tipo == "VIP") {
      return 5;
    }
    return 0;
  }
};

struct NodoPersona {
  Persona *persona;
  NodoPersona *siguiente;

  NodoPersona(Persona *p) {
    persona = p;
    siguiente = nullptr;
  }
};

class ListaGlobalPersonas {
public:
  NodoPersona *inicio;

  ListaGlobalPersonas() { inicio = nullptr; }

  void agregar(Persona *p) {
    NodoPersona *nuevo = new NodoPersona(p);
    nuevo->siguiente = inicio;
    inicio = nuevo;
  }

  Persona *buscar(int dni) {
    NodoPersona *actual = inicio;
    while (actual) {
      if (actual->persona->dni == dni)
        return actual->persona;
      actual = actual->siguiente;
    }
    return nullptr;
  }
};

class TablaHash {
private:
  Persona **tabla;
  int tam;

public:
  TablaHash(int t) {
    tam = t;
    tabla = new Persona *[tam];
    for (int i = 0; i < tam; i++) {
      tabla[i] = nullptr;
    }
  }

  int funcionHash(int clave) { return abs(clave) % tam; }

  void insertar(Persona *persona) {
    int pos = funcionHash(persona->dni);
    while (tabla[pos] != nullptr && tabla[pos]->dni != persona->dni) {
      pos = (pos + 1) % tam;
    }
    tabla[pos] = persona;
  }

  Persona *buscar(int clave) {
    int pos = funcionHash(clave);
    int inicio = pos;
    while (tabla[pos]) {
      if (tabla[pos]->dni == clave) {
        return tabla[pos];
      }
      pos = (pos + 1) % tam;
      if (pos == inicio) {
        break;
      }
    }
    return nullptr;
  }

  void mostrar() {
    bool hayDatos = false;
    cout << "\nTabla Hash:\n";
    int mostrados = 0;
    for (int i = 0; i < tam && mostrados < 10; i++) {
      if (tabla[i]) {
        cout << "DNI: " << tabla[i]->dni << " | Nombre: " << tabla[i]->nombre
             << endl;
        hayDatos = true;
        mostrados++;
      }
    }
    if (!hayDatos) {
      cout << "\nNo hay datos en la tabla hash.\n";
    }
  }
};

class MaxHeap {
private:
  Persona *heap[50000];
  int size;

public:
  MaxHeap() { size = 0; }

  void heapifyUp(int i) {
    while (i > 0 && heap[i]->prioridad > heap[(i - 1) / 2]->prioridad) {
      swap(heap[i], heap[(i - 1) / 2]);
      i = (i - 1) / 2;
    }
  }

  void heapifyDown(int i) {
    int max = i;
    int izquierda = 2 * i + 1;
    int derecha = 2 * i + 2;

    if (izquierda < size && heap[izquierda]->prioridad > heap[max]->prioridad) {
      max = izquierda;
    }
    if (derecha < size && heap[derecha]->prioridad > heap[max]->prioridad) {
      max = derecha;
    }
    if (max != i) {
      swap(heap[i], heap[max]);
      heapifyDown(max);
    }
  }

  void insertar(Persona *p) {
    heap[size] = p;
    heapifyUp(size);
    size++;
  }

  Persona *extraerMax() {
    if (size == 0) {
      return nullptr;
    }
    Persona *max = heap[0];
    heap[0] = heap[--size];
    heapifyDown(0);
    return max;
  }

  bool vacio() { return size == 0; }


  void mostrarTop5() {
    MaxHeap copia;
    for (int i = 0; i < size; i++) {
        copia.insertar(new Persona(*heap[i]));
    }

    cout << "\n--- Top 5 Prioridades ---\n";
    for (int i = 0; i < 5 && !copia.vacio(); i++) {
        Persona* p = copia.extraerMax();
        cout << i + 1 << ". " << p->nombre << " (" << p->tipo
            << ") Prioridad: " << p->prioridad << endl;
        delete p;
    }
  }

  void actualizarPrioridad() {
    int dniBuscado;
    string nuevoTipo;
    cout << "Ingrese el DNI de la persona a actualizar: ";
    cin >> dniBuscado;
    cout << "Ingrese el nuevo tipo (VIP, medico, seguridad, discapacitado, publico general): ";
    cin.ignore();
    getline(cin, nuevoTipo);

    for (int i = 0; i < size; i++) {
        if (heap[i]->dni == dniBuscado) {
            heap[i]->tipo = nuevoTipo;
            heap[i]->prioridad = Persona::obtenerPrioridad(nuevoTipo);
            cout << "\nPrioridad actualizada correctamente:\n";
            cout << "DNI: " << heap[i]->dni << ", Nombre: " << heap[i]->nombre
                << ", Nuevo Tipo: " << heap[i]->tipo
                << ", Nueva Prioridad: " << heap[i]->prioridad << endl;
            return;
        }
    }
    cout << "No se encontro el DNI ingresado en el heap.\n";
  }
};

class NodoAcceso {
private:
  string zona, hora;
  // Persona persona;
  int cantidad, altura;
  NodoAcceso *izquierda, *derecha;

public:
  NodoAcceso(string zona, string hora) { // , Persona persona
    this->zona = zona;
    this->hora = hora;
    // this->persona = persona;
    this->cantidad = 1;
    this->altura = 1;
    this->izquierda = nullptr;
    this->derecha = nullptr;
  }
  string getZona() { return zona; }
  string getHora() { return hora; }
  // Persona getPersona() { return persona; }
  int getCantidad() { return cantidad; }
  int getAltura() { return altura; }
  NodoAcceso *getIzquierda() { return izquierda; }
  NodoAcceso *getDerecha() { return derecha; }

  void setZona(string valor) { zona = valor; }
  void setHora(string valor) { hora = valor; }
  // void setPersona(Persona valor) { persona = valor; }
  void setCantidad(int valor) { cantidad = valor; }
  void setAltura(int valor) { altura = valor; }
  void setIzquierda(NodoAcceso *nodo) { izquierda = nodo; }
  void setDerecha(NodoAcceso *nodo) { derecha = nodo; }
};

// Arbol AVL
// =======================================================================
class ArbolRegistroAcceso {
private:
  NodoAcceso *raiz;
  static const int MAX_ZONAS = 5;
  string zonas[MAX_ZONAS] = {"A", "B", "C", "D", "E"};

public:
  ArbolRegistroAcceso() { this->raiz = nullptr; }

  // Altura y Balance
  int altura(NodoAcceso *nodo) { return nodo ? nodo->getAltura() : 0; }

  int factorBalance(NodoAcceso *nodo) {
    return nodo ? altura(nodo->getIzquierda()) - altura(nodo->getDerecha()) : 0;
  }

  void actualizarAltura(NodoAcceso *nodo) {
    int alt = 1 + max(altura(nodo->getIzquierda()), altura(nodo->getDerecha()));
    nodo->setAltura(alt);
  }

  // Balanceo
  bool esBalanceado() { return esBalanceado(raiz); }
  bool esBalanceado(NodoAcceso *nodo) {
    if (nodo == nullptr) {
      return true;
    }
    int fb = factorBalance(nodo);
    if (fb < -1 || fb > 1) {
      return false;
    }
    bool izq = esBalanceado(nodo->getIzquierda());
    bool der = esBalanceado(nodo->getDerecha());
    return izq && der;
  }

  // Utils
  bool esMenor(string zona, string hora, NodoAcceso *nodo) {
    if (!nodo || nodo == nullptr)
      return false;
    return zona < nodo->getZona() ||
           (zona == nodo->getZona() && hora < nodo->getHora());
  }

  // Insertar y Rotaciones
  void insertarR(string zona, string hora) { // , Persona persona
    insertar(raiz, zona, hora);
  }

  NodoAcceso *rotacionIzquierda(NodoAcceso *actualRaiz) {
    NodoAcceso *nuevaRaiz = actualRaiz->getDerecha();
    NodoAcceso *subArbol = nuevaRaiz->getIzquierda();

    nuevaRaiz->setIzquierda(actualRaiz);
    actualRaiz->setDerecha(subArbol);

    actualizarAltura(actualRaiz);
    actualizarAltura(nuevaRaiz);

    return nuevaRaiz;
  }

  NodoAcceso *rotacionDerecha(NodoAcceso *actualRaiz) {
    NodoAcceso *nuevaRaiz = actualRaiz->getIzquierda();
    NodoAcceso *subArbol = nuevaRaiz->getDerecha();

    nuevaRaiz->setDerecha(actualRaiz);
    actualRaiz->setIzquierda(subArbol);

    actualizarAltura(actualRaiz);
    actualizarAltura(nuevaRaiz);

    return nuevaRaiz;
  }

  void insertar(NodoAcceso *&nodo, string zona,
                string hora) { // , Persona persona
    if (nodo == nullptr) {
      nodo = new NodoAcceso(zona, hora);
    } else if (zona == nodo->getZona() && hora == nodo->getHora()) {
      nodo->setCantidad(nodo->getCantidad() + 1);
      return;
    } else if (esMenor(zona, hora, nodo)) {
      NodoAcceso *tmp = nodo->getIzquierda();
      insertar(tmp, zona, hora);
      nodo->setIzquierda(tmp);
    } else {
      NodoAcceso *tmp = nodo->getDerecha();
      insertar(tmp, zona, hora);
      nodo->setDerecha(tmp);
    }

    actualizarAltura(nodo);
    int facBalance = factorBalance(nodo);

    // der - der
    if (facBalance < -1 && !esMenor(zona, hora, nodo->getDerecha())) {
      nodo = rotacionIzquierda(nodo);
    }
    // izq - izq
    if (facBalance > 1 && esMenor(zona, hora, nodo->getIzquierda())) {
      nodo = rotacionDerecha(nodo);
    }
    // izq - der
    if (facBalance > 1 && !esMenor(zona, hora, nodo->getIzquierda())) {
      NodoAcceso *tmp = rotacionIzquierda(nodo->getIzquierda());
      nodo->setIzquierda(tmp);
      nodo = rotacionDerecha(nodo);
    }
    // der - izq
    if (facBalance < -1 && esMenor(zona, hora, nodo->getDerecha())) {
      NodoAcceso *tmp = rotacionDerecha(nodo->getDerecha());
      nodo->setDerecha(tmp);
      nodo = rotacionIzquierda(nodo);
    }
  }

  // Busqueda
  bool buscarR(string zona, string hora) { return buscar(raiz, zona, hora); }

  bool buscar(NodoAcceso *nodo, string zona, string hora) {
    if (nodo == nullptr)
      return false;
    if (hora == nodo->getHora() && zona == nodo->getZona())
      return true;
    if (esMenor(zona, hora, nodo))
      return buscar(nodo->getIzquierda(), zona, hora);
    else
      return buscar(nodo->getDerecha(), zona, hora);
  }

  // Vistas
  void accesoPreorderR() { accesoPreorder(raiz); }

  void accesoInorderR() { accesoInorder(raiz); }

  void accesoPostorderR() { accesoPostorder(raiz); }

  void accesoPreorder(NodoAcceso *nodo) {
    if (nodo != nullptr) {
      string formattedHora = nodo->getHora();
      cout << "Zona: " << nodo->getZona() << " -> Hora: " << nodo->getHora()
           << " -> (" << nodo->getCantidad() << ")" << endl;
      accesoPreorder(nodo->getIzquierda());
      accesoPreorder(nodo->getDerecha());
    }
  }

  void accesoInorder(NodoAcceso *nodo) {
    if (nodo != nullptr) {
      accesoInorder(nodo->getIzquierda());
      cout << "Zona: " << nodo->getZona() << " -> Hora: " << nodo->getHora()
           << " -> (" << nodo->getCantidad() << ")" << endl;
      accesoInorder(nodo->getDerecha());
    }
  }

  void accesoPostorder(NodoAcceso *nodo) {
    if (nodo != nullptr) {
      accesoPostorder(nodo->getIzquierda());
      accesoPostorder(nodo->getDerecha());
      cout << "Zona: " << nodo->getZona() << " -> Hora: " << nodo->getHora()
           << " -> (" << nodo->getCantidad() << ")" << endl;
    }
  }

  // Contadores
  // Cantidad Accesos Total
  int cantidadAccesosTotal() { return cantidadAccesosTotal(raiz); }

  int cantidadAccesosTotal(NodoAcceso *nodo) {
    if (nodo == nullptr) {
      return 0;
    } else {
      int izq = cantidadAccesosTotal(nodo->getIzquierda());
      int der = cantidadAccesosTotal(nodo->getDerecha());
      return nodo->getCantidad() + izq + der;
    }
  }

  // Cantidad Total por Zona
  void cantidadPorZona(string zona) {
    cout << "Cantidad Por Zona " << zona << " (" << cantidadPorZona(raiz, zona)
         << ")" << endl;
  }

  int cantidadPorZona(NodoAcceso *nodo, string zona) {
    if (nodo == nullptr) {
      return 0;
    } else {
      int izq = cantidadPorZona(nodo->getIzquierda(), zona);
      int der = cantidadPorZona(nodo->getDerecha(), zona);
      int actual = zona == nodo->getZona() ? nodo->getCantidad() : 0;

      return actual + izq + der;
    }
  }

  // Cantidad Total por Hora
  void cantidadPorHora(string hora) {
    cout << "Cantidad Por Hora " << hora << " (" << cantidadPorHora(raiz, hora)
         << ")" << endl;
  }

  int cantidadPorHora(NodoAcceso *nodo, string hora) {
    if (nodo == nullptr) {
      return 0;
    } else {
      int izq = cantidadPorHora(nodo->getIzquierda(), hora);
      int der = cantidadPorHora(nodo->getDerecha(), hora);
      int actual = hora == nodo->getHora() ? nodo->getCantidad() : 0;

      return actual + izq + der;
    }
  }

  // Cantidad Total por Zona y Hora
  int cantidadPorZonaHora(string zona, string hora) {
    return cantidadPorZonaHora(raiz, zona, hora);
  }

  int cantidadPorZonaHora(NodoAcceso *nodo, string zona, string hora) {
    if (nodo == nullptr)
      return 0;
    if (hora == nodo->getHora() && zona == nodo->getZona())
      return nodo->getCantidad();
    if (esMenor(zona, hora, nodo))
      return cantidadPorZonaHora(nodo->getIzquierda(), zona, hora);
    else
      return cantidadPorZonaHora(nodo->getDerecha(), zona, hora);
  }

  // Cantidad por Zonas
  void cantidadZonas() {
    int cantZonas[MAX_ZONAS] = {0};
    cantidadZonas(raiz, cantZonas);

    cout << "Cantidad por Zona: " << endl;
    for (int i = 0; i < MAX_ZONAS; i++) {
      cout << "Zona " << zonas[i] << " (" << cantZonas[i] << ")" << endl;
    }
  }

  void cantidadZonas(NodoAcceso *nodo, int *contadores) {
    if (nodo == nullptr)
      return;
    if (nodo->getZona() == zonas[0]) {
      contadores[0] += nodo->getCantidad();
    } else if (nodo->getZona() == zonas[1]) {
      contadores[1] += nodo->getCantidad();
    } else if (nodo->getZona() == zonas[2]) {
      contadores[2] += nodo->getCantidad();
    } else if (nodo->getZona() == zonas[3]) {
      contadores[3] += nodo->getCantidad();
    } else if (nodo->getZona() == zonas[4]) {
      contadores[4] += nodo->getCantidad();
    }

    cantidadZonas(nodo->getIzquierda(), contadores);
    cantidadZonas(nodo->getDerecha(), contadores);
  }

  // Consulta por franja horario
  void franjaHoraR(string horaMin, string horaMax) {
    cout << "Accesos entre " << horaMin << " y " << horaMax << endl;
    return franjaHora(raiz, horaMin, horaMax);
  }

  void franjaHora(NodoAcceso *nodo, string horaMin, string horaMax) {
    if (nodo != nullptr) {
      if (nodo->getHora() >= horaMin) {
        franjaHora(nodo->getIzquierda(), horaMin, horaMax);
      }
      if (horaMin <= nodo->getHora() && horaMax >= nodo->getHora()) {
        cout << "Zona: " << nodo->getZona() << " -> Hora: " << nodo->getHora()
             << " (" << nodo->getCantidad() << ")" << endl;
      }
      if (nodo->getHora() <= horaMax) {
        franjaHora(nodo->getDerecha(), horaMin, horaMax);
      }
    }
  }

  // Estadisticas
  // Top Zona
  void topZonasR(int limit) {
    if (limit < 0 || limit > MAX_ZONAS) {
      cout << "ERROR: El limit es menor a cero o excede la cant. de zonas"
           << endl;
      return;
    }

    int *topCant = new int[MAX_ZONAS]{0};
    string *topZonas = zonas;
    topNZonas(raiz, topCant);
    sortTopZonas(topCant, topZonas, MAX_ZONAS);

    cout << "Top " << limit << " Zonas por cantidad: " << endl;
    for (int i = 0; i < limit; i++) {
      cout << "\t" << i + 1 << ". Zona " << topZonas[i] << " (" << topCant[i]
           << ")" << endl;
    }
  }

  void topNZonas(NodoAcceso *nodo, int *cants) {
    if (nodo == nullptr)
      return;
    if (nodo->getZona() == zonas[0]) {
      cants[0] += nodo->getCantidad();
    } else if (nodo->getZona() == zonas[1]) {
      cants[1] += nodo->getCantidad();
    } else if (nodo->getZona() == zonas[2]) {
      cants[2] += nodo->getCantidad();
    } else if (nodo->getZona() == zonas[3]) {
      cants[3] += nodo->getCantidad();
    } else if (nodo->getZona() == zonas[4]) {
      cants[4] += nodo->getCantidad();
    }

    topNZonas(nodo->getIzquierda(), cants);
    topNZonas(nodo->getDerecha(), cants);
  }

  // Sorts
  void sortTopZonas(int *list, string *zonas, int size) {
    // Bubble Sort
    for (int i = 0; i < size - 1; i++) {
      for (int j = 0; j < size - 1 - i; j++) {
        if (list[j] < list[j + 1]) {
          int temp = list[j];
          list[j] = list[j + 1];
          list[j + 1] = temp;

          string zonaTemp = zonas[j];
          zonas[j] = zonas[j + 1];
          zonas[j + 1] = zonaTemp;
        }
      }
    }
  }
};

void leerPersonas(ListaGlobalPersonas &lista, TablaHash &tabla) {
  ifstream archivo("personas.txt");
  string linea;
  while (getline(archivo, linea)) {
    stringstream ss(linea);
    string dniStr, nombre, tipo, zona;
    getline(ss, dniStr, ',');
    getline(ss, nombre, ',');
    getline(ss, tipo, ',');
    getline(ss, zona);
    int dni = stoi(dniStr);
    Persona *p = new Persona(dni, nombre, tipo, zona);
    lista.agregar(p);
    tabla.insertar(p);
  }
  cout << "\nPersonas cargadas correctamente.\n";
}

void leerParticipantes(MaxHeap &heap, TablaHash &tabla) {
  ifstream archivo("participantes.txt");
  string linea;
  while (getline(archivo, linea)) {
    int dni = stoi(linea);
    Persona *p = tabla.buscar(dni);
    if (p) {
      heap.insertar(p);
    }
  }
  cout << "\nParticipantes cargados al heap.\n";
}

void procesarIngreso(MaxHeap &heap, ArbolRegistroAcceso &arbol) {
  int cantidad;
  cout << "Cuantas personas desea ingresar?: ";
  cin >> cantidad;
  for (int i = 0; i < cantidad && !heap.vacio(); i++) {
    Persona *p = heap.extraerMax();
    time_t now = time(0);
    tm *ltm = localtime(&now);
    char hora[6];
    snprintf(hora, sizeof(hora), "%02d:%02d", ltm->tm_hour, ltm->tm_min);
    arbol.insertarR(p->zona, hora);
    cout << p->nombre << " ingreso a zona " << p->zona << " a las " << hora
         << endl;
  }
}

int main() {
    ListaGlobalPersonas lista;
    TablaHash tabla(50000);
    MaxHeap heap;
    ArbolRegistroAcceso arbol;
    int op;
    do {
        cout << "\n========= MENU DEL SISTEMA DE INGRESO =========\n";
        cout << "1. Registrar personas\n";
        cout << "2. Cargar participantes\n";
        cout << "3. Ingreso al evento\n";
        cout << "4. Ver tabla hash\n";
        cout << "5. Ver Top 5 heap\n";
        cout << "6. Ver arbol AVL\n";
        cout << "7. Consultar accesos por franja horaria\n";
        cout << "8. Actualizar prioridad de una persona\n";
        cout << "9. Salir\n";
        cout << "Ingrese una opcion: ";
        cin >> op;
        switch (op) {
            case 1:
                leerPersonas(lista, tabla);
                break;
            case 2:
                leerParticipantes(heap, tabla);
                break;
            case 3:
                procesarIngreso(heap, arbol);
                break;
            case 4:
                tabla.mostrar();
                break;
            case 5:
                heap.mostrarTop5();
                break;
            case 6:
                arbol.accesoInorderR();
                break;
            case 7: {
                string hInicio, hFin;
                cout << "Ingrese hora inicio (HH:MM): ";
                cin >> hInicio;
                cout << "Ingrese hora fin (HH:MM): ";
                cin >> hFin;
                arbol.franjaHoraR(hInicio, hFin);
                break;
            }
            case 8:
                heap.actualizarPrioridad();
                break;
            case 9:
                cout << "\nSaliendo del sistema.\n";
                break;

            default:
                cout << "\nOpcion invalida. Intente nuevamente.\n";
                break;
        }
    } while (op != 9);
    return 0;
}
