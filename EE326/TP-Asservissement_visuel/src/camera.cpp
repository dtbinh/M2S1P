
#include <opencv2/opencv.hpp>
#include <time.h>
#include <curl/curl.h>

#include <stdio.h>
//#include <conio.h>
#include <stdlib.h>
#include <math.h>
//#include "visServoing/CommandeCamera.h"
#include "visServoing/Definitions.h"
#include "visServoing/Castan.h"
#include "visServoing/Gauss.h"

#define DCF_FILE "M_CCIR"

CURL * connection_handle;
char error_buffer[CURL_ERROR_SIZE];

#define CAMERA_IP "http://192.168.1.2"
#define CAMERA_LOG "admin"
#define CAMERA_PWD "camera"
#define CAMERA_LIVE_VIEW1 "rtsp://192.168.1.2:554/play1.sdp"
#define CAMERA_LIVE_VIEW2 "rtsp://192.168.1.2:554/play2.sdp"

using namespace cv;

static size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp){
    //do nothing do not forward hatml page to other anyplace
    size_t realsize = size * nmemb;
    return realsize;
}


int camera_Move_Relative(int x,int y)
{
    char params[100];
    string ip_command = CAMERA_IP;
    ip_command += "/cgi/ptdc.cgi?command=set_relative_pos";
    sprintf(params, "&posX=%d&posY=%d",x,y);
    ip_command += params;
    curl_easy_setopt(connection_handle, CURLOPT_URL, ip_command.c_str());	//IP command for moving pan/tilt of the camera

    //printf("sending %s\n",ip_command.c_str());
    if(curl_easy_perform(connection_handle) != CURLE_OK){ /* transfer http */
        printf("[ERROR] sending request : %s\n", error_buffer);
    }
    return 1;
}



