global delta;
global n;
global alpha;

n=100;
%%% we use delta - 1 in our analytical solution in stead of delta, possibly to take care of off-by-one-error
%%% note that we assume epsilon is infinity
delta=19;
alpha=0.05;
tmax = 400;

lags= delta;
tspan = [delta+1 tmax];

sol = dde23(@dde,lags,@history,tspan);
tplot = linspace(delta+1,tmax,100);
T1 = deval(sol,tplot);
T1 = [repmat(1/n,1,delta) T1];
tplot = [1:(delta) tplot];
h2 =plot(tplot,T1,'red');

xlabel('epsilon')     %Label the horizontal axis
ylabel('y')         %Label the vertical axis
string_title = strcat('n=',num2str(n));
string_title= strcat(string_title, strcat(',delta=',num2str(delta+1)));
string_title= strcat(string_title, strcat(',alpha=',num2str(alpha)));
title(string_title)
legend('Numerical Solution');


