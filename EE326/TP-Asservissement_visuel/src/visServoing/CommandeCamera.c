///////////////////////////////////////////////////////////////////////////
//  Driver permettant la commande de camera Pan/Tilt SONY EVI/D100P
//  Developpe par T. Duquesne  pour O. Strauss le 07/2006
//  Pour TP Automatique Master 1 STPI 
/////////////////////////////////////////////////////////////////////////////


//#include "stdafx.h"
#include <windows.h>
#include <winbase.h>
#include <stdio.h>
//#include <conio.h>
#include <string.h>
#include <math.h>

//#includes <pan_tilt.h>

// Definition des codes d'erreurs
typedef enum
{
 e_ErrCom_None,      // Pas d'erreur
 e_ErrCom_Creation,  // Erreur lors de la creation du flux
 e_ErrCom_Utilise,       // Le port com est dejˆ utilise
 e_ErrCom_Inexistant,    // Le port com n'existe pas
 e_ErrCom_Timeout,   // Timeout lors d'une emission-reception
 e_ErrCom_Emission,      // Erreur lors de l'emission
 e_ErrCom_Reception,     // Erreur lors de la reception
 e_ErrCom_Definition_Trame,  // Erreur de definition de la trame
 e_ErrCom_Nack,  // Demande non prise en compte
 e_ErrCom_Checksum       // Erreur de checksum
} e_ErrCom;

// Definition des vitesses de communication
#define V1200      CBR_1200
#define V2400      CBR_2400
#define V4800      CBR_4800
#define V9600      CBR_9600
#define V19200     CBR_19200
#define V57600     CBR_57600
#define V115200    CBR_115200
// Definition du nombre de bits
#define BITS_7  7
#define BITS_8  8
// Definition du nombre de bits de stop
#define BIT_DE_STOP_1   ONESTOPBIT
#define BIT_DE_STOP_15  ONE5STOPBITS
#define BIT_DE_STOP_2   TWOSTOPBITS
// Definition de la parite
#define PAS_DE_PARITE   NOPARITY
#define PARITE_IMPAIRE  ODDPARITY
#define PARITE_PAIRE    EVENPARITY
// Codes de retour generiques
#define OK 1
#define KO 0
// Longueur max reservee pour une trame
#define LG_TRAME    100

// PROTOTYPES des Fonctions
//----------------------------------------------------------------------------
e_ErrCom OuvreCom(char *strPort,long BaudRate,int BitsSize,int Parity,int StopBits);
e_ErrCom EmissionCom(const void *emBuf, unsigned int nCount);
e_ErrCom ReceptionCom(void *lpBuf, unsigned int nCountMax, unsigned int *pCountRead);
void FermeCom();
void Pan_Tilt(double vitesse_pan, double vitesse_tilt);
void camera_on(void);
void camera_off(void);
void camera_home(void);
void zoom_stop(void);
void zoom_in(int speed_in);
void zoom_out(int speed_out);
void zoom_position(int position);
void zoom_init(void);
void Zoom(int speed) ;



//void sendInt(int value);

// VARIABLES GLOBALES
//-----------------------------------------------------------------------------
DCB g_DCB;                          // structure dcb du port
e_ErrCom g_ErrCom= e_ErrCom_None;   // Variable des erreurs de com
HANDLE g_hCom = 0;					// handle de la com

char *pcCommPort = "COM1";			// definition du port courant

unsigned char lpBuf[2];
unsigned int pCountRead;
BOOL fSuccess;


//-----------------------------------------------------------------------
// FONCTION : pan_tilt(vitesse_pan, vitesse_tilt)
//-----------------------------------------------------------------------
// DESCRIPTION  :
// permet ˆ la camera de se deplacer en X(Pan) et en Y(Tilt)
//
//-----------------------------------------------------------------------
// PARAMETRES   :
//      vitesse_pan  : deplacement en X, valeurs comprises entre -1 et 1
//      vitesse_tilt : deplacement en Y, valeurs comprises entre -1 et 1
//-----------------------------------------------------------------------
// RETOUR   :Neant
//-----------------------------------------------------------------------

