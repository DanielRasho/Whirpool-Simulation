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
#include "randomGenerator.h" // importing random generator funtions
using namespace std;

int main() {

    srand((unsigned) time(NULL)); //Seed Generator

    int distance; //Distance of the competition
    int estilo;

    float swimmers[8][4]; //Rows --> Swimmer    Column --> Swimming style
                            //Swimming Styles: 0 -> Estilo libre, 1 -> Dorso, 2 -> Pecho, 3 -> Mariposa

    std::cout << "Ingrese la distancia total a nadar (50/100/200/400 m): "; 
    std::cin >> distance; //Scan for distance to swim 

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

    printf("\n\nEstilos de nado:\n1: Estilo libre\n2: Dorso\n3: Pecho\n4: Mariposa\n");
    std::cout << "Ingrese el estilo de nado (1 - 4): "; 
    std::cin >> estilo; //Scan for swimming style 

    printf("Los nadadores tienen las siguientes velocidades: ");
    for (int i = 0; i < 8; i++)
    {
        printf("\nNadador %d: %f", i+1,swimmers[i][estilo-1]);
    }
    
    
    std::cout << std::endl;

    return 0;
}
