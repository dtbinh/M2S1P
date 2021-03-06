#ifndef _DEFINITIONS_H_
#define _DEFINITIONS_H_

#include <stdlib.h>
#define ALLOCATION_KO 0
#define ALLOCATION_OK 1
#define TERMINE 2

// #define ALLOCATION(N,type) (type *)mxCalloc(N,sizeof(type))  // pour mexFunction
#define ALLOCATION(N,type) (type *)malloc(N*sizeof(type))  // pour C windows
// #define ALLOCATION(N,type) new type [N]   // pour C++ windows
// #define DESALLOCATION(pointeur) if(pointeur!=NULL) mxFree(pointeur)  // pour mexFunction
#define DESALLOCATION(pointeur) free(pointeur)   // pour C windows
// #define DESALLOCATION(pointeur) delete pointeur   // pour C++ windows

#define EPSILON 1e-32


#define Fabs(a) ( ( (a) > 0.0 ) ? (a) : (-(a)) )
#define Min(a,b) ( ( (a) < (b) ) ? (a) : (b) )
#define Max(a,b) ( ( (a) > (b) ) ? (a) : (b) )

#endif

