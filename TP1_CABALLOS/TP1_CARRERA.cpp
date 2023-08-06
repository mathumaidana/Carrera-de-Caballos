#include <stdio.h>
#include <iostream>
#include <list>
#include <vector>
#include <algorithm>
#include <chrono>
#include <math.h>
#include <string>
#include <cmath>
#include <sys/wait.h>
#include <sys/types.h>
#include <cstdlib>
#include <ctime>
#include <random>
#include <tuple>
#include <iomanip>
#include "caballo_correr.h"
using namespace std;
using namespace chrono;

// definir variables a utilizar en el futuro
int cont_carrera = 1;
int carrera_caballo_rapido{}, carrera_caballo_lento{}, caballo_pedido{};
int cant_caballos;
string accion;
vector<tuple<int, string, int>> lista_caballos_terminados;
tuple<int, string, int, int> caballo_rapido, caballo_lento;
bool primera_vuelta = true;
bool menu = true;

// vector que contiene los caballos posibles
vector<string> caballos = {"Cuarto de Milla",
                           "Rayo",
                           "Lineal",
                           "Exponencial",
                           "Duoballo",
                           "Crono"};

vector<std::tuple<int, string, int>> simular_carrera(vector<string> lista_caballos, int cant_caballos)
{
    vector<tuple<int, string, int>> lista_caballos_terminados;

    // por cada caballo, iniciar un "timer"
    for (int i = 0; i < cant_caballos; i++)
    {
        auto now = time_point_cast<milliseconds>(system_clock::now());
        auto seed = now.time_since_epoch().count();
        srand(seed);

        string tipo_caballo = lista_caballos[i];
        // parent crea proceso hijo
        pid_t pid = fork();

        // verificar que el hijo ha sido creado correctamente
        if (pid == -1)
        {
            cout << "error";
            exit(1);
        }
        // varación para hijos
        else if (pid == 0)
        {
            int duracion_caballo = caballo_correr(tipo_caballo, i);
            exit(duracion_caballo);
        }
        // variación para padre
        else
        {
            int status;
            wait(&status);
            std::tuple<int, string, int> caballo_info;
            if (WIFEXITED(status))
            {

                int duracion = WEXITSTATUS(status);
                caballo_info = make_tuple(i, tipo_caballo, duracion);
            }
            lista_caballos_terminados.push_back(caballo_info);
        }
    }
    return lista_caballos_terminados;
}

vector<string> crear_lista_de_corredores(int cant_caballos, vector<string> caballos)
{
    // generar caballos random y añadirlos al vector
    vector<string> lista_caballos;
    for (int x = 0; x < cant_caballos; x++)
    {
        int n;
        n = rand() % 6;
        lista_caballos.push_back(caballos[n]);
    };
    return lista_caballos;
}

void checkear_records()
{
    // si es la primera carrera, settear el primer y último lugar
    if (cont_carrera == 1)
    {
        caballo_rapido = tuple_cat(lista_caballos_terminados[0], make_tuple(cont_carrera));
        caballo_lento = tuple_cat(lista_caballos_terminados[cant_caballos - 1], make_tuple(cont_carrera));
        return;
    }

    // comparar los récords actuales con el primer y último lugar de la carrera anterior
    if (get<2>(lista_caballos_terminados[0]) < get<2>(caballo_rapido))
    {
        caballo_rapido = tuple_cat(lista_caballos_terminados[0], make_tuple(cont_carrera));
    }
    if (get<2>(lista_caballos_terminados[cant_caballos - 1]) > get<2>(caballo_lento))
    {
        caballo_lento = tuple_cat(lista_caballos_terminados[cant_caballos - 1], make_tuple(cont_carrera));
    }
}

