% Modelisation d un systeme stereo
% Chaque camera est reperee par un numero
% Un modele de stenoppe classique eest utilise
% Une mire unique en 3D permettra d avoir le jeux de points de calibration
%
% Made in TRIBOULET 2008
%
clear;clc;close all;

    % donnees 3D de la mire de calibration et visualisation camera gauche
    fichier=['uv_xyz_g'];
    
    fich=[fichier '.dat'];
    eval(['load ' fich]);
    m=eval(fichier);
	n=max(size(m));
    
    
	fp=fopen('calib_stereo.dat','w');
   	fprintf(fp,'Calibration paire stereovision et reconstruction 3D:\n');

	fprintf(fp,'Fichier de mesures :\t\t%s\n',fich);
	fprintf(fp,'Nombre de points:\t\t%d\n\n',n);

    % Affectation des donnees du fichier a un choix de varaiables

    xo=m(:,3);yo=m(:,4);zo=m(:,5);u=m(:,1);v=m(:,2);
    
    % construction des differentes matrices
    
    % A_=[xo,yo,zo,.....
	% B=[u;v];
    
	% La fameuse pseudo inverse
    C=pinv(A)*B;
    
    % le modele global
	Glob=[C(1:4)';
				C(5:8)';
				C(9:11)',1];

	% formatage du modele de la camera et stockage des donnees
    fprintf(fp,'Modele camera gauche:\n');
	 for i=1:3
		for j=1:4
	 fprintf(fp,'%e\t',Glob1(i,j));
		end;
	 fprintf(fp,'\n');
	 end;


	fprintf(fp,'Fichier de reconstruction :\t\t%s\n',fich);
	fprintf(fp,'Nombre de points:\t\t%d\n\n',n);

    % Donnees pixel reconstruites
	B_cam_rec=Glob*[xo';yo';zo';ones(1,n)];
	u_rec=B_cam_rec(1,:)./B_cam_rec(3,:);
	v_rec=B_cam_rec(2,:)./B_cam_rec(3,:);
    
    % Erreur de reconstruction
    
	Err_cam=[u'-u_rec;v'-v_rec];

% Max Min Moyenne Ecart type

Max_Err_cam=max(Err_cam(:,1:(n)))';
Min_Err_cam=min(Err_cam(:,1:(n)))';
Moy_Err_cam=mean(Err_cam(:,1:(n)))';
Std_Err_cam=std(Err_cam(:,1:(n)))';


fprintf(fp,'Reconstruction caméra gauche en pixels:\n');
fprintf(fp,'Max_u\tMax_v\n');
fprintf(fp,'%e\t%e\n',Max_Err_cam(1),Max_Err_cam(2));
fprintf(fp,'Min_u\tMin_v\n');
fprintf(fp,'%e\t%e\n',Min_Err_cam(1),Min_Err_cam(2));
fprintf(fp,'Moy_u\tMoy_v\n');
fprintf(fp,'%e\t%e\n',Moy_Err_cam(1),Moy_Err_cam(2));
fprintf(fp,'Ect_u\tEct_v\n');
fprintf(fp,'%e\t%e\n',Std_Err_cam(1),Std_Err_cam(2));
fprintf(fp,'\n');
fclose(fp);

% Rependre la meme demarche pour les donnees 3D de la mire de calibration et visualisation camera droite


% Reconstruction 3D a partir des modeles precedents et des equations
% fournies dans le support de TP

     
        
 % Rayons visuels des deux cameras


% Points 3D reconstruits

% Statistiques de reconstruction 3D.......