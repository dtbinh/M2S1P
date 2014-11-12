function [ XfromSpeed, YfromSpeed ] = EstimateFromSpeeds ( X_m, Y_m, DX, DY, t, sigma)
%UNTITLED3 Summary of this function goes here
%   Detailed explanation goes here
ecartType=sqrt(sigma)
size(X_m, 2)
XfromSpeed(1) = X_m(1);
YfromSpeed(1) = Y_m(1);
for I = 1 : (size(t, 2) - 1)
    deltaT=t(I+1)-t(I);
    ax = 0 + ecartType*randn(1);
    XfromSpeed(I+1) = XfromSpeed(I) + deltaT*DX(I) + ax * (deltaT*deltaT) /2;
    ay = 0 + ecartType*randn(1);
    YfromSpeed(I+1) = YfromSpeed(I) + deltaT*DY(I) + ay * (deltaT*deltaT) /2;
end

end

