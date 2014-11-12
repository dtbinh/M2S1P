#include "stdio.h"
#include "mex.h"
#include "math.h"
#include "time.h"
#include "Definitions.h" 
#include "Castan.h" 
#include "Deriche.h" 

#define MESSAGE_D_ERREUR "L'appel de la fonction se fait sous la forme : \n[ GradientX, GradientY, GradientXY ] = DeriveImage(ImageSource, alpha, type) \n type = 1 : shen-castan \n type = 2 : deriche"

void mexFunction(int NbOut, struct mxArray_tag *PtOut[],int NbIn, const struct mxArray_tag *PtIn[])
{
 double *Image, *Derive_x, *Derive_y, *Derive_xy, *pt, *ptd ;
 int TailleImage, Nlin, Ncol, lin, col ;
 double alpha ;
 unsigned char type_de_filtre ;
 int n ;

 switch(NbOut)
 {
  case 3 : break ;
  default : mexErrMsgTxt(MESSAGE_D_ERREUR) ; return ; break ;
 }
 
 switch(NbIn)
 {
  case 3 : break ;
  default : mexErrMsgTxt(MESSAGE_D_ERREUR) ; return ; break ;
 }
   
 Nlin = mxGetM(PtIn[0]) ;
 Ncol = mxGetN(PtIn[0]) ;
 
 if( ( mxGetM(PtIn[1]) != 1 ) || ( mxGetN(PtIn[1]) != 1 ) )
 {
  printf("un seul parametre pour les filtres\n") ;
  mexErrMsgTxt(MESSAGE_D_ERREUR) ; return ;
 }

 if( ( mxGetM(PtIn[2]) != 1 ) || ( mxGetN(PtIn[2]) != 1 ) )
 {
  printf("type = 1 (shen) ou 2 (deriche) " ) ;
  mexErrMsgTxt(MESSAGE_D_ERREUR) ; return ;
 }
 
 
 pt = mxGetPr(PtIn[1]) ; alpha = (*pt) ;
 pt = mxGetPr(PtIn[2]) ; type_de_filtre = (unsigned char)(*pt) ;
  
 TailleImage = Nlin * Ncol ;

 // Allocation de l'image
 
 Image = (double *)mxCalloc(TailleImage,sizeof(double)) ;
 if(Image==NULL)
 {
  printf("Probleme d'allocation pour %d bytes pour l'image source\n",TailleImage) ;
  mexErrMsgTxt("Pas assez de mémoire pour traiter de ce problème\n") ;
  return ;
 }
 
 Derive_x = (double *)mxCalloc(TailleImage,sizeof(double)) ;
 if(Derive_x==NULL)
 {
  mxFree(Image) ;
  printf("Probleme d'allocation pour %d bytes pour l'image cible\n",TailleImage) ;
  mexErrMsgTxt("Pas assez de mémoire pour traiter de ce problème\n") ;
  return ;
 }

 Derive_y = (double *)mxCalloc(TailleImage,sizeof(double)) ;
 if(Derive_y==NULL)
 {
  mxFree(Image) ;
  mxFree(Derive_x) ;
  printf("Probleme d'allocation pour %d bytes pour l'image cible\n",TailleImage) ;
  mexErrMsgTxt("Pas assez de mémoire pour traiter de ce problème\n") ;
  return ;
 }

 Derive_xy = (double *)mxCalloc(TailleImage,sizeof(double)) ;
 if(Derive_xy==NULL)
 {
  mxFree(Image) ;
  mxFree(Derive_x) ;
  mxFree(Derive_y) ;
  printf("Probleme d'allocation pour %d bytes pour l'image cible\n",TailleImage) ;
  mexErrMsgTxt("Pas assez de mémoire pour traiter de ce problème\n") ;
  return ;
 }

 // On remet l'image dans l'ordre 
 pt = mxGetPr(PtIn[0]) ;
 for( col=0 ; col<Ncol ; col++ )
 {
  for( lin=0 ; lin<Nlin ; lin++ )
  {
   n = (lin*Ncol) + col ;
   Image[ n ] = (*pt) ;
   pt++ ;
  }
 }
 
 switch(type_de_filtre)
 {
  case 1 :
  {
   castan(Image, Derive_xy, Derive_x, Derive_y, Nlin, Ncol, alpha) ;
  } break ;
  
  case 2 :
  {
   deriche(Image, Derive_xy, Derive_x, Derive_y, Nlin, Ncol, alpha) ;
  } break ;
  
  default :
  {
   pt = Image ; ptd = Derive_x ; for( n=0 ; n<TailleImage ; n++ ) (*ptd++) = (*pt++) ;
   pt = Image ; ptd = Derive_y ; for( n=0 ; n<TailleImage ; n++ ) (*ptd++) = (*pt++) ;
   pt = Image ; ptd = Derive_xy ; for( n=0 ; n<TailleImage ; n++ ) (*ptd++) = (*pt++) ;
  } break ;
 }
 
 
 PtOut[0] = mxCreateDoubleMatrix(Nlin,Ncol,mxREAL) ;
 PtOut[1] = mxCreateDoubleMatrix(Nlin,Ncol,mxREAL) ;
 PtOut[2] = mxCreateDoubleMatrix(Nlin,Ncol,mxREAL) ;
 
 pt = mxGetPr(PtOut[0]) ;

 for( col=0 ; col<Ncol ; col++ )
 {
  for( lin=0 ; lin<Nlin ; lin++ )
  {
   n = (lin*Ncol) + col ;
   (*pt) = Derive_x[n] ;
   pt++ ;
  }
 }

 pt = mxGetPr(PtOut[1]) ;

 for( col=0 ; col<Ncol ; col++ )
 {
  for( lin=0 ; lin<Nlin ; lin++ )
  {
   n = (lin*Ncol) + col ;
   (*pt) = Derive_y[n] ;
   pt++ ;
  }
 }
 pt = mxGetPr(PtOut[2]) ;

 for( col=0 ; col<Ncol ; col++ )
 {
  for( lin=0 ; lin<Nlin ; lin++ )
  {
   n = (lin*Ncol) + col ;
   (*pt) = Derive_xy[n] ;
   pt++ ;
  }
 }
  
 mxFree(Image) ;
 mxFree(Derive_x) ;
 mxFree(Derive_y) ;
 mxFree(Derive_xy) ;
}