//   Utilisation de la fonction : pan_tilt(vitesse_pan, vitesse_tilt)
//
//     si vitesse_pan > 0 et vitesse_tilt > 0 alors deplacement "up_right"
//     si vitesse_pan > 0 et vitesse_tilt < 0 alors deplacement "up_left"
//     si vitesse_pan < 0 et vitesse_tilt > 0 alors deplacement "down_right"
//     si vitesse_pan < 0 et vitesse_tilt < 0 alors deplacement "down_left"
//
//     si vitesse_pan = 0 et vitesse_tilt > 0 alors deplacement "up"
//     si vitesse_pan = 0 et vitesse_tilt < 0 alors deplacement "down"
//     si vitesse_pan > 0 et vitesse_tilt = 0 alors deplacement "right"
//     si vitesse_pan < 0 et vitesse_tilt = 0 alors deplacement "left"
//

       void Pan_Tilt(double vitesse_pan, double vitesse_tilt)
       {
        int trame = 0;
        int i ;
        double vit_pan = 0;
        double vit_tilt = 0;
        int value_up_right[9] = {129,1,6,1,0,0,2,1,255};
        int value_up_left[9] = {129,1,6,1,0,0,2,2,255};
        int value_down_right[9] = {129,1,6,1,0,0,1,1,255};
        int value_down_left[9] = {129,1,6,1,0,0,1,2,255};
        int value_up[9] = {129,1,6,1,0,0,3,1,255};
        int value_down[9] = {129,1,6,1,0,0,3,2,255};
        int value_right[9] = {129,1,6,1,0,0,2,3,255};
        int value_left[9] = {129,1,6,1,0,0,1,3,255};
        int stop[9] = {129,1,6,1,0,0,3,3,255};
    
        vit_pan = vitesse_pan;
        vit_tilt = vitesse_tilt;


        if((vit_pan == 0) & (vit_tilt == 0)) //arret deplacement
        {
         OuvreCom(pcCommPort,V9600,BITS_8,PAS_DE_PARITE,BIT_DE_STOP_1);
         for(i=0 ; i<9 ; i++) EmissionCom(&stop[i],1);
         FermeCom();
         trame = 1; //permettra de gagner des cycles d'horloges CPU
         }
         //else;
         if(trame == 0) // test permettant de gagner du CPU
         {
                  if((vit_pan == 0) & (vit_tilt > 0)) //deplacement up
                  {
                  value_up[5] = (int)(floor(20*vit_tilt));
                  OuvreCom(pcCommPort,V9600,BITS_8,PAS_DE_PARITE,BIT_DE_STOP_1);
                  for(i=0 ; i<9 ; i++)
                  EmissionCom(&value_up[i],1);
                  FermeCom();
                  trame = 1;
                  }
         }
         //else;

         if(trame == 0) // test permettant de gagner du CPU
         {
                  if((vit_pan == 0) & (vit_tilt < 0)) //deplacement down
                  {
                  value_down[5] = (int)(floor(-20*vit_tilt));
                  OuvreCom(pcCommPort,V9600,BITS_8,PAS_DE_PARITE,BIT_DE_STOP_1);
                  for(i=0 ; i<9 ; i++)
                  EmissionCom(&value_down[i],1);
                  FermeCom();
                  trame = 1;
                  }
         }
         //else;
         if(trame == 0)
         {
                  if((vit_tilt == 0) & (vit_pan > 0)) //deplacement right
                  {
                  value_right[4] = (int)(floor(24*vit_pan));
                  OuvreCom(pcCommPort,V9600,BITS_8,PAS_DE_PARITE,BIT_DE_STOP_1);
                  for(i=0 ; i<9 ; i++)
                  EmissionCom(&value_right[i],1);
                  FermeCom();
                  trame = 1;
                  }
         }

         if(trame == 0)
         {
                  if((vit_tilt == 0) & (vit_pan < 0)) //deplacement left
                  {
                  value_left[4] = (int)(floor(-24*vit_pan));
                  OuvreCom(pcCommPort,V9600,BITS_8,PAS_DE_PARITE,BIT_DE_STOP_1);
                  for(i=0 ; i<9 ; i++)
                  EmissionCom(&value_left[i],1);
                  FermeCom();
                  trame = 1;
                  }
         }

         if(trame == 0)
         {
                  if((vit_pan > 0) & (vit_tilt > 0)) //deplacement up_right
                  {
                  value_up_right[4] = (int)(floor(24*vit_pan));
                  value_up_right[5] = (int)(floor(20*vit_tilt));
                  OuvreCom(pcCommPort,V9600,BITS_8,PAS_DE_PARITE,BIT_DE_STOP_1);
                  for(i=0 ; i<9 ; i++)
                  EmissionCom(&value_up_right[i],1);
                  FermeCom();
                  trame = 1;
                  }
         }

         if(trame == 0)
         {
                  if((vit_pan < 0) & (vit_tilt < 0)) //deplacement down_left
                  {
                  value_down_left[4] = (int)(floor(-24*vit_pan));
                  value_down_left[5] = (int)(floor(-20*vit_tilt));
                  OuvreCom(pcCommPort,V9600,BITS_8,PAS_DE_PARITE,BIT_DE_STOP_1);
                  for(i=0 ; i<9 ; i++)
                  EmissionCom(&value_down_left[i],1);
                  FermeCom();
                  trame = 1;
                  }
         }

         if(trame == 0)
         {
                  if((vit_pan < 0) & (vit_tilt > 0)) //deplacement down_right
                  {
                  value_down_right[4] = (int)(floor(-24*vit_pan));
                  value_down_right[5] = (int)(floor(20*vit_tilt));
                  OuvreCom(pcCommPort,V9600,BITS_8,PAS_DE_PARITE,BIT_DE_STOP_1);
                  for(i=0 ; i<9 ; i++)
                  EmissionCom(&value_down_right[i],1);
                  FermeCom();
                  trame = 1;
                  }
         }


         if((vit_pan > 0) & (vit_tilt < 0)) //deplacement up_left
         {
         value_up_left[4] = (int)(floor(24*vit_pan));
         value_up_left[5] = (int)(floor(-20*vit_tilt));
         OuvreCom(pcCommPort,V9600,BITS_8,PAS_DE_PARITE,BIT_DE_STOP_1);
         for(i=0 ; i<9 ; i++)
         EmissionCom(&value_up_left[i],1);
         FermeCom();
         }
         //else;

         }


