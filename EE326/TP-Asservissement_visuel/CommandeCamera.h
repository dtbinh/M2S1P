///////////////////////////////////////////////////////////////////////////
//  Driver permettant la commande de camera Pan/Tilt SONY EVI/D100P
//  Developpe par T. Duquesne  pour O. Strauss le 07/2006
//  Pour TP Automatique Master 1 STPI 
/////////////////////////////////////////////////////////////////////////////

#ifndef _COMMANDE_CAMERA_H
#define _COMMANDE_CAMERA_H

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
}  e_ErrCom;

// PROTOTYPES des Fonctions
//----------------------------------------------------------------------------
e_ErrCom OuvreCom(char *strPort,long BaudRate,int BitsSize,int Parity,int StopBits);
e_ErrCom EmissionCom(const void *emBuf, unsigned int nCount);
e_ErrCom ReceptionCom(void *lpBuf, unsigned int nCountMax, unsigned int *pCountRead);
void FermeCom();
void Pan_Tilt(double vitesse_pan, double vitesse_tilt);
void camera_on(void);
void camera_off(void);
void camera_home(void);void FermeCom();
void zoom_stop(void);
void zoom_in(int speed_in);
void zoom_out(int speed_out);
void zoom_position(int position);
void zoom_init(void);
void Zoom(int speed) ;



#endif
