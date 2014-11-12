 #include "math.h"
 #include "Definitions.h"

 int deriche(double * Image,
	     double * Imagexy,
	     double * Imagex,
	     double * Imagey, 
	     int nlin,
	     int ncol,
	     double alpha) //typiquement 0.7

 {

  double a0,a1,a2,b1,b2,b3,b4,b5,b6,b,c,bb;
  double expa;
  double *B1,*B2, *dx, *dy, *dxy ;
  double *debut, *fin ;
  int dim,ncol1,nlin1,nbre_d_element;
  int col,lin;
  double *pixel_courant, *pixel_suivant ;
  double *B_courant, *B_precedent, *B_suivant ;


  ncol1 = ncol - 1;
  nlin1 = nlin - 1;
  nbre_d_element = ncol * nlin;
  dim = (nlin>ncol) ? nlin : ncol ;

  expa = exp(-alpha);

  b = 1.0 - expa;
  b = b * b;
  c = b / expa;
  b2  = exp(-2.0*alpha);
  b = b / (1.0 + (2.0 * alpha * expa) - b2);

  a0 = c *expa;
  a1 = b *  b2;
  a2 = b*(alpha +1.0)*expa;
  b3 = b*(alpha -1.0)*expa;
  b1  = 2.0 * expa;
  bb = (expa-1.0);
  bb= bb*bb*bb;
  bb=bb/(2.0*(b2+expa));
  b4 = bb*expa;
  b5 = b4 / ( 1.0 - b1 + b2 ) ;
  b6 = b1 - b2  +b +b3;
  
  /* allocation memoire*/

  B1  = ALLOCATION(dim,double); //(double *) kmalloc(dim  * sizeof(double) ) ;
  B2  = ALLOCATION(dim,double); //(double *) kmalloc(dim * sizeof(double) ) ;

  if((B1==NULL)||(B2==NULL))
  {
   printf("Impossible d'allouer B1 ou B2 dans la procedure Deriche!!") ;   return(0) ;
  }
  


/**********************************/
/* Calcul de la derivee suivant y */
/**********************************/

  // lissage en x 

  for( debut=Image, lin=0 ; lin<nlin ; lin++, debut+=ncol )
  {
    // Lissage Causal : B1 

   B_courant = B1 ;
   B_precedent = B1 ;
   B_suivant = B1 ;
   pixel_courant = debut ;
   pixel_suivant = debut ;

   (*B_suivant) =  b6*(*pixel_suivant) ;
   B_suivant++  ; pixel_suivant++;
   (*B_suivant) =  b1 * (*B_courant) - 
                   b2 * (*pixel_courant) +
		   b  * (*pixel_suivant) +
		   b3 * (*pixel_courant) ;
   B_suivant++ ; pixel_suivant++ ; 
   B_courant++ ; pixel_courant++;
   fin = B1 + ncol ;

   while(B_suivant<fin)
   {	
    (*B_suivant) = b1 * (*B_courant) - 
                   b2 * (*B_precedent)+
		   b * (*pixel_suivant) +b3 * (*pixel_courant) ;
    B_suivant++ ; B_courant++ ; B_precedent++ ;
    pixel_suivant++ ; pixel_courant++ ;
   }
    
    // Lissage Anti-Causal : B2 
   
   B_courant = B2 + ncol1 ;
   B_precedent = B2 + ncol1 ;
   B_suivant = B2 + ncol1 ;
   pixel_courant = debut + ncol1 ;
   pixel_suivant = debut + ncol1 ;

   (*B_suivant) =  b6*(*pixel_courant) ;
   B_suivant-- ; pixel_suivant--;
   (*B_suivant) =  b1 * (*B_courant) - 
                   b2 * (*pixel_courant) +
		   b * (*pixel_suivant) + 
		   b3*(*pixel_courant) ;
   B_suivant-- ; pixel_suivant--;
   B_courant-- ; pixel_courant-- ; 
   fin = B2 ;

   while(B_suivant>=fin)
   {	
    (*B_suivant) = b1 * (*B_courant) - 
                   b2 * (*B_precedent) +
		   b * (*pixel_suivant) + 
		   b3*(*pixel_courant) ;

    B_suivant-- ; B_courant-- ; B_precedent-- ;
    pixel_suivant-- ; pixel_courant-- ;
   }

   
   B_precedent = B1 ; B_suivant = B2 ;
   dy = Imagey + ( lin * ncol ) ; fin = dy + ncol ;
   pixel_suivant = Image + (lin * ncol);
   while(dy<fin)
   {
    (*dy) = (*B_precedent) + (*B_suivant)-b * (*pixel_suivant)  ;
    dy++ ; B_precedent++ ; B_suivant++ ;pixel_suivant++;
   }


  } // fin du lissage 


/****************************************************************************/

  /* derivateur en y */


  for( debut=Imagey, col=0 ; col<ncol ; col++, debut++ )
  {
    //derivateur Causal : B1 

   B_courant = B1 ;
   B_precedent = B1 ;
   B_suivant = B1 ;
   pixel_courant = debut ;

   (*B_suivant) =  b5 * (*pixel_courant) ;
   B_suivant++ ;
   (*B_suivant) =  b1 * (*B_courant) - 
                   b2 * (*B_courant) +
				   b4 * (*pixel_courant);
 
   B_suivant++ ; B_courant++ ; pixel_courant+=ncol ; 
   fin = B1 + nlin ;

   while(B_suivant<fin)
   {	
    (*B_suivant) = b1 * (*B_courant) - 
                   b2 * (*B_precedent)+
				   b4 * (*pixel_courant) ;
    B_suivant++ ; B_courant++ ; B_precedent++ ;
    pixel_courant+=ncol ;
   }

    // Lissage Anti-Causal : B2 
   
   B_courant = B2 + nlin1 ;
   B_precedent = B2 + nlin1 ;
   B_suivant = B2 + nlin1 ;
   pixel_courant = debut + nlin1*ncol ;

   (*B_suivant) =  b5*(*pixel_courant) ;
   B_suivant-- ;
   (*B_suivant) =  b1 * (*B_courant) - 
                   b2 * (*B_courant) +
				   b4 * (*pixel_courant);
   B_suivant-- ;
   B_courant-- ; pixel_courant-=ncol ; 
   fin = B2 ;

   while(B_suivant>=fin)
   {	
    (*B_suivant) = b1 * (*B_courant) -
                   b2 * (*B_precedent) +
				   b4 * (*pixel_courant) ;
    B_courant-- ; B_precedent-- ;
    B_suivant--;
    pixel_courant-=ncol ;
   }

   B_precedent = B1 ; B_suivant = B2 ;
   dy = Imagey + col ; fin = dy + nlin*ncol;
   while(dy<fin)
   {
    (*dy) = (*B_precedent) - (*B_suivant)  ;
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
   B_precedent = B1 ;
   B_suivant = B1 ;
   pixel_courant = debut ;
   pixel_suivant = debut ;

   (*B_suivant) =  b6 *(*pixel_suivant) ;
   B_suivant++  ; pixel_suivant+=ncol;
   (*B_suivant) =  b1 * (*B_courant) - 
                   b2 * (*pixel_courant) +
		   b  * (*pixel_suivant) +
		   b3 * (*pixel_courant) ;
   B_suivant++ ; pixel_suivant+=ncol ; 
   B_courant++ ; pixel_courant+=ncol;
   fin = B1 + nlin ;

   while(B_suivant<fin)
   {	
    (*B_suivant) = b1 * (*B_courant) - 
                   b2 * (*B_precedent)+
		   b * (*pixel_suivant) +b3 * (*pixel_courant) ;

    B_suivant++ ; B_courant++ ; B_precedent++ ;
    pixel_suivant+=ncol ; pixel_courant+=ncol;
   }



    // Lissage Anti-Causal : B2 
   
   B_courant = B2 + nlin1 ;
   B_precedent = B2 + nlin1 ;
   B_suivant = B2 + nlin1 ;
   pixel_courant = debut + nlin1*ncol ;
   pixel_suivant = debut + nlin1 *ncol;

   (*B_suivant) =  b6 *(*pixel_courant) ;
   B_suivant-- ; pixel_suivant-=ncol;
   (*B_suivant) =  b1 * (*B_courant) - 
                   b2 * (*pixel_courant) +
		   b * (*pixel_suivant) + 
		   b3*(*pixel_courant) ;
   B_suivant-- ; pixel_suivant-=ncol;
   B_courant-- ; pixel_courant-=ncol ; 
   fin = B2 ;

   while(B_suivant>=fin)
   {	
    (*B_suivant) = b1 * (*B_courant) - 
                   b2 * (*B_precedent) +
		   b * (*pixel_suivant) + 
		   b3*(*pixel_courant) ;

    B_suivant-- ; B_courant-- ; B_precedent-- ;
    pixel_suivant-=ncol ; pixel_courant-=ncol ;
   }

  
   B_precedent = B1 ; B_suivant = B2 ;
   dx = Imagex + col ; fin = dx + nlin *ncol;
   pixel_suivant = Image + col;
   while(dx<fin)
   {
    (*dx) = (*B_precedent) + (*B_suivant)-b * (*pixel_suivant)  ;
    dx+=ncol ; B_precedent++ ; B_suivant++ ;pixel_suivant+=ncol;
   }

  } // fin du lissage 


  // derivee en x 


  for( debut=Imagex, lin=0 ; lin<nlin ; lin++, debut+=ncol )
  {
    // derivee Causal : B1 
   

   B_courant = B1 ;
   B_precedent = B1 ;
   B_suivant = B1 ;
   pixel_courant = debut ;

   (*B_suivant) =  b5 * (*pixel_courant) ;
   B_suivant++ ;
   (*B_suivant) =  b1 * (*B_courant) - 
                   b2 * (*B_courant) +
				   b4 * (*pixel_courant);
 
   B_suivant++ ; B_courant++ ; pixel_courant++; 
   fin = B1 + ncol ;

   while(B_suivant<fin)
   {	
    (*B_suivant) = b1 * (*B_courant) - 
                   b2 * (*B_precedent)+
		   b4 * (*pixel_courant) ;
    B_suivant++ ; B_courant++ ; B_precedent++ ;
    pixel_courant++ ;
   }

  

   // derivee Anti-Causal : B2 
   B_courant = B2 + ncol1 ;
   B_precedent = B2 + ncol1 ;
   B_suivant = B2 + ncol1 ;
   pixel_courant = debut + ncol1 ;

   (*B_suivant) =  b5*(*pixel_courant) ;
   B_suivant-- ;
   (*B_suivant) =  b1 * (*B_courant) - 
                   b2 * (*B_courant) +
		   b4 * (*pixel_courant);
   B_suivant-- ;
   B_courant-- ; pixel_courant-- ; 
   fin = B2 ;

   while(B_suivant>=fin)
   {	
    (*B_suivant) = b1 * (*B_courant) -
                   b2 * (*B_precedent) +
		   b4 * (*pixel_courant) ;
    B_courant-- ; B_precedent-- ;
    B_suivant--;
    pixel_courant-- ;
   }

  
 
   B_precedent = B1 ; B_suivant = B2 ;
   dx = Imagex + ( ncol * lin ) ; fin = dx + ncol;

   while(dx<fin)
   {
    (*dx) = (*B_precedent) - (*B_suivant)  ;
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
 return 1;
 //kfree(B1) ; kfree(B2) ;
}














