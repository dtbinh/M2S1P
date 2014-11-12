load stereovision;

% Nom de l'image que l'on va charger
% NomGenerique = 'Turtle' ;
% NomGenerique = 'Baixa' ;
% NomGenerique = 'Dracula' ;
% NomGenerique = 'Rock' ;
NomGenerique = 'Stereo' ;

% et son suffixe
Suffixe = '.tif' ;
% Suffixe = '.jpg' ;

% Composition des noms des images a charger et chargement de ces images
Nom = sprintf('%sG%s',NomGenerique,Suffixe) ;
ImageGauche = double(imread(Nom)) ;
figure(1) ; hold off ; image(uint8(ImageGauche)) ; hold on ; axis image ;
title('Image gauche') ; if(Nplan<3) colormap(gray(256)) ; end ; drawnow ;

Nom = sprintf('%sD%s',NomGenerique,Suffixe) ;
ImageDroite = double(imread(Nom)) ;
figure(2) ; hold off ; image(uint8(ImageDroite)) ; hold on ; axis image ;
title('Image droite') ; if(Nplan<3) colormap(gray(256)) ; end ; drawnow ;

[Nlin, Ncol, Nplan] = size(ImageGauche) ;

% Nombre de points que l'on va sélectionner sur les images
NombreDePoints = 2 ;
PointDroite = ones(NombreDePoints,3);
PointGauche = ones(NombreDePoints,3);

% x == colonnes // y == lignes
for n=1:NombreDePoints
    figure(1) ; drawnow ;
    [x,y] = ginput(1) ;
    plot(x,y,'bo') ;
    PointGauche(n,1:2) = [x,y] ;
    PointDroite(n,1:2) = [x,y] ;
end

% Selection de points aleatoires
%indice = floor(rand(1,p)*n) + 1 ;

saisie=0;
%chargement des points apparies entre les deux images
XG=PointGauche(:,1);
YG=PointGauche(:,2);
XD=PointDroite(:,1);
YD=PointDroite(:,2);
methode=2
if methode==1
%methode 1 - methode des moindres carres avec contrainte unitaire 
%sur une des coordonnees de f (question 4.1)
%calculer A B et f
    A = ones(NombreDePoints,8);
    B = ones(NombreDePoints,1);
    for n=1:NombreDePoints
        A(n,1:8) = [XG(n)*XD(n) , XG(n)*YD(n) , XG(n) , YG(n)*XD(n) , YG(n)*YD(n) , YG(n) , XD(n) , YD(n)] ;
        B(n,1) = 1 ;
    end
    f = -pinv(A)*B ;
    
end;
if methode==2
%methode 2 - methode des moindres carres avec contrainte unitaire 
%sur la norme de f (question 4.2)
%calculer C D V et f
    C = ones(NombreDePoints,9);
    for n=1:NombreDePoints
        C(n,1:9) = [XG(n)*XD(n) , XG(n)*YD(n) , XG(n) , YG(n)*XD(n) , YG(n)*YD(n) , YG(n) , XD(n) , YD(n), 1] ;
    end
    D = transpose(C)*C;
    
    % Decomposition en matrice singuliere
    [U,S,V] = svd(D) ;
    f = V(:,9);
end;
F=[f(1:3)';f(4:6)';f(7:8)' 1];

x_min=0;
x_max=size(ImageGauche,2);
y_min=0;
y_max=size(ImageGauche,1);

%calculer ld et modifier x_min x_max y_min et y_max
LD = F*transpose(PointGauche) ;
if((theta <= pi/2) && (theta >= -pi/2)) || ((theta >= 3*pi/2) && (theta <= -3*pi/2))
    y_min = (-LD(3) - (LD(1)*x_min))/LD(2) ;
    y_max = (-LD(3) - (LD(1)*x_max))/LD(2) ;
else
    x_min = (-LD(3) - (LD(2)*y_min))/LD(1) ;
    x_max = (-LD(3) - (LD(2)*y_max))/LD(1) ;
end

% affichage de la droite epipolaire
figure(2) ;
line( [x_min,x_max] , [y_min,y_max] , 'color', 'r' ) ;
drawnow ;

