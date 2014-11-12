/***************************************************************************/
/* Procedure d'inversion de matrice par la methode d'elimination de Gauss  */
/* avec pivot total. le 16/08/90. O Strauss.                               */
/***************************************************************************/

#include "stdio.h"
#include "math.h"
#include "stdlib.h"
#include "Definitions.h"
#include "Gauss.h"

#define EPS 1e-200

double Pivot_max(double *M,int *lm,int *cm,int k,int N)
{
 int i,j ;
 double pivot,Mij ;
 lm[k]=k ; cm[k]=k ;
 pivot = (* (M+(k*N)+k) ) ;
 i=k ; j=k ;
 for(j=k ; j<N ; j++)
 {
  for(i=k ; i<N ; i++)
  {
   Mij = (* (M+(i*N)+j) ) ;
   if( Fabs(Mij) > Fabs(pivot) )
   {
    pivot = Mij ; lm[k] = i ; cm[k] = j ;
   }
  }
 }
 return(pivot) ;
}

void ech_co(double *M,int j,int k,int N,int sens)
{
 int i ;
 double sol ;
 for(i=0 ; i<N ; i++)
 {
  sol = ( *(M+(i*N)+j) ) * sens ;
  ( *(M+(i*N)+j) ) = (-1.0) * ( *(M+(i*N)+k) ) * sens ;
  ( *(M+(i*N)+k) ) = sol ;
 }
}

void ech_li(double *M,int i,int k,int N,int sens)
{
 int j ;
 double sol ;
 for(j=0 ; j<N ; j++)
 {
  sol = ( *(M+(i*N)+j) ) * sens ;
  ( *(M+(i*N)+j) ) = (-1.0) * ( *(M+(k*N)+j) ) * sens ;
  ( *(M+(k*N)+j) ) = sol ;
 }
}


double Gaussym(double *M, int N)
{
 int i,ik,il,jc,k,kk,l,nk ;
 int *lm,*cm ; /* ce sont les vecteurs temporaires des indices
                  d'echange de ligne et de colonne */
 double pivot , det=1.0 ;

 /* Attribution de m‚moire */
 lm = ALLOCATION( N , int ) ;
 cm = ALLOCATION( N , int ) ;
 nk=0 ;

 /* Boucle principale */
 for(k=0 ; k<N ; k++)
 {
  kk=nk+k ; /* l'‚l‚ment de la diagonnale d'indice k */
  nk+=N ;
  /* Recherche du pivot maximal */
  pivot = Pivot_max(M,lm,cm,k,N) ;
  if(Fabs(pivot)<1e-160)
  { printf("\n Matrice singuliere !!") ; return(0.0) ; }
  det *= pivot ;


  /* echange de lignes */
  il=lm[k] ;
  if(k!=il) ech_li(M,k,il,N,-1) ;

  /* echange de colonnes */
  jc=cm[k] ;
  if(k!=jc) ech_co(M,k,jc,N,-1) ;

  (*(M+kk)) = 1.0 ;
  /* elimination */
  for(l=0 ; l<N ; l++) ( *(M+(k*N)+l) ) /= pivot ;
  for(i=0 ; i<N ; i++)
  {
   if(i!=k)
   {
    ik = (i*N) + k  ;
    pivot = (*(M+ik)) ;
    ( *(M+ik) ) = 0.0 ;
    for(l=0 ; l<N ; l++) ( *(M+(i*N)+l) ) -= pivot * ( *(M+(k*N)+l) ) ;
   }
  }
 }
 /* Rearrangement de la matrice */

 for(k=N-1 ; k>=0 ; k--)
 {
  /* echange de colonnes */
  il=lm[k] ;
  if(k!=il) ech_co(M,k,il,N,1) ;

  /* echange de lignes */
  jc=cm[k] ;
  if(k!=jc) ech_li(M,k,jc,N,1) ;
 }
 DESALLOCATION(lm) ; DESALLOCATION(cm) ;
 return(det) ;
}


int PseudoInverse(double *A, int Nlin, int Ncol)
{
 double *ATA, *AT ;
 int lin, col, k ;
 double *pt, *pt1, *pt2 ;
 double determinant ;
 ATA = ALLOCATION( Ncol*Ncol, double) ;
 if(ATA==NULL) return 0 ;
 AT = ALLOCATION( Nlin*Ncol, double) ;
 if(AT==NULL)
 {
  DESALLOCATION(ATA) ;
  return 0 ;
 }
 
 pt = ATA ;
 for( lin=0 ; lin<Ncol ; lin++ )
 {
  for( col=0 ; col<Ncol ; col++, pt++ )
  {
   pt1 = A + lin ;
   pt2 = A + col ;
   (*pt) = 0.0 ;
   for( k=0 ; k<Nlin ; k++ )
   {
    (*pt) += (*pt1) * (*pt2) ;
    pt1 += Ncol ;
    pt2 += Ncol ;
   }
  }
 }
 
 pt = ATA ;
 for( lin=0 ; lin<Ncol ; lin++ )
 {
  for( col=0 ; col<Ncol ; col++, pt++ )
  {
   printf("[%g]",(*pt)) ;
  }
  printf("\n") ;
 }


 pt1 = A ;
 for( lin=0 ; lin<Nlin ; lin++ )
 {
  pt2 = AT + lin ;
  for( col=0 ; col<Ncol ; col++ )
  {
   (*pt2) = (*pt1) ;
   pt1 ++ ;
   pt2 += Nlin ;
  }
 }
 
 determinant = Gaussym(ATA, Ncol) ;

 pt = A ;
 for( lin=0 ; lin<Ncol ; lin++ )
 {
  for( col=0 ; col<Nlin ; col++, pt++ )
  {
   pt1 = ATA + lin*Ncol ;
   pt2 = AT + col ;
   (*pt) = 0.0 ;
   for( k=0 ; k<Ncol ; k++ )
   {
    (*pt) += (*pt1) * (*pt2) ;
    pt1 ++ ;
    pt2 += Nlin ;
   }
  }
 }

 printf(" ok 5 \n");
 
 DESALLOCATION(ATA) ;
 DESALLOCATION(AT) ;
 return 1 ;
}
