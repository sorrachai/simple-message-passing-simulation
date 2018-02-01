function dydt = dde(t,T,Z )
%DDE Summary of this function goes here
%   Detailed explanation goes here
global delta;
global alpha;
 dydt = (1-T)*(1-exp(-alpha*Z));

end