//-----------------------------------------------------------------------
// FONCTION : camera_on
//-----------------------------------------------------------------------
// DESCRIPTION  :
// Permet de mettre la camera sous tension
//
//-----------------------------------------------------------------------
// PARAMETRES   :
//      Neant
//-----------------------------------------------------------------------
// RETOUR   :Neant
//-----------------------------------------------------------------------

         void camera_on(void)
         {
         int value_camera_on[6] = {129,1,4,0,2,255};
         int i ;
         OuvreCom(pcCommPort,V9600,BITS_8,PAS_DE_PARITE,BIT_DE_STOP_1);
         for(i=0 ; i<6 ; i++)
         EmissionCom(&value_camera_on[i],1);
         FermeCom();
         }

//-----------------------------------------------------------------------
// FONCTION : camera_off
//-----------------------------------------------------------------------
// DESCRIPTION  :
// Permet de mettre la camera sous tension
//
//-----------------------------------------------------------------------
// PARAMETRES   :
//      Neant
//-----------------------------------------------------------------------
// RETOUR   :Neant
//-----------------------------------------------------------------------

         void camera_off(void)
         {
         int value_camera_off[6] = {129,1,4,0,3,255};
         int i ;
         OuvreCom(pcCommPort,V9600,BITS_8,PAS_DE_PARITE,BIT_DE_STOP_1);
         for(i=0 ; i<6 ; i++)
         EmissionCom(&value_camera_off[i],1);
         FermeCom();
         }
//-----------------------------------------------------------------------
// FONCTION : camera_home
//-----------------------------------------------------------------------
// DESCRIPTION  :
// Permet de remettre la camera en position d'origine
//
//-----------------------------------------------------------------------
// PARAMETRES   :
//      Neant
//-----------------------------------------------------------------------
// RETOUR   :Neant
//-----------------------------------------------------------------------

         void camera_home(void)
         {
         int value_camera_home[5] = {129,1,6,4,255};
         int i ;
         OuvreCom(pcCommPort,V9600,BITS_8,PAS_DE_PARITE,BIT_DE_STOP_1);
         for(i=0 ; i<5 ; i++)
         EmissionCom(&value_camera_home[i],1);
         FermeCom();
         }



