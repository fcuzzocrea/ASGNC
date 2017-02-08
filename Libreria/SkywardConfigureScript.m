% This code was developed for the Aerospace System Guidance & Control Course, held in Politecnico di Milano in collaboration with The Mathworks (R).
% Author: Francescodario Cuzzocrea
% Email: francescodario.cuzzocrea@skywarder.eu
% October 2014
% Version 0.5 - codename Junia
%
% I would like to thanks the following people :
%
% Umberto "Umbie" Saetti, for believing in me
% Flavio Baduini, for supporting me
% Mattia "Pela" Giurato, for helping me with the sensors
% Edoardo "Trevis" Codispoti, for helping me during the classroom
% Aureliano Rivolta, for helping me to better understand the simulations
% All the guys of the course - Thank You

%% SKYWARD CONFIGURE SCRIPT

% Just run the script and it will install the Simulink library for using the PelaBoard

ProjDir = pwd;
disp('Adding Current Path to MATLAB path three...');
addpath(ProjDir,[ProjDir,'\Ultrasonic'],[ProjDir,'\Magnetometer'],[ProjDir,'\IMU'],[ProjDir,'\Barometer'],[ProjDir,'\Printf']);
addpath('C:\MATLAB\SupportPackages\R2014a\arduino-1.0.5\libraries');
savepath;
disp('Building Skyward BLIMP blocks');

cd ([ProjDir,'\Printf']);
        mex 'printf.c' 'printf_wrapper.cpp'

cd ([ProjDir,'\Ultrasonic']);
        mex 'HC_SR04.c' 'HC_SR04_wrapper.cpp'
        
cd ([ProjDir,'\Magnetometer']);
        mex 'HMC5883L.c' 'HMC5883L_wrapper.cpp' 
        changeFile('HMC5883L_wrapper.cpp',ProjDir);

cd ([ProjDir,'\IMU']);
        mex 'MPU6050.c' 'MPU6050_wrapper.cpp'
        changeFile('MPU6050_wrapper.cpp',ProjDir);

cd ([ProjDir,'\Barometer']);
        mex 'BMP_085.c' 'BMP_085_wrapper.cpp'
        changeFile('BMP_085_wrapper.cpp',ProjDir);
        
% Complete Compilation
cd (ProjDir);
disp('Adding Skyward BLIMP Target to Simulink...');
libBrow = LibraryBrowser.StandaloneBrowser;
libBrow.refreshLibraryBrowser;

disp('Setup Complete ! Happy Hacking :D');
clear all
