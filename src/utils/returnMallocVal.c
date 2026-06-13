#include "returnMallocVal.h"
#include <stdlib.h>
#include<stdio.h>

void* returnMallocInt(int val)
{
    int* intVal = malloc(sizeof(int));
    if(intVal == NULL)
        return NULL;
    
    *intVal = val;

    return intVal;
}