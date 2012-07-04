%
% verify data
%

clear;clc

d_load = load('FFJ3_tripod_small_closed.txt');

n = size(d_load,1);

ind_start = 1; % remove some data
ind_step = 10;

% no need to use all data points
ind = ind_start:ind_step:n;
d = d_load(ind,:);

hold on;
plot(d(:,1),d(:,2),'b.') % q
plot(d(:,1),d(:,3),'r.') % dq
%plot(d(:,1),d(:,4),'g') % ddq

% sanity check
if 0
    dt = diff(d_load(:,1)); dt = dt(1);
    plot(d_load(1:end-1,1),diff(d_load(:,2))/dt,'ko')
    plot(d_load(1:end-1,1),diff(d_load(:,3))/dt,'ko')
end

grid on
save('data.txt', 'd', '-ASCII')

% ------------------------------------------------------------
% plot result from simulation (with "the optimal" parameters)
% ------------------------------------------------------------

s_opt = load('states_opt.txt');

plot(s_opt(:,1),s_opt(:,2),'co')
plot(s_opt(:,1),s_opt(:,3),'mo')

% ---------------------------------

s_sim = load('states_sim.txt');

plot(s_sim(:,1),s_sim(:,2),'c')
plot(s_sim(:,1),s_sim(:,3),'m')

% ---------------------------------

s_sim1 = load('states_sim1.txt');

plot(s_sim1(:,1),s_sim1(:,2),'k--')
plot(s_sim1(:,1),s_sim1(:,3),'k--')

%%%EOF