void mostrar_resultados()
{
    // recorrer la lista de posiciones
    // mostrando individualmente el lugar, posición y tipo de caballo
    cout << "\033[4mTabla de Posiciones\033[0m\n"
         << endl;
    for (int i = 0; i < lista_caballos_terminados.size(); i++)
    {
        tuple<int, string, int> caballo_tupla = lista_caballos_terminados[i];

        cout << setw(5) << (i + 1) << "° Lugar: ";
        cout << setw(20) << get<1>(caballo_tupla) << " con:  ";
        cout << setw(5) << get<2>(caballo_tupla) << "s!" << endl;
    }
    cout << endl;
}

void iniciar_carrera()
{
    while (true)
    {
        cout << "Ingrese la cantidad de caballos que van a correr: ";
        if (cin >> cant_caballos)
        {
            if (cant_caballos >= 1)
                break;
        }
        cout << "Input no válido. Debe ser un número mayor o igual a 1. " << endl;
        cin.clear();                                         // Clear any error flags on cin
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard any invalid input
    }

    cout << endl;

    // mediante la cantidad elegida, generar la lista y simular la carrera
    vector<string> lista_caballos = crear_lista_de_corredores(cant_caballos, caballos);
    lista_caballos_terminados = simular_carrera(lista_caballos, cant_caballos);

    // ordenar los resultados por tiempo, de menor a mayor
    std::sort(lista_caballos_terminados.begin(), lista_caballos_terminados.end(),
              [](const auto &a, const auto &b)
              {
                  return std::get<2>(a) < std::get<2>(b);
              });

    // verificar si los récords han sido superados
    checkear_records();
    ++cont_carrera;

    mostrar_resultados();
};

void mostrar_menu()
{
    cout << "Menú:" << endl
         << "1. Ver más rápido" << endl
         << "2. Ver más lento" << endl
         << "3. Correr de nuevo" << endl
         << "4. Ver caballo específico" << endl
         << "5. Mostrar resultados" << endl
         << "6. Salir" << endl
         << endl;

    cout << "Ingrese un número: ";
    cin >> accion;
    cout << endl;
}

int main()
{
    // limpiar la terminal y comenzar la simulación
    system("clear");
    cout << "Bienvenido chaval!! 🐎🏁" << endl;
    sleep(1);
    iniciar_carrera();

    while (menu)
    {
        mostrar_menu();

        // mostrar caballo más rápido
        if (accion == "1")
        {
            system("clear");
            cout << "El caballo " << get<1>(caballo_rapido) << " con el número " << get<0>(caballo_rapido) + 1 << " es el más rápido, con "
                 << get<2>(caballo_rapido) << "s en la simulación " << get<3>(caballo_rapido) << "!" << endl
                 << endl;
        }
        // mostrar caballo más lento
        else if (accion == "2")
        {
            system("clear");
            cout << "El caballo " << get<1>(caballo_lento) << " con el número " << get<0>(caballo_lento) << " es el más lento, "
                 << get<2>(caballo_lento) << "s en la simulación " << get<3>(caballo_lento) << "!" << endl
                 << endl;
        }
        else if (accion == "3")
        {
            system("clear");
            iniciar_carrera();
        }
        // mostrar data de caballo elegido
        else if (accion == "4")
        {
            int caballo_elegido;
            system("clear");

            while (true)
            {
                cout << "Ingrese la posicion del caballo que desea ver: ";
                // verificar si el input es válido
                if (cin >> caballo_elegido && caballo_elegido <= cant_caballos && caballo_elegido >= 1)
                {
                    if (caballo_elegido <= cant_caballos && caballo_elegido >= 1)
                        break;
                }
                cout << "Input no válido. Hay un total de " << cant_caballos << " caballos!" << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }

            cout << "El caballo elegido es un " << get<1>(lista_caballos_terminados[caballo_elegido - 1]) << " y acabó con una duracion de "
                 << get<2>(lista_caballos_terminados[caballo_elegido - 1]) << "s! " << endl
                 << endl;
        }
        else if (accion == "5")
        {
            system("clear");
            mostrar_resultados();
        }
        else if (accion == "6")
        {
            cout << "Gracias por jugar! 🏇" << endl;
            exit(0);
        }
        else
        {
            cout << "Valor no válido" << endl;
        }
    }
}