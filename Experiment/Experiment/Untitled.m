clc
clear

load('EB.txt');
L = length(EB);
A = EB(1:L,1);
B = EB(1:L,2);
C = EB(1:L,3);
D = EB(1:L,4);

NE = B ./ (B + C - D);

k = 1;
for PSO = 0.4:0.035:0.98
   EBP =  PSO * D + (1-PSO) * B;
   EBNP = PSO * C + (1-PSO) * A;
   PEP(k) = sum((sign(EBP - EBNP) + 1)/2)/L;
   k = k+1;
end
PSO = 0.99;
EBP =  PSO * D + (1-PSO) * B;
EBNP = PSO * C + (1-PSO) * A;
PEP(k) = sum((sign(EBP - EBNP) + 1)/2)/L;
PSO = [0.4:0.035:0.98,0.99];
plot(PSO,PEP)
axis([0 0.99 0 1])
hold on

h1 = histogram(NE);
h1.Normalization = 'probability';
h1.BinWidth = 0.04;


load('EB(punishfactor1).txt');
L = length(EB);
A = EB_punishfactor1_(1:L,1);
B = EB_punishfactor1_(1:L,2);
C = EB_punishfactor1_(1:L,3);
D = EB_punishfactor1_(1:L,4);

NE = B ./ (B + C - D);

hold on
h2 = histogram(NE);
h2.Normalization = 'probability';
h2.BinWidth = 0.04;

hold on
k = 1;
for PSO = 0.4:0.035:0.98
   EBP =  PSO * D + (1-PSO) * B;
   EBNP = PSO * C + (1-PSO) * A;
   PEP(k) = sum((sign(EBP - EBNP) + 1)/2)/L;
   k = k+1;
end
PSO = 0.99;
EBP =  PSO * D + (1-PSO) * B;
EBNP = PSO * C + (1-PSO) * A;
PEP(k) = sum((sign(EBP - EBNP) + 1)/2)/L;
PSO = [0.4:0.035:0.98,0.99];
plot(PSO,PEP)
