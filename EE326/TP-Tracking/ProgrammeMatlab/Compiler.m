reponse = which('delete DeriveImage.mexmac') ;
if(strcmp(reponse,'')==0) delete DeriveImage.mexmac ; end ;
% mex -v -argcheck DeriveOmni.c % InterNoyau.c Interaction.c Choquet.c Tri.c
mex -v -argcheck DeriveImage.c Deriche.c Castan.c % Choquet.c 
