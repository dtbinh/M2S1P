


clear all ;

aviobj = avifile('mymovie.avi','fps',25) ;

NomDuDossier='../Ghost3/' ;
% NomDuDossier='../Maisons/' ;

for u=-3:3
    for v=-3:3
        Masque(u+4,v+4) = exp( -( (u*u) + (v*v) ) ) ;
    end
end

Masque = Masque / sum(sum(Masque)) ;
    

for numero = 0:1
    if(numero<10)
        Nom = strcat(NomDuDossier,sprintf('GITS00%d.bmp',numero)) ;
%        Nom = strcat(NomDuDossier,sprintf('Image00%d.tif',numero)) ;
    else
        if(numero<100)
            Nom = strcat(NomDuDossier,sprintf('GITS0%d.bmp',numero)) ;
%            Nom = strcat(NomDuDossier,sprintf('Image0%d.tif',numero)) ;
        else
            Nom = strcat(NomDuDossier,sprintf('GITS%d.bmp',numero)) ;
%            Nom = strcat(NomDuDossier,sprintf('Image%d.tif',numero)) ;
        end
    end

    NumeroDeFichier=fopen(Nom) ;
    Nom,

    if(NumeroDeFichier>0)	

        fclose(NumeroDeFichier) ;
        
        MonImage = double(imread(Nom)) ;
        [Nlin,Ncol,Nplan] = size(MonImage) ;
        ImageNg = zeros(Nlin,Ncol) ;
        NlinNcol = Nlin * Ncol ;
        if Nplan>1
            for plan = 1:Nplan ImageNg(1:Nlin,1:Ncol) = ImageNg(1:Nlin,1:Ncol) + MonImage(1:Nlin,1:Ncol,plan) ; end
            MonImage = ImageNg / Nplan ;
        end
        
        [ GradientX, GradientY, GradientXY ] = DeriveImage(MonImage, 0.3, 1) ;
        IndicateurHarris = zeros(Nlin,Ncol) ;
        DetecteurHarris = zeros(Nlin,Ncol) ;
        
        for lin=4:Nlin-3
            for col=4:Ncol-3
                
                M = zeros(2,2) ;
                for u=-3:3
                    for v=-3:3
                        M(1,1) = M(1,1) + ( Masque(u+4,v+4) * GradientX(lin+u,col+v) * GradientX(lin+u,col+v) ) ;
                        M(1,2) = M(1,2) + ( Masque(u+4,v+4) * GradientX(lin+u,col+v) * GradientY(lin+u,col+v) ) ;
                        M(2,2) = M(2,2) + ( Masque(u+4,v+4) * GradientY(lin+u,col+v) * GradientY(lin+u,col+v) ) ;
                        M(2,1) = M(1,2) ;
                    end
                end
                
                det = ( M(1,1)*M(2,2) ) - ( M(1,2)*M(1,2) ) ;
                trace = M(1,1) + M(2,2) ;
                if(abs(trace)>1.0e-15)
                    IndicateurHarris(lin,col) = det / trace ;
                end
%IndicateurHarris(lin,col) = det ;
                
            end
        end

        figure(50) ; image(uint8((255*(IndicateurHarris-min(min(IndicateurHarris)))/(max(max(IndicateurHarris))-min(min(IndicateurHarris)))))) ; axis image ;
        
        k=1 ;
        
        ValeurTriee = zeros(NlinNcol,1) ;
        DetecteurHarris = IndicateurHarris ;
        
        for lin=4:Nlin-3
            for col=4:Ncol-3
                valeur_max = IndicateurHarris(lin-1,col-1) ;
                for u=-1:1
                    for v=-1:1                
                        if( (u~=0) || (v~=0) ) valeur_max = max( valeur_max, IndicateurHarris(lin+u,col+v) ) ; end
                    end
                end
                if(IndicateurHarris(lin,col)<=valeur_max) DetecteurHarris(lin,col) = 0.0 ; end
                if( (lin<30) || (lin>(Nlin-29)) || (col<30) || (col>(Ncol-29)) ) DetecteurHarris(lin,col) = 0.0 ; end
                ValeurTriee(k) = DetecteurHarris(lin,col) ; k=k+1 ;
                
            end
        end


       figure(51) ; image(uint8((255*(DetecteurHarris-min(min(DetecteurHarris)))/(max(max(DetecteurHarris))-min(min(DetecteurHarris)))))) ;  axis image ;
       
       ValeurTriee = sort(ValeurTriee) ;
       seuil = ValeurTriee(NlinNcol-128) ;
       k=1 ;
       x=zeros(1,1) ; y=zeros(1,1) ;
       
       for lin=4:Nlin-3
            for col=4:Ncol-3
                if(DetecteurHarris(lin,col)<=seuil) DetecteurHarris(lin,col) = 0.0 ; else x(k) = lin ; y(k) = col ; k=k+1 ; end                
            end
       end

       figure(52) ; image(uint8((255*(DetecteurHarris-min(min(DetecteurHarris)))/(max(max(DetecteurHarris))-min(min(DetecteurHarris)))))) ; axis image ;
%       figure(52) ; surf(DetecteurHarris) ; shading interp ; rotate3d on ;
       h53 = figure(253) ; hold off ; image(uint8((MonImage))) ; axis image ; colormap(gray(256)) ; hold on ; plot(y,x,'o') ; axis off ; drawnow ;
       

       saveas(h53,'imagetemp.bmp','bmp') ;
       ImageCouleur = imread('imagetemp.bmp') ;
       imwrite(ImageCouleur(177:690,156:1087,:),'imagetemp.bmp') ;
       ImageCouleur = imread('imagetemp.bmp') ;

       figure(100) ; image(uint8(ImageCouleur)) ; axis image ; drawnow ;
       
       aviobj = addframe(aviobj,ImageCouleur);

    end
