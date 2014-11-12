clear all
% Nom de l?image que l?on va charger
 NomGenerique = 'ImageParabolique' ;
% et son suffixe
Suffixe = '.tif' ;
% Composition des noms des images a charger et chargement de ces images
Nom = sprintf('%s%s',NomGenerique,Suffixe) ;
Image = double(imread(Nom)) ;
[Nlin, Ncol, Nplan] = size(Image) 
figure(1) ; hold off ; image(uint8(Image)) ; hold on ; axis image ;
if(Nplan<3) colormap(gray(256)) ; end ; drawnow ;
for W = 1 : Ncol
    for H = 1 : Nlin
        Image2(H,W) = Image(H, W);
    end
end
figure(2) ; hold off ; image(uint8(Image2)) ; hold on ; axis image ;
if(Nplan<3) colormap(gray(256)) ; end ; drawnow ;

