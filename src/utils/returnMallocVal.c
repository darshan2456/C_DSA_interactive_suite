#include "returnMallocVal.h"
#include <stdio.h>
#include <stdlib.h>

void* returnMallocInt(int val)
{
    int* intVal = malloc(sizeof(int));
    if (intVal == NULL)
        return NULL;

    *intVal = val;

    return intVal;
}