//------------------------------------------------------------------------------
// FONCTION : OuvreCom
//------------------------------------------------------------------------------
// DESCRIPTION  :
// Initialise et ouvre un port serie
//
//------------------------------------------------------------------------------
// PARAMETRES   :
//      - strPort       Nom du port "COM1", "COM2"
//      - BaudRate      Vitesse
//      - BitsSize      Taille de l'info
//      - Parity        Parite
//      - StopBits      Nombre de bits de stop
//
// RETOUR   :Code d'erreur
//------------------------------------------------------------------------------
e_ErrCom OuvreCom(char *strPort,long BaudRate,int BitsSize,int Parity,int StopBits)
{
    g_ErrCom = e_ErrCom_None;
    // On ouvre le port serie
    g_hCom = CreateFile(strPort,
						GENERIC_READ | GENERIC_WRITE,
						0,
						NULL,
						OPEN_EXISTING,
						0,
						NULL
						);

    if(g_hCom == INVALID_HANDLE_VALUE)
    {
        // Echec
        g_ErrCom=e_ErrCom_Creation;
    }
    else
    {
		//acquisition de la Configuration actuelle
		fSuccess = GetCommState(g_hCom, &g_DCB);
		if (!fSuccess) 
		{
		// Handle the error.
			printf ("Erreur sur GetCommState n¡: %d.\n", GetLastError());
			g_ErrCom=e_ErrCom_Creation;
		}	
		else
		{
       // On vide les buffers
			PurgeComm(g_hCom,PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);      
        // On paramtre le port serie
			g_DCB.DCBlength = sizeof(DCB);
		//Modification du DCB
			g_DCB.BaudRate=BaudRate;
			g_DCB.ByteSize=BitsSize;
			g_DCB.Parity=Parity;
			g_DCB.StopBits=StopBits;
			g_DCB.fDtrControl=DTR_CONTROL_DISABLE;
			g_DCB.fRtsControl=RTS_CONTROL_DISABLE;

		//Configuration de la liaison serie
			SetCommState(g_hCom,&g_DCB);
		}
    }
    return g_ErrCom;
}

//----------------------------------------------------------------------------
// FONCTION : EmissionCom
//----------------------------------------------------------------------------
// DESCRIPTION  :
// Emission d'octets sur la liaison serie
//
//----------------------------------------------------------------------------
// PARAMETRES   :
//      -lpBuf Pointeur sur les octets a emettre
//      -nCount Nombre d'octet a emettre
//
//----------------------------------------------------------------------------
// RETOUR   :Code d'erreur
//----------------------------------------------------------------------------
e_ErrCom EmissionCom(const void * emBuf,unsigned int nCount)
{   
    DWORD NumBytes=0;
    if(g_hCom!=NULL)
    {
        // On pari sur pas d'erreur
        g_ErrCom=e_ErrCom_None;
        //Emission du buffer
        if(WriteFile(g_hCom,emBuf,nCount,&NumBytes,NULL)==0)
        {
            g_ErrCom=e_ErrCom_Emission;
			printf("erreur emission");
        }
    }
    else
        //Le port n'a pas ete ouvert
        g_ErrCom=e_ErrCom_Creation;
    return g_ErrCom;
}

//---------------------------------------------------------------------------
// FONCTION : ReceptionCom
//---------------------------------------------------------------------------
// DESCRIPTION  :
// Reception de caractres sur la liaison serie
//
//---------------------------------------------------------------------------
// PARAMETRES   :
//      -lpBuf Pointeur sur le buffer de caractre a lire
//      -nCountMax Nombre maxi de caractre a lire
//      -pCountRead Pointeur sur le nombre de caractres lus
//---------------------------------------------------------------------------
// RETOUR   :Code d'erreur
//---------------------------------------------------------------------------
e_ErrCom ReceptionCom(void *lpBuf,unsigned int nCountMax, unsigned int *pCountRead)
{
COMSTAT Stat;
DWORD Errors;
unsigned int nCarALire;
unsigned long NCarLus=0;

    if(g_hCom!=NULL)
    {
        //on pari sur pas d'erreur
        g_ErrCom=e_ErrCom_None;
        //Pour eviter de gerer un time out
        Sleep(50);
        //Pour connaitre le nombre d'octets dans le buffer d'entree
        ClearCommError(g_hCom,&Errors,&Stat);
        nCarALire=Stat.cbInQue;
        //On effectue la lecture si il y a des caractres presents
        if( (nCarALire>0)&&(nCarALire<=nCountMax) )
        {
            if(ReadFile(g_hCom,lpBuf,nCarALire,&NCarLus,NULL)==0)
            {       
                g_ErrCom=e_ErrCom_Reception;
				printf("erreur reception");
            }
        }
        *pCountRead=NCarLus;
    }
    else
        //Le port n a pas ete ouvert
        g_ErrCom=e_ErrCom_Creation;
	//Compte rendu de l'execution
    return g_ErrCom;
}

