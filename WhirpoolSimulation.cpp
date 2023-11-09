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
#include <random>
#include <cstdlib>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
using namespace std;

#define PRINT_WIDTH 40
#define NUM_SWIMMERS 8

void* updateSwimmerPosition(void*);

// Representation of a swimmer
struct Swimmer{
    float position;
    float velocity;
};

void printSwimmerPositions(int len, float position);
void clearScreen();
void semaforo();
void printMedal(const std::string& filename);

Swimmer swimmers[NUM_SWIMMERS];
pthread_t threads [NUM_SWIMMERS];
pthread_mutex_t lock;
time_t currentTime = time(0);
int currentTimeInt = static_cast<int>(currentTime);
std::mt19937 gen(currentTimeInt);
std::uniform_real_distribution<float> dist(1.0, 3.0);



int distance; //Distance of the competition
int estilo;
int counter;

// ####################
//  PTHREAD FUNCTIONS
// ####################

void* genswimers(void *arg)
{
    pthread_mutex_lock(&lock);		
    //Seed Generator			//establecer bloqueo antes utilizar recurso
    // Get a random number
	// Create a random number generator

    unsigned long i = 0;
    					//incialización variable utilizada para retardo
    swimmers[counter].position = 0;
    swimmers[counter].velocity = std::round(dist(gen)*100)/100.0; //poner random float
    counter += 1;			

    pthread_mutex_unlock(&lock);
    return NULL;
}

int main() {

// ########################################
// ###  INITALIZING SWIMMERS PARAMETERS ###
// ########################################
    srand((unsigned) time(NULL)); //Seed Generator
    int distance;

    std::cout << "Ingrese la distancia total a nadar (50/100/200/400 m): "; 
    std::cin >> distance; //Scan for distance to swim 
                          //
    printf("\n\nEstilos de nado:\n1: Estilo libre\n2: Dorso\n3: Pecho\n4: Mariposa\n");
    std::cout << "Ingrese el estilo de nado (1 - 4): "; 
    std::cin >> estilo; //Scan for swimming style 
    
    // GENERATING STATS
    int i = 0;
    int err;
    if (pthread_mutex_init(&lock, NULL) != 0) 						//inicializacion de mutex no completada
    {
        printf("\n mutex init failed\n");
        return 1;
    }
    while(i < NUM_SWIMMERS)													//se crean solo 2 hilos
    {
        err = pthread_create(&(threads[i]), NULL, &genswimers, NULL); 	//creacion de hilos sin paso de parametros
        if (err){
            printf("\ncan't create thread :[%s]", strerror(err));	//impresion de mensaje si el hilo no se crea correctamente
            exit(-1);
        }
        i++;
    }

    for (int i = 0; i < NUM_SWIMMERS; i++){
        // Here the thread will join
        pthread_join(threads[i], NULL);
    }

    printf("Los nadadores tienen las siguientes velocidades (m/s): ");
    for (int i = 0; i < NUM_SWIMMERS; i++)
    {
        printf("\nNadador %d: %f", i+1,swimmers[i].velocity);
        //printf("\nNadador %d: %f", i+1,swimmers[i][0]);
    }
    std::cout << std::endl;
    
// ####################
// ###  MAIN LOOP   ###
// ####################

    int tiempo = 0;
    bool statement = true;
    float pos = 0;
    semaforo();
    bool applauseShown = false; // Variable para controlar si se ha mostrado el aplauso
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);


    while (statement == true){
        printf("Tiempo Actual: %d\n", tiempo);

        // CALCULATING NEXT POSITION
        for (int i = 0; i < NUM_SWIMMERS; i++)
        {
            // Crear un hilo para actualizar la posición del nadador
            err = pthread_create(&threads[i], &attr, updateSwimmerPosition, (void*) &swimmers[i]);
            if (err) {
                printf("Error al crear el hilo: %d\n", err);
                exit(-1);
            }
        }
        for (int i = 0; i < NUM_SWIMMERS; i++){
            // Here the thread will join
            pthread_join(threads[i], NULL);
        }

        // ## PRINTING SWIMMERS POSITIONS ##
        if (statement) {
            for(int i = 0; i < NUM_SWIMMERS; i++){
                for(int j = 0; j < PRINT_WIDTH; j++){
                    printf("#");
                }
                printf("\n");
                printSwimmerPositions(distance, swimmers[i].position);

                if (swimmers[i].position > distance)
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
                    printMedal("applause.txt");
                    statement=false;
                    break;
                }
            }
        }

        if (statement == false) {
            break;
        }
        sleep(2); //Two seconds so refresh rate of every 2 seconds
        tiempo = tiempo + 2;
        clearScreen();
    }

    pthread_mutex_destroy(&lock);	

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

// Función para calcular la posición del nadador
void* updateSwimmerPosition(void* arg) {
    Swimmer* swimmer = (Swimmer*)arg; // Cast del argumento a un puntero de Swimmer
    swimmer->position += swimmer->velocity; // Actualizar posición
    return NULL; // Retornar NULL ya que no se requiere pasar información de vuelta
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
