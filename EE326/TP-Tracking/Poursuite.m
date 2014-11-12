clear all ; % on efface toutes les variables deja crees

Suffixe = '.tif' ; % suffixe des images
% Nom comprenant l'endroit ou se trouvent les images et le nom de base des images
NomDeBase = '/Users/cherubini/Documents/lavoro/enseignement/M2/TP/TP326/TP-Tracking/SequenceAvecVariation/Image' ; 
NumeroImage = 1 ; % numero de l'image courante

% pour creer le nom d'une image il suffit de faire :
NomImage=strcat(NomDeBase,sprintf('00%d%s',NumeroImage,Suffixe)) ,

% pour voir si l'image existe il suffit de faire :
NumeroDeFichier=fopen(NomImage) ;
if(NumeroDeFichier<=0)
	oui=0 ; return ; % la premiere image n?existe pas alors ce n'est pas la peine de continuer
end

fclose(NumeroDeFichier) ; % on ferme le fichier
ImageCourante = imread(NomImage) ;	 % et on l?ouvre a nouveau en tant qu?image

% pour obtenir la taille de l?image :
[Nlin, Ncol] = size(ImageCourante) ;

figure(1) ; title('Selectionnez un motif a poursuivre') ;
% pour selectionner un motif il faut utiliser la commande imcrop
[ Motif, Rectangle ] = imcrop(ImageCourante) ; 
figure(2) ; imshow(Motif) ; title('Motif a poursuivre') ; % Motif c'est l'imagette du motif a poursuivre

% Rectangle c'est la zone dans laquelle se trouve le motif dans l'image
% Rectangle(1) est la coordonnee du coin en haut ? gauche en colonne, Rectangle(2) est sa coordonnee en ligne,
% Rectangle(3) est la taille du rectangle en colonne, Rectangle(4) est la taille du rectangle en ligne, 
Rectangle(1:4) = round(Rectangle(1:4)) ; % on transforme le rectangle de facon a ce que ses coordonnees soient entieres

% Si le rectangle a une dimension inferieure a 3 pixel, c'est qu'on a fait
% une erreur de manipulation sur le clic de la zone

if min(Rectangle(3:4) < 3 ) return ; end ;

% On va creer maintenant une liste de points correspondant au contour ferme du rectangle.
x(1) = Rectangle(1) ;
x(2) = Rectangle(1) ;
x(3) = Rectangle(1) + Rectangle(3) ;
x(4) = Rectangle(1) + Rectangle(3) ;
x(5) = x(1) ;
delta_col = Rectangle(3);

y(1) = Rectangle(2) ;
y(2) = Rectangle(2) + Rectangle(4) ;
y(3) = Rectangle(2) + Rectangle(4) ;
y(4) = Rectangle(2) ;
y(5) = y(1) ;
delta_lin = Rectangle(4);

% On dessine le rectangle sur l'image (en bleu)
figure(1) ; 
hold off ;
imshow(uint8(ImageCourante)) ; 
hold on ;
title('Image Courante') ;
line(x,y) ; 
drawnow ;
Motif = double(Motif) ; % il faut convertir le format du motif pour pouvoir faire des calculs en virgule flottante (nombres reels)

[nlin, ncol] = size(Motif);% nlin, ncol c'est la taille du motif

oui = 1 ; % tant que oui vaudra 1 on continuera de lire les images

while(oui)
    %oui = 0
    % on fabrique le nom de l'image courante
	if(NumeroImage<10)
		NomImage=strcat(NomDeBase,sprintf('00%d%s',NumeroImage,Suffixe)) ;
    else %on s arrete apres dix images; sinon, retirer le commentaire plus bas
        oui = 0;
% 		if(NumeroImage<100)
% 			NomImage=strcat(NomDeBase,sprintf('0%d%s',NumeroImage,Suffixe)) ;
% 		else		
% 			NomImage=strcat(NomDeBase,sprintf('%d%s',NumeroImage,Suffixe)) ;
% 		end
	end	
 
    % on regarde si l'image existe
    NumeroDeFichier=fopen(NomImage) ;
	if(NumeroDeFichier<=0)
		oui=0 ;
	else % si elle existe
		fclose(NumeroDeFichier) ;
        % on la lit et on la transforme en image "reelle"
        ImageCourante = double(imread(NomImage)) ; 	
        %tableau des SADs pour chaque paire de candidats u, v (il faut
        %chercher le minimum de ce tableau
        SAD = zeros(Nlin-nlin,Ncol-ncol) ;
        for lin=1:Nlin-nlin
            for col=1:Ncol-ncol
                Imagette = ImageCourante(lin:lin+delta_lin-1, col:col+delta_col-1) ;
                
                % calcul de la difference entre l'imagette courante et le
                % motif de reference.
                Difference = Motif - Imagette ;
                % calcul de la somme absolue des differences (SAD(lin, col))
                SAD(lin,col) = sum(sum(abs(Difference))) ;
                
                % si vous voulez voir le motif courant, vous pouvez retirer
                % le commentaire devant la ligne ci-dessous (mais le
                % calcul va etre tres long.
                %figure(3) ; imshow(uint8(Imagette)) ; title('Fenetre a comparer') ; drawnow ; 
            end
        end
        % on cherche la valeur de lin et col qui minimise la distance SAD
        %c'est a dire la valeur minimale dans le tableau
        [lin_opt,col_opt] = find(SAD<=min(min(SAD))) ; 
        lin_opt;
        col_opt;
        % la variation de position en x et y est alors donnee par :
        deltax = ( ( max(col_opt) + min(col_opt) ) / 2 ) - x(1) ;    
        deltay = ( ( max(lin_opt) + min(lin_opt) ) / 2 ) - y(1) ;
        % car il se peut qu'il y ait plusieurs maxima auquel cas on prend
        % la moyenne des maxima
        % la nouvelle position des points du rectange est alors (translation appliquee au vecteurs x et y) :
        x_courant = round( x + deltax ) ;
        y_courant = round( y + deltay ) ;
        % On dessine le rectangle sur l'image (en bleu)
        figure(1) ; 
        hold off ;
        imshow(uint8(ImageCourante)) ; 
        hold on ;
        line(x_courant,y_courant) ; 
        title('Image Courante') ;
		drawnow ; % pour s'assurer que le dessin se fait immediatement
	end
  	NumeroImage = NumeroImage + 1 ;	% on incremente le numero de l'image a lire
end