end

if(0)
for numero = 118:-1:2 
    if(numero<10)
        Nom = strcat(NomDuDossier,sprintf('GITS00%d.bmp',numero)) ;
    else
        if(numero<100)
            Nom = strcat(NomDuDossier,sprintf('GITS0%d.bmp',numero)) ;
        else
            Nom = strcat(NomDuDossier,sprintf('GITS%d.bmp',numero)) ;
        end
    end

    NumeroDeFichier=fopen(Nom) ;
    Nom,

    if(NumeroDeFichier>0)	

        fclose(NumeroDeFichier) ;
        
        MonImage = double(imread(Nom)) ;
        [Nlin,Ncol] = size(MonImage) ;
        NlinNcol = Nlin * Ncol ;
        
        [ GradientX, GradientY, GradientXY ] = DeriveImage(MonImage, 0.3, 1) ;
        IndicateurHarris = zeros(Nlin,Ncol) ;
        DetecteurHarris = zeros(Nlin,Ncol) ;
        
        for lin=4:Nlin-3
            for col=4:Ncol-3
                
                M = zeros(2,2) ;
                for u=-3:3
                    for v=-3:3
                        M(1,1) = M(1,1) + ( Masque(u+4,v+4) * GradientX(lin+u,col+v) * GradientX(lin+u,col+v) ) ;
                        M(1,2) = M(1,2) + ( Masque(u+4,v+4) * GradientX(lin+u,col+v) * GradientY(lin+u,col+v) ) ;
                        M(2,2) = M(2,2) + ( Masque(u+4,v+4) * GradientY(lin+u,col+v) * GradientY(lin+u,col+v) ) ;
                        M(2,1) = M(1,2) ;
                    end
                end
                
                det = ( M(1,1)*M(2,2) ) - ( M(1,2)*M(1,2) ) ;
                trace = M(1,1) + M(2,2) ;
                if(abs(trace)>1.0e-15)
                    IndicateurHarris(lin,col) = det / trace ;
                end
%IndicateurHarris(lin,col) = det ;
                
            end
        end

        figure(50) ; image(uint8((255*(IndicateurHarris-min(min(IndicateurHarris)))/(max(max(IndicateurHarris))-min(min(IndicateurHarris)))))) ;
        
        k=1 ;
        
        ValeurTriee = zeros(NlinNcol,1) ;
        DetecteurHarris = IndicateurHarris ;
        
        for lin=4:Nlin-3
            for col=4:Ncol-3
                valeur_max = IndicateurHarris(lin-1,col-1) ;
                for u=-1:1
                    for v=-1:1                
                        if( (u~=0) || (v~=0) ) valeur_max = max( valeur_max, IndicateurHarris(lin+u,col+v) ) ; end
                    end
                end
                if(IndicateurHarris(lin,col)<=valeur_max) DetecteurHarris(lin,col) = 0.0 ; end
                if( (lin<30) || (lin>(Nlin-29)) || (col<30) || (col>(Ncol-29)) ) DetecteurHarris(lin,col) = 0.0 ; end
                ValeurTriee(k) = DetecteurHarris(lin,col) ; k=k+1 ;
                
            end
        end


       figure(51) ; image(uint8((255*(DetecteurHarris-min(min(DetecteurHarris)))/(max(max(DetecteurHarris))-min(min(DetecteurHarris)))))) ;
       
       ValeurTriee = sort(ValeurTriee) ;
       seuil = ValeurTriee(NlinNcol-128) ;
       k=1 ;
       x=zeros(1,1) ; y=zeros(1,1) ;
       
       for lin=4:Nlin-3
            for col=4:Ncol-3
                if(DetecteurHarris(lin,col)<=seuil) DetecteurHarris(lin,col) = 0.0 ; else x(k) = lin ; y(k) = col ; k=k+1 ; end                
            end
       end

       figure(52) ; image(uint8((255*(DetecteurHarris-min(min(DetecteurHarris)))/(max(max(DetecteurHarris))-min(min(DetecteurHarris)))))) ;
%       figure(52) ; surf(DetecteurHarris) ; shading interp ; rotate3d on ;
       h53 = figure(53) ; hold off ; image(uint8((MonImage))) ; axis image ; colormap(gray(256)) ; hold on ; plot(y,x,'o') ; axis off ; drawnow ;
       
       if(numero<10)
           Nom = strcat(NomDuDossier,sprintf('Harris00%d.bmp',numero)) ;
       else
           if(numero<100)
               Nom = strcat(NomDuDossier,sprintf('Harris0%d.bmp',numero)) ;
           else
               Nom = strcat(NomDuDossier,sprintf('Harris%d.bmp',numero)) ;
           end
       end

       saveas(h53,'imagetemp.bmp','bmp') ;
       ImageCouleur = imread('imagetemp.bmp') ;
       imwrite(ImageCouleur(177:690,156:1087,:),'imagetemp.bmp') ;
       ImageCouleur = imread('imagetemp.bmp') ;

       figure(100) ; image(uint8(ImageCouleur)) ; axis image ; drawnow ;
       
       aviobj = addframe(aviobj,ImageCouleur);
    end
end
end

aviobj = close(aviobj);