//-----------------------------------------------------------------------
// FONCTION : FermeCom
//-----------------------------------------------------------------------
// DESCRIPTION  :
// Ferme le port serie prealablement ouvert avec OuvreCom
//
//-----------------------------------------------------------------------
// PARAMETRES   :
//      Neant
//-----------------------------------------------------------------------
// RETOUR   :Neant
//-----------------------------------------------------------------------
void FermeCom()
{
    if(g_hCom!=NULL)
    {
        CloseHandle(g_hCom);
    }
}

void sendInt(int value)
{
    g_ErrCom=EmissionCom(&value,1);
    do
	{
	    g_ErrCom=ReceptionCom(lpBuf, 1, &pCountRead);	
	}while (pCountRead != 1);
	
	value >>= 8;
    g_ErrCom=EmissionCom(&value,1);
    do
	{
	    g_ErrCom=ReceptionCom(lpBuf, 1, &pCountRead);	
	}while (pCountRead != 1);

	value >>= 8;
    g_ErrCom=EmissionCom(&value,1);
    do
	{
	    g_ErrCom=ReceptionCom(lpBuf, 1, &pCountRead);	
	}while (pCountRead != 1);

	value >>= 8;
    g_ErrCom=EmissionCom(&value,1);
    do
	{
	    g_ErrCom=ReceptionCom(lpBuf, 1, &pCountRead);	
	}while (pCountRead != 1);
}

//-----------------------------------------------------------------------
// FONCTION : zoom_stop
//-----------------------------------------------------------------------
// DESCRIPTION  :
// Permet d'arrêter la progression du zoom
//
//-----------------------------------------------------------------------
// PARAMETRES   :
//      Néant
//-----------------------------------------------------------------------
// RETOUR   :Néant
//-----------------------------------------------------------------------

void zoom_stop(void)
{
 int i=0;
 int value_zoom_stop[6] = {129,1,4,7,0,255};
 OuvreCom(pcCommPort,V9600,BITS_8,PAS_DE_PARITE,BIT_DE_STOP_1);
 for(i=0 ; i<6 ; i++)
 EmissionCom(&value_zoom_stop[i],1);
 FermeCom();
}

//-----------------------------------------------------------------------
// FONCTION : zoom_in
//-----------------------------------------------------------------------
// DESCRIPTION  :
// Permet d'augmenter le zoom
//
//-----------------------------------------------------------------------
// PARAMETRES   :
//      speed_in de 0 (lente) à 7 (rapide)
//-----------------------------------------------------------------------
// RETOUR   :Néant
//-----------------------------------------------------------------------

void zoom_in(int speed_in)
{
 int speed = 0;
 int i=0;
 int value_zoom_in[6] = {129,1,4,7,2,255};
 speed = speed_in;

 if(speed < 0) speed = 0;
 if(speed > 7) speed = 7;

 value_zoom_in[4] = (speed + 0x20);
 OuvreCom(pcCommPort,V9600,BITS_8,PAS_DE_PARITE,BIT_DE_STOP_1);
 for(i=0 ; i<6 ; i++) EmissionCom(&value_zoom_in[i],1);
 FermeCom();
}

//-----------------------------------------------------------------------
// FONCTION : zoom_out
//-----------------------------------------------------------------------
// DESCRIPTION  :
// Permet de diminuer zoom
//
//-----------------------------------------------------------------------
// PARAMETRES   :
//      speed_out de 0 (lente) à 7 (rapide)
//-----------------------------------------------------------------------
// RETOUR   :Néant
//-----------------------------------------------------------------------

void zoom_out(int speed_out)
{
 int speed = 0;
 int i=0;
 int value_zoom_out[6] = {129,1,4,7,3,255};

 speed = speed_out;
 if(speed < 0) speed = 0;
 if(speed > 7) speed = 7;
 value_zoom_out[4] = (speed + 0x30);
 OuvreCom(pcCommPort,V9600,BITS_8,PAS_DE_PARITE,BIT_DE_STOP_1);
 for(i=0 ; i<6 ; i++) EmissionCom(&value_zoom_out[i],1);
 FermeCom();
}

