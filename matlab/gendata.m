function xi = gendata(N)
%
% N - Number of points 
%

%Randomly generate parameter vector
xi=randn(6,1); % xi(1:3) - angulary velocity
               % xi(4:5) - linear velocity

xi_hat = [ tilde(xi(1:3)), xi(4:6); 
               zeros(1,3),      0];
           
%Randomly generate N points (in 3D)
p = [rand(3,N);ones(1,N)];

%Apply rigid-body transformation (assume delta t = 1.0)
q = expm(xi_hat)*p;

%Output data
% -------------------------------
fw=fopen('measurements.txt','w');

fprintf(fw,'%f ',0 );
fprintf(fw,'%f ',p(1:3,:));

fprintf(fw,'\n%f ',1 );
fprintf(fw,'%f ',q(1:3,:));

fclose(fw);
% -------------------------------
fw=fopen('xi.txt','w');
fprintf(fw,'%f ',0 );
fprintf(fw,'%f ',xi);
fclose(fw);
% -------------------------------

%%%EOF
