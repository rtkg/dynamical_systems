clear;clc

% -----------------
N = 30;
InitialGuess = 0;
OPTION = 1;
KKT_tol = 1e-6;
% -----------------

xi = gendata(N);

IG = [0 0 0 0 0 0];
%IG = xi; % perfect initial guess

if 1
  fid = fopen('InitialGuess.txt', 'w');
  fprintf(fid, '%e ', 0);
  fprintf(fid, '%e ', IG);
  fclose(fid);
end

path = '../bin/';
executable = 'example';
arguments = [num2str(N)           , ' ', ...
             num2str(InitialGuess), ' ', ...
             num2str(OPTION)      ,' ', ...
             num2str(KKT_tol)];

disp(' ');
disp([' Execute command:  ', executable, ' ', arguments])
disp(' ');

out = system([path, executable, ' ', arguments]);