int main(int argc, char* argv[])
{
    printf("start ");
    //init new camera
    char key='a';
    IplImage *imageRGB = NULL;
    IplImage *image = NULL;
    string loginandpasswd = CAMERA_LOG;
    loginandpasswd += ":";
    loginandpasswd += CAMERA_PWD;
    CvCapture *capture = NULL;
    CvFont police_caractere;

    //initialize HTTP connection
    if(curl_global_init(CURL_GLOBAL_ALL) != 0) {
        printf("[PROBLEM] when initializing\n");
        exit(0);
    }
    connection_handle = curl_easy_init();
    curl_easy_setopt(connection_handle, CURLOPT_HTTPAUTH, CURLAUTH_ANY);		//basic authentification method
    curl_easy_setopt(connection_handle, CURLOPT_USERPWD, loginandpasswd.c_str());	//login and passwd
    curl_easy_setopt(connection_handle, CURLOPT_ERRORBUFFER, error_buffer);		//allocating error buffer
    curl_easy_setopt(connection_handle, CURLOPT_WRITEFUNCTION, write_data);		//returned html pages are forwarded to the write_data function

    capture=cvCaptureFromFile(CAMERA_LIVE_VIEW2);
    if(!capture)
    {
        printf("Main=> Erreur capture du flux video impossible");
        return -1;
    }
    cvNamedWindow("Dlink Camera",CV_WINDOW_AUTOSIZE);
    cvInitFont( &police_caractere , 0  , 0.5f , 0.5f , 0.2f , 1 , 8 );

    //init old camera
    unsigned char *ImageAcquise, *ptI ;
    int TailleImage, Nlin, Ncol, lin, col;
    int TailleMotif, nlin, ncol, off_lin, off_col  ;
    double *Motif;
    double *Derivee_t, *Derivee_x, *Derivee_y ;
    double *MatriceFlotOptique ;
    double *pt, *fin, *ptM ;
    double alpha_castan = 0.4 ;
    double theta_x, theta_y;
    double gain = 5;
    int uo;
    int vo;



    // Boucle permettant de visualiser l'image courante
    // On sort de la boucle en appuyant sur la touche espace
    printf("Appuyer sur touche espace pour acquerir le motif\n");
    bool first = true;

    while(key != ' ') {
        // Acquisition et affichage de l'image courante en noir et blanc
        imageRGB=cvQueryFrame(capture);
        if (first) {
            image = cvCreateImage((cvSize)(imageRGB->width,imageRGB->height), IPL_DEPTH_8U, 1);
            first = false;
        }
        cvCvtColor(imageRGB, image, CV_RGB2GRAY);
        cvShowImage("Dlink Camera",image);
        key=cvWaitKey(50);
    } // sortie de la boucle d'acquisition

    printf("------------------\n");
    Ncol = image->width;
    Nlin = image->height;
    ImageAcquise = ALLOCATION(TailleImage,unsigned char) ;
    if(ImageAcquise == NULL)
    {
        printf("Impossible d'allouer %d bytes pour une image %dx%d\n",(int)TailleImage,(int)Nlin,(int)Ncol) ;
    }
    ImageAcquise = (unsigned char *) image->imageData;
    TailleImage = Nlin * Ncol ;
    nlin = Nlin / 4 ; // Nombre de ligne du motif a poursuivre
    ncol = Ncol / 4 ; // Nombre de colonnes du motif a poursuivre
    off_lin = 3 * Nlin / 8 ; // position du motif a poursuivre
    off_col = 3 * Ncol / 8 ; // position du motif a poursuivre
    uo = nlin/2;
    vo = ncol/2;

    TailleMotif = nlin * ncol ;
    printf("------------------ Ncol %d Nlin %d \n", Ncol, Nlin);

    // Allocations des divers buffers necessaires pour l'application
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

    // Calcul de la derivee du motif
    castan(Motif, Derivee_x, Derivee_y, nlin, ncol, alpha_castan ) ; //typiquement alpha = 0.4
    printf(" taille motif is %d \n", TailleMotif);
    // Ici vous devez realiser le calcul de la matrice d'interaction

    pt = MatriceFlotOptique;
    for(col = 0; col < TailleMotif; col++) {
        (*pt++) = (*Derivee_x++);
        (*pt++) = (*Derivee_y++);
    }

    if (!PseudoInverse(MatriceFlotOptique, TailleMotif, 2)){
        printf("pseudoinverse is singular\n") ;
    } else {
        printf("pseudoinverse calculated\n") ;
    }

    // Cette boucle vous permettra de tester votre calcul
    int cnt= 0;
    const int imgPerMovement = 10;
    vector <double> theta_x_;
    vector <double> theta_y_;
    theta_x_.resize(imgPerMovement);
    theta_y_.resize(imgPerMovement);
    int lineX, lineY;


    while(key != 'e')
    {

        imageRGB=cvQueryFrame(capture);
        cvCvtColor(imageRGB, image, CV_RGB2GRAY);
        cvLine(image, cvPoint(image->width/2,image->height/2), cvPoint((double)(image->width/2)+lineX,(double)(image->height/2)+lineY), cv::Scalar(1.0),1,8,0);

        cvShowImage("Dlink Camera",image);
        key=cvWaitKey(50);

        ImageAcquise = (unsigned char *) image->imageData;
        int imgCnt = cnt % imgPerMovement;
        //printf("cnt %d imgCnt %d \n", cnt, imgCnt);

        ptM = Motif ;
        fin = Motif + TailleMotif ;
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
        // Calcul de theta_x
        // Calcul de theta_y
        theta_x_[imgCnt] = 0.;
        theta_y_[imgCnt] = 0.;

        pt = Derivee_t;
        ptM = MatriceFlotOptique;
        for(col = 0; col < TailleMotif; col++) {
            theta_y_[imgCnt] += (*ptM)*(*pt);
            ptM++;
            pt++;
        }
        pt = Derivee_t;
        for(col = 0; col < TailleMotif; col++) {
            theta_x_[imgCnt] += (*ptM)*(*pt);
            ptM++;
            pt++;
        }
        // retirez les commentaires si vous voulez voir les valeurs calculees
        //printf(" current angles are %f     %f     \n", theta_x_[imgCnt], theta_y_[imgCnt]) ;
        if ((cnt !=0) && (imgCnt == 0)){
            for (int i = 0; i < imgPerMovement; i++) {
                theta_x+= theta_x_[i];
                theta_y+= theta_y_[i];
                //printf(" summed angles are %f     %f     \n", theta_x, theta_y) ;
            }
            theta_x /= imgPerMovement;
            theta_y /= imgPerMovement;
            //printf(" averaged angles are %f     %f     \n", theta_x, theta_y) ;
            theta_x = theta_x*gain;
            theta_y = theta_y*gain;
            if (theta_x > 5) theta_x = 5;
            else if (theta_x < -5) theta_x = -5;
            if (theta_y > 5) theta_y = 5;
            else if (theta_y < -5) theta_y = -5;
            //thetaX postif a droite
            //thetaY postif en haut
            camera_Move_Relative(theta_x,theta_y);
            //camera_Move_Relative(0,theta_y);
            lineX=theta_x*10;
            lineY=theta_y*10;
            printf("move camera %f %f\n", theta_x, theta_y);
            theta_x = 0;
            theta_y = 0;
        }
        cnt++;
    } // fin du while
    // Desallocation de tous les buffers
    printf("Terminated program\n");
    cvReleaseCapture(&capture);
    cvDestroyWindow("Dlink Camera");
    curl_easy_cleanup(connection_handle);
    curl_global_cleanup();
    return 0;
}

