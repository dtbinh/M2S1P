% Saisie des donn?es 2D et 3D ? partir des vues des 2 cam?ars et de l'objet 3D de calibrage connu
%
% Made in TRIBOULET 2008
%
clear;clc;close all;

    % Nombre de points de calibrage sur chaque prise de vue de l objet 3D
    npts_calib=12;
    
%     Observer l ordre des points propose dans le TP
 
    xyz=[   0,5,5;
            0,1,5;
            0,1,1;
            0,5,1;
            1,0,5;
            5,0,5;
            5,0,1;
            1,0,1;
            1,1,6;
            1,5,6;
            5,5,6;
            5,1,6]*1e-2;
         
    % Chargement de la premi?re image
    [fic_ig,path_fic_ig]=uigetfile('*.bmp', 'Image cam?ra Gauche');
    ig=imread([path_fic_ig fic_ig]);
    % saisie des points
    figure(1);
    imshow(ig);
    for i=1:npts_calib
    [xg(i),yg(i)]=ginput(1);
    xg(i)=round(xg(i));
    yg(i)=round(yg(i));
    end;
    % resolution subpixel pour les coins debut
    [xc_g] = cornerfinder([xg;yg],ig);
    
    ug=xc_g(1,:)';
    vg=xc_g(2,:)';
    
    % donnees 3D de la mire de calibration et visualisation camera gauche
    uv_xyz_g=[xg' yg' xyz];
    fp=fopen('uv_xyz_g.dat','w');
    fprintf(fp,'%e\t%e\t%e\t%e\t%e\n',uv_xyz_g');
    fclose(fp);
    uv_spix_xyz_g=[ug vg xyz];
    fp=fopen('uv_spix_xyz_g.dat','w');
    fprintf(fp,'%e\t%e\t%e\t%e\t%e\n',uv_spix_xyz_g');
    fclose(fp);

    
    hold on
    % sur impression des points choisis sur l image
    plot(ug,vg,'+r');
    pause;
    close all;    
    
    
    % Chargement de la seconde image
    % choix image depart
    [fic_id,path_fic_id]=uigetfile('*.bmp', 'Image cam?ra Droite');
    id=imread([path_fic_id fic_id]);
    % saisie des points
    figure(2);
    imshow(id);
    for i=1:npts_calib
    [xd(i),yd(i)]=ginput(1);
    xd(i)=round(xd(i));
    yd(i)=round(yd(i));
    end;
    % resolution subpixel pour les coins debut
    [xc_d] = cornerfinder([xd;yd],id);
    
    ud=xc_d(1,:)';
    vd=xc_d(2,:)';
    
    % donnees 3D de la mire de calibration et visualisation camera droite
    uv_xyz_d=[xd' yd' xyz];
    fp=fopen('uv_xyz_d.dat','w');
    fprintf(fp,'%e\t%e\t%e\t%e\t%e\n',uv_xyz_d');
    fclose(fp);
    uv_spix_xyz_d=[ud vd xyz];
    fp=fopen('uv_spix_xyz_d.dat','w');
    fprintf(fp,'%e\t%e\t%e\t%e\t%e\n',uv_spix_xyz_d');
    fclose(fp);

    
    hold on
    plot(ud,vd,'+r');