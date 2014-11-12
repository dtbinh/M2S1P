function [ XKal, YKal, CovKalman ] = EstimateFromExtKalman ( InitState, X_m, Y_m, V, t, initCov, sigmaAcc)
%state kalman is [x y dot{x} dot{y}]
%initCov must be a 4 element column vector
%InitState must be a 4 element column vector
%initial state and covariance
StateKalman(1:4,1) = InitState; %try [X_m(1); Y_m(1); DX(1); DY(1)];
CovKalman(1:4, 1:4, 1) = diag(initCov); %attention doit pas etre nul
%carachteristic matrices
deltaT=t(2)-t(1)
F = ;
Q = ;
R = ;
for I = 2 : (size(t, 2) - 1)
    %prediction 
    %celle de l etat est identique a la premiere question du TP lorsque on
    %ne compte pas l'acceleration
    StateKalman(1:4, I) = F * StateKalman(1:4, I-1);
    CovKalman(1:4, 1:4, I) = F * CovKalman(1:4, 1:4, I-1) * F' + Q;
    %mesure
    Y(1:3, I) = ;
    %mise a jour
    V(I) = ;
    H = ;
    S(1:3, 1:3) = ;
    K(1:4, 1:3) = ;%attention: nul si CovKalman=0 
    StateKalman(1:4, I) = ;
    CovKalman(1:4, 1:4, I) = ;
    XKal(I) = StateKalman(1, I);
    YKal(I) = StateKalman(2, I);
end

end

