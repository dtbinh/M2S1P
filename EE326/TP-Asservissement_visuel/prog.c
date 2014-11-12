#include <stdio.h> 
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <mil.h>
#include "CommandeCamera.h"
#include "Castan.h"
#include "Definitions.h"
#include "Gauss.h"


#define DCF_FILE "M_CCIR"

#pragma comment (lib, "mil.lib") 


int main(void)
{
 unsigned char *ImageAcquise, *ptI ;
 long TailleImage, Nlin, Ncol, Nplan, lin, col, n ;
 int TailleMotif, nlin, ncol, off_lin, off_col  ;
 double *Motif;
 double *Derivee_t, *Derivee_x, *Derivee_y ;
 double *MatriceFlotOptique ;
 double *pt, *ptx, *pty, *fin, *ptM ;
 double alpha_castan = 0.4 ;
 char caractere_lu, pas_appuye_sur_tabulation ;
 double theta_x, theta_y ;

 // Declaration des differentes zones memoires
 // associees a la carte d'acquisition MATROX
 MIL_ID MilApplication,    /* Application identifier.  */
          MilSystem,       /* System identifier.       */
          MilDisplay,      /* Display identifier.      */
          MilDisplayOther, /* Sub-display identifier.      */
          MilDigitizer,    /* Digitizer identifier.    */ 
          MilImage,        /* Image buffer identifier. */
          MilImageOther;   /* Sub-image buffer identifier. */

 // Rajoutez ici des declarations de variables supplementaires

int i;

 // Ne pas modifier ce code //
 camera_on(); //Mise de la camera sous tension
 camera_home() ; // On demande a la camera de venir en position initiale
 zoom_init() ;

 // Temporisation le temps que la camera s'initialise
 n=50000000 ; while(n--) theta_x = exp((double)n) ;

 MappAlloc(M_COMPLETE, &MilApplication);
 MsysAlloc(M_SYSTEM_METEOR_II, M_DEV0, M_DEFAULT, &MilSystem);
 MdispAlloc(MilSystem, M_DEV0, "M_DEFAULT", M_DEFAULT, &MilDisplay);
 MdispAlloc(MilSystem, M_DEV0, "M_DEFAULT", M_DEFAULT, &MilDisplayOther);
 MdigAlloc(MilSystem, M_DEV0, DCF_FILE, M_DEFAULT, &MilDigitizer);

 Ncol = MdigInquire(MilDigitizer, M_SIZE_X, M_NULL); // Nombre de ligne de l'image complete
 Nlin = MdigInquire(MilDigitizer, M_SIZE_Y, M_NULL); // Nombre de colonnes de l'image complete
 Nplan = MdigInquire(MilDigitizer, M_SIZE_BAND, M_NULL); // Nombre de plans de l'image complete (en cas d'image couleur)

 TailleImage = Nlin * Ncol ;

 nlin = Nlin / 4 ; // Nombre de ligne du motif a poursuivre
 ncol = Ncol / 4 ; // Nombre de colonnes du motif a poursuivre
 off_lin = 3 * Nlin / 8 ; // position du motif a poursuivre
 off_col = 3 * Ncol / 8 ; // position du motif a poursuivre
 TailleMotif = nlin * ncol ;

 // Allocations des divers buffers necessaires a la carte d'acquisition
 MbufAllocColor(MilSystem, Nplan, Ncol, Nlin, 8L+M_UNSIGNED, M_IMAGE+M_GRAB+M_DISP, &MilImage);
 MbufChild2d(MilImage, off_col, off_lin, ncol, nlin, &MilImageOther) ;
 MbufClear(MilImage, M_BLACK);
 MdispSelect(MilDisplay,MilImage) ;
 MdispSelect(MilDisplayOther,MilImageOther) ;

 // Allocations des divers buffers necessaires pour l'application
 ImageAcquise = ALLOCATION(TailleImage,unsigned char) ;
 if(ImageAcquise == NULL)
 {
  printf("Impossible d'allouer %d bytes pour une image %dx%d\n",TailleImage,Nlin,Ncol) ;
 }

 Motif = ALLOCATION(TailleMotif,double) ;
 if(Motif == NULL)
 {
  DESALLOCATION(ImageAcquise) ;
  printf("Impossible d'allouer %d doubles pour une image %dx%d\n",TailleMotif,nlin,ncol) ;
 }

 Derivee_x = ALLOCATION(TailleMotif,double) ;
 if(Derivee_x == NULL)
 {
  DESALLOCATION(Motif) ;
  DESALLOCATION(ImageAcquise) ;
  printf("Impossible d'allouer %d doubles pour une image %dx%d\n",TailleMotif,nlin,ncol) ;
 }

 Derivee_y = ALLOCATION(TailleMotif,double) ;
 if(Derivee_y == NULL)
 {
  DESALLOCATION(Derivee_x) ;
  DESALLOCATION(Motif) ;
  DESALLOCATION(ImageAcquise) ;
  printf("Impossible d'allouer %d doubles pour une image %dx%d\n",TailleMotif,nlin,ncol) ;
 }

 Derivee_t = ALLOCATION(TailleMotif,double) ;
 if(Derivee_t == NULL)
 {
  DESALLOCATION(Derivee_y) ;
  DESALLOCATION(Derivee_x) ;
  DESALLOCATION(Motif) ;
  DESALLOCATION(ImageAcquise) ;
  printf("Impossible d'allouer %d doubles pour une image %dx%d\n",TailleMotif,nlin,ncol) ;
 }

 MatriceFlotOptique = ALLOCATION(2*TailleMotif,double) ;
 if(MatriceFlotOptique == NULL)
 {
  DESALLOCATION(Derivee_t) ;
  DESALLOCATION(Derivee_y) ;
  DESALLOCATION(Derivee_x) ;
  DESALLOCATION(Motif) ;
  DESALLOCATION(ImageAcquise) ;
  printf("Impossible d'allouer %d doubles pour une image 2x%dx%d\n",2*TailleMotif,nlin,ncol) ;
 }
 
 // Boucle permettant de visualiser l'image courante
 // On sort de la boucle en appuyant sur la touche tabulation
 pas_appuye_sur_tabulation = 1 ;
 while(pas_appuye_sur_tabulation)
 {
  if(kbhit())
  {
   printf("appuye sur touche \n") ;
   caractere_lu = getch() ;
   pas_appuye_sur_tabulation = ( caractere_lu != ' ' ) ;
  }
  // Acquisition de l'image courante
 	MdigGrab(MilDigitizer, MilImage);
  // L'image courant est copiee dans le buffer ImageAcquise
 	MbufGet(MilImage, ImageAcquise);  
  // On copie dans le buffer Motif la zone cible de l'image acquise
  pt = Motif ; fin = Motif + TailleMotif ;
  for( lin=0 ; lin<nlin ; lin++)
  {
   ptI = ImageAcquise + ( ( lin + off_lin ) * Ncol ) + off_col ;
   for( col=0 ; col<ncol ; col++, ptI++, pt++)
   { 
    (*pt) = (double)(*ptI) ;
   }
  }
 } // sortie de la boucle d'acquisition

 // Calcul de la derivee du motif
 castan(Motif, Derivee_x, Derivee_y, nlin, ncol, alpha_castan ) ; //typiquement alpha = 0.4

 // Ici vous devez realiser le calcul de la matrice d'interaction

for (i=0;i<2*TailleMotif;i++)
{if(i/2==i)
  {MatriceFlotOptique[i]=Derivee_x[i/2];}
 else MatriceFlotOptique[i]=Derivee_y[i/2];
}
MatriceFlotOptique=PseudoInverse(MatriceFlotOptique, TailleMotif, 2);

 // Cette boucle vous permettra de tester votre calcul
 pas_appuye_sur_tabulation = 1 ;
 while(pas_appuye_sur_tabulation)
 {
  if(kbhit())
  {
   printf("appuye sur touche \n") ;
   caractere_lu = getch() ;
   pas_appuye_sur_tabulation = ( caractere_lu != ' ' ) ;
  }

 	MdigGrab(MilDigitizer, MilImage);
 	MbufGet(MilImage, ImageAcquise);  
  ptM = Motif ; fin = Motif + TailleMotif ;
  pt = Derivee_t ;
  for( lin=0 ; lin<nlin ; lin++)
  {
   ptI = ImageAcquise + ( ( lin + off_lin ) * Ncol ) + off_col ;
   for( col=0 ; col<ncol ; col++, ptI++, pt++, ptM++)
   { 
    (*pt) = (*ptM) - (double)(*ptI) ;
    (*ptI) = (unsigned char)floor((*ptM) - (double)(*ptI)) ;
   }
  }

  // Pour afficher la difference d'images et bien voir la superposition du motif et de l'image courante
  MbufPut(MilImage,ImageAcquise) ;
  

  *ptx=0;
  *pty=0;
  for(i=0;i<TailleMotif;i++)
  {
  *ptx=*ptx-MatriceFlotOptique[i]*Derivee_t[i];
  *pty=*pty-MatriceFlotOptique[i+TailleMotif]*Derivee_t[i];
  }


  theta_x = *pty*0.1 ;
  // Calcul de theta_x

  theta_y = *ptx*0.1 ;
  // Calcul de theta_y

  // retirez les commentaires si vous voulez voir les valeurs calculees
  printf(" %g     %g       \r",theta_x,theta_y) ;
  Pan_Tilt(theta_y, theta_x) ; 
  //deplacement pan = rotation autour de l'axe vertical
  //deplacement tilt = rotation autour de l'axe horiozontal


 } // fin du while


  
 camera_off(); //camera hors tension

 // Desallocation de tous les buffers

 MbufFree(MilImageOther);
 MbufFree(MilImage);
 MdigFree(MilDigitizer);
 MdispFree(MilDisplayOther);
 MdispFree(MilDisplay);
 MsysFree(MilSystem);
 MappFree(MilApplication);

 DESALLOCATION(MatriceFlotOptique) ;
 DESALLOCATION(Derivee_t) ;
 DESALLOCATION(Derivee_y) ;
 DESALLOCATION(Derivee_x) ;
 DESALLOCATION(Motif) ;
 DESALLOCATION(ImageAcquise) ;

}
