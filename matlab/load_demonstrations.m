clear all; close all; clc;


% %%%%%%%%%%%%%%%%% Load and smooth demonstrated trajectories %%%%%%%%%%%%%%%

data_path = '/home/rkg/ros/aass_teleop/demonstration_logger/data/';

joint_names{1}='THJ1';
joint_names{2}='THJ2';
joint_names{3}='THJ3';
joint_names{4}='THJ4';
joint_names{5}='THJ5';
joint_names{6}='FFJ0';
joint_names{7}='FFJ3';
joint_names{8}='FFJ4';
joint_names{9}='MFJ0';
joint_names{10}='MFJ3';
joint_names{11}='MFJ4';
joint_names{12}='RFJ0';
joint_names{13}='RFJ3';
joint_names{14}='RFJ4';
joint_names{15}='LFJ0';
joint_names{16}='LFJ3';
joint_names{17}='LFJ4';
joint_names{18}='LFJ5';


Td = 1/100; %Trajectories are resampled with Td
smooth_window=30;
v_thresh=1;
alpha_T=1.1;

file= strcat(data_path,'tripod_small_closed.txt');
trajectories=processData(joint_names,file,Td,smooth_window,0);
trajectories=truncateTrajectories(trajectories,v_thresh,alpha_T,0);
trajectories=shiftToOrigin(trajectories);
write_ACADO_measurements(trajectories.t,trajectories.joints([7]),'demonstrations/FFJ3_tripod_small_closed.txt');

file= strcat(data_path,'tripod_small_open.txt');
trajectories=processData(joint_names,file,Td,smooth_window,0);
trajectories=truncateTrajectories(trajectories,v_thresh,alpha_T,0);
trajectories=shiftToOrigin(trajectories);
write_ACADO_measurements(trajectories.t,trajectories.joints([7]),'demonstrations/FFJ3_tripod_small_open.txt');



