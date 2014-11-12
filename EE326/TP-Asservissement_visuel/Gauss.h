#ifndef _GAUSS_H_
#define _GAUSS_H_

double Gaussym(double *, int) ;
double Pivot_max(double *M,int *lm,int *cm,int k,int N);
void ech_co(double *M,int j,int k,int N,int sens);
void ech_li(double *M,int i,int k,int N,int sens);
int PseudoInverse(double *A, int Nlin, int Ncol) ;



#endif