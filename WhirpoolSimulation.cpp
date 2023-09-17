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

#include "randomGenerator.h" // importing random generator funtions

#if __has_include(<unistd.h>) // Importing sleep() for different OS
#endif
#if __has_include(<windows.h>)
#include <unistd.h>
#endif

#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <omp.h>
#include <random>
#include <cstdlib>
#include <time.h>
#include <unistd.h>
using namespace std;

#define PRINT_WIDTH 40
#define NUM_SWIMMERS 8

void printSwimmerPositions(int len, float position);
void clearScreen();
void semaforo();
void printMedal(const std::string& filename);


int main() {

// #######################################
// ###  INITALIZIN SWIMMERS PARAMETERS ###
// #######################################

    srand((unsigned) time(NULL)); //Seed Generator

    int distance; //Distance of the competition
    int estilo;

    float swimmers[NUM_SWIMMERS][2]; //Rows --> Swimmer    Column1 --> Swimming style Column2 --> position
                                    //Swimming Styles: 0 -> Estilo libre, 1 -> Dorso, 2 -> Pecho, 3 -> Mariposa

    std::cout << "Ingrese la distancia total a nadar (50/100/200/400 m): "; 
    std::cin >> distance; //Scan for distance to swim 
                          //
    printf("\n\nEstilos de nado:\n1: Estilo libre\n2: Dorso\n3: Pecho\n4: Mariposa\n");
    std::cout << "Ingrese el estilo de nado (1 - 4): "; 
    std::cin >> estilo; //Scan for swimming style 

    #pragma omp parallel
    {
        #pragma omp parallel for  
        for (int i = 0; i < NUM_SWIMMERS; i++)
        {
            swimmers[i][0] = randomFloat(1, 3); //Generate a random float number beetween 1 and 3
            swimmers[i][1] = 0; // Set initial position
        }        
    }

    printf("Los nadadores tienen las siguientes velocidades (m/s): ");
    for (int i = 0; i < NUM_SWIMMERS; i++)
    {
        printf("\nNadador %d: %f", i+1,swimmers[i][0]);
    }
    std::cout << std::endl;
    
// ####################
// ###  MAIN LOOP   ###
// ####################

    // TODO: Andre, change the condition to stop loop when one swimmer win.
    int tiempo = 0;
    bool statement = true;
    float pos = 0;
    semaforo();
    bool applauseShown = false; // Variable para controlar si se ha mostrado el aplauso

    while (statement == true){
        printf("Tiempo Actual: %d\n", tiempo);
        #pragma omp parallel
        {
            #pragma omp parallel for  
            for (int i = 0; i < NUM_SWIMMERS; i++)
            {
                pos = swimmers[i][0] * tiempo;
                swimmers[i][1] = pos;
                
            }
        }

        // ## PRINTING SWIMMERS POSITIONS ##
        if (statement) {
            for(int i = 0; i < NUM_SWIMMERS; i++){
                for(int j = 0; j < PRINT_WIDTH; j++){
                    printf("#");
                }
                printf("\n");
                printSwimmerPositions(distance, swimmers[i][1]);

                if (swimmers[i][1] > distance)
                {
                    statement = false;
                    clearScreen();
                    printf("   ~~~     ~~~     ~~~\n");
                    printf("~~~~   ~~~ EL GANADOR ES... ~~~   ~~~~   \n");
                    printf("  ~~~     ~~~     ~~~\n");
                    sleep(3);
                    printMedal("drumroll.txt"); // Llama a la función printMedal para mostrar la medalla
                    sleep(5);
                    clearScreen();
                
                    printf("~~~~   ~~~ EL NADADOR DEL CARRIL %d", (i+1),"~~~   ~~~~");
            
                    sleep(2);

                    printMedal("banner.txt"); // Llama a la función printMedal para mostrar un banner
                    sleep(5);
                    clearScreen();
                    
                    if (!applauseShown) {
                        printMedal("applause.txt"); // Llama a la función printMedal para mostrar aplausos
                        applauseShown = true; // Marca que se ha mostrado el aplauso
                    }
                }
            }
        }

        if (applauseShown) {
            break; // Sal del bucle principal después de mostrar el aplauso
        }

        sleep(2); //Two seconds so refresh rate of every 2 seconds
        tiempo = tiempo + 2;
        clearScreen();
    }

    return 0;
}

// Prints a representation of swimmer on its rale.
void printSwimmerPositions(int distance, float position){

    int positionInBar = (position * PRINT_WIDTH) / distance;

    for(int i = 0; i < PRINT_WIDTH; i++){
        if(i == positionInBar)
            printf("\033[1;30;41mo\033[0m"); // Print Swimmer
        else if (i < positionInBar)
            printf("=");                    // Print space swimmed
        else
            printf("-");                    // Print remaining space
    }
    printf("\n");
}

// Clear terminal screen.
void clearScreen()
{
#if defined _WIN32
    system("cls");
    //clrscr(); // including header file : conio.h
#elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
    system("clear");
    //std::cout<< u8"\033[2J\033[1;1H"; //Using ANSI Escape Sequences 
#elif defined (__APPLE__)
    system("clear");
#endif
}

//Print initial set mark go!
void semaforo()
{
    sleep(5);
    clearScreen();
    system("Color 0C");
    printf("   ~~~     ~~~     ~~~\n");
    printf("~~~~   ~~~ PREPARADOS ~~~   ~~~~   \n");
    printf("  ~~~     ~~~     ~~~\n");
    sleep(3);
    clearScreen();
    system("Color 06");
    printf("   ~~~        ~~~     ~~~\n");
    printf("~~~~   ~~ LISTOS ~~~   ~~~   \n");
    printf("  ~~~     ~~~   ~~~\n");
    sleep(3);
    clearScreen();
    system("Color 0A");
    printf("~~  ~~~     ~~~    ~~~\n");
    printf("~~~   ~~~ YA ~~~   ~~~~   \n");
    printf("  ~~~    ~~~     ~~~\n");
    sleep(3);
    system("Color 00");
    sleep(3);
    clearScreen();
}

void printMedal(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    std::string art;

    if (file.is_open()) {
        while (getline(file, line)) {
            art += line + "\n";
        }
        file.close();
    } else {
        std::cerr << "No se pudo abrir el archivo " << filename << std::endl;
        return;
    }

    // Mostrar el arte ASCII en la consola
    std::cout << art << std::endl;
}

