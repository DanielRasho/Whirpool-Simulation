#include <random>

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
