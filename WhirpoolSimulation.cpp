/*--------------------------------------------------------
* UNIVERSIDAD DEL VALLE DE GUATEMALA
* FACULTAD DE INGENIERÍA
* DEPARTAMENTO DE CIENCIA DE LA COMPUTACIÓN
* CC3086 Programación de microprocesadores
* Proyecto 2: Simulación de competencia de natación
* 
* WhirpoolSimulation.cpp
* Original: 
* Creado por: José Prince, Andre Jo, Irving Acosta y Daniel RAyo
* Fecha: 21/09/2023
* Descripción: Simulador de competencia de natación 
* para diferentes estilos
* --------------------------------------------------------*/

#include <iostream>
#include <omp.h>
#include <random>
#include <cstdlib>
#include <time.h>
using namespace std;

float randomFloat() //Generates a float number between 0 - 1
{
    return (float)(rand()) / (float)(RAND_MAX);
}

int randomInt(int a, int b) //Generates a integer between a - b
{
    if (a > b)
        return randomInt(b, a);
    if (a == b)
        return a;
    return a + (rand() % (b - a));
    
}

float randomFloat(int a, int b) //Gets the float number by sum the integer and the float obtained previously
{
    if (a > b)
        return randomFloat(b, a);
    if (a == b)
        return a;
    
    return (float)randomInt(a, b) + randomFloat();
}

main() {

    srand((unsigned) time(NULL)); //Seed Generator

    int distance; //Distance of the competition

    float swimmers[8][4]; //Rows --> Swimmer    Column --> Swimming style

    std::cout << "Ingrese la distancia total a nadar (50/100/200/400 m): "; 
    std::cin >> distance; //Scan for distance swum 

    #pragma omp parallel
    {
            
        #pragma omp parallel for  
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                swimmers[i][j] = randomFloat(1, 3); //Generate a random float number beetween 1 and 3
            }  
        }        
    }
    
    std::cout << std::endl;
}

