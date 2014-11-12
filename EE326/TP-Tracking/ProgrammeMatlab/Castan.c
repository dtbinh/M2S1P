#include "math.h"
#include "Definitions.h"

 int castan(double * Image,
            double * Imagexy,
	    double * Imagex,
	    double * Imagey, 
	    int nlin,
	    int ncol,
	    double alpha) //typiquement 0.4

 {

  double ao,oa,ko,klo;
  double *B1,*B2, *dx, *dy, *dxy ;
  double *debut, *fin ;
  int dim,ncol1,nlin1,nbre_d_element;
  int col,lin;
  double *pixel_suivant ;
  double *B_courant, *B_suivant,*B_precedent ;
 

  ncol1 = ncol - 1;
  nlin1 = nlin - 1;
  nbre_d_element = ncol * nlin;
  dim = (nlin>ncol) ? nlin : ncol ;
  ao = ( (alpha>0.0) && (alpha<1.0) ) ? alpha : 0.135 ;
  oa = 1.0 - ao ;
  ko = oa/(2.0*ao);
  klo=1.0/(2.0-ao);
  /* allocation memoire*/

  B1  = ALLOCATION(dim,double); //(double *) kmalloc(dim  * sizeof(double) ) ;
  B2  = ALLOCATION(dim,double); //(double *) kmalloc(dim * sizeof(double) ) ;

  if((B1==NULL)||(B2==NULL))
  {
   printf( "Impossible d'allouer B1 ou B2 dans la procedure Deriche!!");
   return(0) ;
  }


/**********************************/
/* Calcul de la derivee suivant y */
/**********************************/

  /* lissage en x */
 for( debut=Image, lin=0 ; lin<nlin ; lin++, debut+=ncol )
  {
    /* Lissage Causal : B1 */

   B_courant = B1 ;
   B_suivant = B1 ;
   pixel_suivant = debut ;

   (*B_suivant) =  (*pixel_suivant) ;
   B_suivant++  ; pixel_suivant++;
  
   fin = B1 + ncol ;

   while(B_suivant<fin)
   {	
    (*B_suivant) = ao * (*B_courant) +
		   oa * (*pixel_suivant) ;
    B_suivant++ ; B_courant++ ;
    pixel_suivant++ ;
   }
    
    /* Lissage Anti-Causal : B2 */
   
   B_courant = B2 + ncol1 ;
   B_suivant = B2 + ncol1 ;
   pixel_suivant = debut + ncol1 ;

   (*B_suivant) =  (*pixel_suivant) ;
   B_suivant-- ; pixel_suivant--;
  
   fin = B2 ;

   while(B_suivant>=fin)
   {	
    (*B_suivant) = ao * (*B_courant) +
                   oa * (*pixel_suivant);		 

    B_suivant-- ; B_courant-- ;
    pixel_suivant-- ; 
   }

   
   B_precedent = B1 ; B_suivant = B2 ;
   dy = Imagey + ( lin * ncol ) ; fin = dy + ncol ;
   pixel_suivant = Image + (lin * ncol);
   while(dy<fin)
   {
    (*dy) = klo*((*B_precedent) + (*B_suivant)-ao * (*pixel_suivant))  ;
    dy++ ; B_precedent++ ; B_suivant++ ;pixel_suivant++;
   }


  } /* fin du lissage */


/****************************************************************************/

  /* derivateur en y */


  for( debut=Imagey, col=0 ; col<ncol ; col++, debut++ )
  { 

    // derivateur Causal : B1 
   
   B_courant = B1 ;
   B_suivant = B1 ;
   pixel_suivant = debut ;

   (*B_suivant) =  (*pixel_suivant) ;
 
   B_suivant++  ; pixel_suivant+=ncol ; 
   fin = B1 + nlin ;

   while(B_suivant<fin)
   {	
    (*B_suivant) = ao * (*B_courant) +
                   oa * (*pixel_suivant);
    B_suivant++ ; B_courant++ ;
    pixel_suivant+=ncol ;
   }

    // Lissage Anti-Causal : B2 
   
   B_courant = B2 + nlin1 ;
   B_suivant = B2 + nlin1 ;
   pixel_suivant = debut + nlin1*ncol ;

   (*B_suivant) =  (*pixel_suivant) ;
 
   B_suivant-- ;
   pixel_suivant-=ncol ; 
   fin = B2 ;

   
   while(B_suivant>=fin)
   {	
    (*B_suivant) = ao * (*B_courant) +
                   oa * (*pixel_suivant);
    B_courant-- ;
    B_suivant--;
    pixel_suivant-=ncol ;
   }

  
   B_precedent = B1 ; B_suivant = B2 ;
   dy = Imagey + col ; fin = dy + nlin*ncol;
   while(dy<fin)
   {
    (*dy) = ko*(-(*B_precedent) + (*B_suivant))  ;
    dy+=ncol ; B_precedent++ ; B_suivant++ ;
   }
   
  } // fin du derivateur 



/**********************************/
/* Calcul de la derivee suivant x */
/**********************************/

 
  /* lissage en y */


  for( debut=Image, col=0 ; col<ncol ; col++, debut++ )
  {
    // lissage Causal : B1 

   B_courant = B1 ;
   B_suivant = B1 ;
   pixel_suivant = debut ;

   (*B_suivant) =  (*pixel_suivant) ;
   B_suivant++  ; pixel_suivant+=ncol;
   
   fin = B1 + nlin ;

   while(B_suivant<fin)
   {	
    (*B_suivant) = ao * (*B_courant) + 
                   oa * (*pixel_suivant);
    B_suivant++ ; B_courant++ ; 
    pixel_suivant+=ncol ;
   }



    // Lissage Anti-Causal : B2 
   
   B_courant = B2 + nlin1 ;
   B_suivant = B2 + nlin1 ;
   pixel_suivant = debut + nlin1 *ncol;

   (*B_suivant) =  (*pixel_suivant) ;
   B_suivant-- ; pixel_suivant-=ncol;
  
   fin = B2 ;

   while(B_suivant>=fin)
   {	
    (*B_suivant) = ao * (*B_courant) + 
                   oa * (*pixel_suivant);

    B_suivant-- ; B_courant-- ; 
    pixel_suivant-=ncol ;
   }

  
   B_precedent = B1 ; B_suivant = B2 ;
   dx = Imagex + col ; fin = dx + nlin *ncol;
   pixel_suivant = Image + col;
   while(dx<fin)
   {
    (*dx) =klo*( (*B_precedent) + (*B_suivant)-ao * (*pixel_suivant))  ;
    dx+=ncol ; B_precedent++ ; B_suivant++ ;pixel_suivant+=ncol;
   }

  } // fin du lissage 




  // derivee en x 


  for( debut=Imagex, lin=0 ; lin<nlin ; lin++, debut+=ncol )
  {

    // derivee Causal : B1 
   

   B_courant = B1 ;
   B_suivant = B1 ;
   pixel_suivant = debut ;

   (*B_suivant) =  (*pixel_suivant) ;
 
   B_suivant++ ; pixel_suivant++; 
   fin = B1 + ncol ;

   while(B_suivant<fin)
   {	
    (*B_suivant) = ao * (*B_courant) + 
                   oa * (*pixel_suivant);
    B_suivant++ ; B_courant++ ;
    pixel_suivant++ ;
   }

  
   // derivee Anti-Causal : B2 
   B_courant = B2 + ncol1 ;
   B_suivant = B2 + ncol1 ;
   pixel_suivant = debut + ncol1 ;

   (*B_suivant) =  (*pixel_suivant) ;
  
   B_suivant-- ;
   pixel_suivant-- ;
   
   fin = B2 ;

   while(B_suivant>=fin)
   {	
    (*B_suivant) = ao * (*B_courant) +
                   oa * (*pixel_suivant) ;
    B_courant-- ;
    B_suivant--;
    pixel_suivant-- ;
   }

  
 
   B_precedent = B1 ; B_suivant = B2 ;
   dx = Imagex + ( ncol * lin ) ; fin = dx + ncol;

   while(dx<fin)
   {
    (*dx) =ko*(- (*B_precedent) + (*B_suivant))  ;
    dx++ ; B_precedent++ ; B_suivant++ ;
   }


  } // fin du derivateur

 

 /***********************************************/
 /*      Calcul du gradient                     */
 /***********************************************/

 
 dx = Imagex;
 dy = Imagey;
 for(lin=0, dxy = Imagexy; lin< nbre_d_element;lin++, dxy++)
 {
  (*dxy) = sqrt((*dx)*(*dx)+(*dy)*(*dy));
  dx++;
  dy++;

 }

 DESALLOCATION(B1);
 DESALLOCATION(B2);
 return 0;
 
}