//-----------------------------------------------------------------------
// FONCTION : Zoom
//-----------------------------------------------------------------------
// DESCRIPTION  :
// Permet de faire varier le zoom
//
//-----------------------------------------------------------------------
// PARAMETRES   :
//      speed de -7 zoom out rapide à 7 zoom in rapide
//-----------------------------------------------------------------------
// RETOUR   :Néant
//-----------------------------------------------------------------------

void Zoom(int speed)
{
 int i=0;
 int value_zoom_out[6] = {129,1,4,7,3,255};
 int value_zoom_in[6] = {129,1,4,7,2,255};
 int value_zoom_stop[6] = {129,1,4,7,0,255};

 if(speed<0) 
 {
  speed = - speed ;
  if(speed > 7) speed = 7;
  value_zoom_out[4] = (speed + 0x30);
  OuvreCom(pcCommPort,V9600,BITS_8,PAS_DE_PARITE,BIT_DE_STOP_1);
  for(i=0 ; i<6 ; i++) EmissionCom(&value_zoom_out[i],1);
  FermeCom();
 }
 else
 {
  if(speed>0) 
  {
   if(speed > 7) speed = 7;
   value_zoom_in[4] = (speed + 0x20);
   OuvreCom(pcCommPort,V9600,BITS_8,PAS_DE_PARITE,BIT_DE_STOP_1);
   for(i=0 ; i<6 ; i++) EmissionCom(&value_zoom_in[i],1);
   FermeCom();
  }
  else
  {
   OuvreCom(pcCommPort,V9600,BITS_8,PAS_DE_PARITE,BIT_DE_STOP_1);
   for(i=0 ; i<6 ; i++) EmissionCom(&value_zoom_stop[i],1);
   FermeCom();
  }
 }
}

//-----------------------------------------------------------------------
// FONCTION : zoom_position
//-----------------------------------------------------------------------
// DESCRIPTION  :
// Permet de définir la position du zoom
//
//-----------------------------------------------------------------------
// PARAMETRES   :
//      position de 0 (zoom faible) à 16384 (zoom fort)
//-----------------------------------------------------------------------
// RETOUR   :Néant
//-----------------------------------------------------------------------

void zoom_position(int position)
{
 int position_zoom = 0;
 position_zoom = position;
 if(position < 0) position_zoom = 0;
 if(position > 0x4000) position_zoom = 0x4000;
 else
 {
  int i=0;
  int value_zoom_position[9] = {129,1,4,71,0,0,0,0,255};
  value_zoom_position[4] = (int)((position_zoom & 0XF000)>>12);
  value_zoom_position[5] = (int)((position_zoom & 0X0F00)>>8);
  value_zoom_position[6] = (int)((position_zoom & 0X00F0)>>4);
  value_zoom_position[7] = (int)(position_zoom & 0X000F);
  OuvreCom(pcCommPort,V9600,BITS_8,PAS_DE_PARITE,BIT_DE_STOP_1);
  for(i=0 ; i<9 ; i++) EmissionCom(&value_zoom_position[i],1);
  FermeCom();
 }
}
         
//-----------------------------------------------------------------------
// FONCTION : zoom_init
//-----------------------------------------------------------------------
// DESCRIPTION  :
// Permet d'initialiser la position du zoom
//
//-----------------------------------------------------------------------
// PARAMETRES   :
//      aucun
//-----------------------------------------------------------------------
// RETOUR   :Néant
//-----------------------------------------------------------------------

void zoom_init(void)
{
 int position_zoom = 8000; //valeur de 0 à 16384
 int i=0;
 int value_zoom_position[9] = {129,1,4,71,0,0,0,0,255};
 value_zoom_position[4] = (int)((position_zoom & 0XF000)>>12);
 value_zoom_position[5] = (int)((position_zoom & 0X0F00)>>8);
 value_zoom_position[6] = (int)((position_zoom & 0X00F0)>>4);
 value_zoom_position[7] = (int)(position_zoom & 0X000F);
 OuvreCom(pcCommPort,V9600,BITS_8,PAS_DE_PARITE,BIT_DE_STOP_1);
 for(i=0 ; i<9 ; i++) EmissionCom(&value_zoom_position[i],1);
 FermeCom();
}