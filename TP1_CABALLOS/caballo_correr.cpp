#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <math.h>
#include <string>
#include <random>
#include "caballo_correr.h"
using namespace std;

// Función ejecutada individualmente para cada caballo que va a correr
int caballo_correr(string tipo, int id)
{
    float tiempo = 0.0;

    if (tipo == "Cuarto de Milla")
    {
        // Cada 2 segundos tiene 5 % de probabilidad de llegar a la meta
        // Generar número aleatorio entre 1 y 20
        // Si es 1 completa la carrera
        int num = rand() % 20 + 1;
        while (num != 1)
        {
            tiempo += 2;
            num = rand() % 20 + 1;
        }

        return static_cast<int>(ceil(tiempo));
    }

    else if (tipo == "Rayo")
    {
        // Cada 1 segundo tiene 3 % de probabilidad de llegar a la meta
        // Generar un número aleatorio entre 1 y 100
        // Si es 1 | 2 |  3 completa la carrera
        int num = rand() % 100 + 1;
        while (num > 3)
        {
            tiempo += 1;
            num = rand() % 100 + 1;
        }

        return static_cast<int>(ceil(tiempo));
    }

    else if (tipo == "Lineal")
    {
        /*Caballo Lineal: Cada 3 segundos tiene 2*X% de probabilidades de terminar, donde X es la cantidad
        de iteraciones que hizo hasta el momento.*/
        int num = rand() % 100 + 1;
        int cont = 1;
        while (num > (2 * cont))
        {
            tiempo += 3;
            num = rand() % 100 + 1;
            cont++;
        }
        return static_cast<int>(ceil(tiempo));
    }
    else if (tipo == "Exponencial")
    {

        /*Cada 0.5 segundos tiene 1 - 0.98X% de probabilidades de terminar, donde X
        es la cantidad de iteraciones que hizo hasta el momento. Si la probabilidad es negativa, se considera
        que el caballo llego a la meta. Ademas si el tiempo que tarda no es entero, se considera que el tiempo
        se redondea para arriba.*/
        int cont = 1;

        double probabilidad = (1.0 - pow(0.98, cont)) * 100;
        int num = rand() % 100 + 1;
        while (num > probabilidad)
        {
            tiempo += 0.5;
            num = rand() % 100 + 1;
            probabilidad = (1.0 - pow(0.98, cont)) * 100;
        }

        return static_cast<int>(ceil(tiempo));
    }
    else if (tipo == "Duoballo")
    {

        int id = getpid();
        // Si el ultimo de su PID es mayor a 5, actúa como un caballo lineal, caso contrario es un caballo exponencial
        if ((id % 10) >= 5)
        {
            // Lineal
            int num = rand() % 100 + 1;
            int cont = 1;
            while (num > (2 * cont))
            {
                tiempo += 3;
                num = rand() % 100 + 1;
                cont++;
            }
        }
        else
        {
            // Exponencial
            int cont = 1;

            double probabilidad = (1.0 - pow(0.98, cont)) * 100;
            int num = rand() % 100 + 1;
            while (num > probabilidad)
            {
                tiempo += 0.5;
                num = rand() % 100 + 1;
                probabilidad = (1.0 - pow(0.98, cont)) * 100;
            }
        }
        return static_cast<int>(ceil(tiempo));
    }

    else if (tipo == "Crono")
    {
        /*Cada X segundos el caballo tiene una probabilidad de 0.05% de finalizar, donde X
        es un numero flotante aleatorio entre 0.1 y 3.5 segundos que cambia en cada iteracion. Ademas si el
        tiempo que tarda no es entero, se considera que el tiempo se redondea para arriba.*/
        int num = rand() % 2000 + 1;
        while (num != 1)
        {
            int tiempo_random = rand() % (35 + 1);

            tiempo += (ceil(tiempo_random / 10));
            num = rand() % 2000 + 1;
        }

        return static_cast<int>(ceil(tiempo));
    }
    else
    {
        cout << "caballo inexistente";
        return 1;
    }
};
