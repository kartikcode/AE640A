clc
clear all;
close all;
 
% parameters
dim = 2;
iters= 500;
% R_init and T_init are the initial guess parameters
R_init = eye(2); 
T_init = zeros(2,1); 
% Maximum distance treshold to eliminate the outlier correspondances. This has to be tuned. 
max_tresh = 10 ; 
% loading raw scans
lidar_scans = load('lidar_scans.mat');

scan1 = lidar_scans.scan1;
scan2 = lidar_scans.scan2;

% Remove zero columns in the scans (if any)
scan1( :, ~any(scan1,1) ) = [];
scan2( :, ~any(scan2,1) ) = [];

%% Perform  ICP 

[R,T] =  ICP(scan1,scan2,iters,R_init,T_init,max_tresh); % ---------------> TO DO
 
% Note: find R,T such that they register scan2 onto scan1. This is just to maintain uniformity.  
 
%% Visualizing the output
scan2_transformed = R*scan2+repmat(T,1,size(scan2,2));

figure 
scatter(scan2(1,:),scan2(2,:),'r','.');
hold on
scatter(scan1(1,:),scan1(2,:),'b','.');
axis equal
title('Before ICP registration')
legend('scan2','scan1');
 
figure
scatter(scan2_transformed(1,:),scan2_transformed(2,:),'r','.');
hold on
scatter(scan1(1,:),scan1(2,:),'b','.');
axis equal
title('After ICP registration')
legend('scan2new','scan1');