#include "rands.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

double
randNormalize(void)
{
  return (rand()/((double)RAND_MAX+1));
}


int
randInt(int izq, int der)
{
  return (rand()%(der-izq+1)+izq);
}

void
randomize(void)
{
  (srand((int)time(NULL)));
}

double
randReal(double izq,double der)
{
  return ((randNormalize()*(der-izq)+izq));
}
