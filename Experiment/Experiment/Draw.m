function [ H ] = Draw( EB )
%UNTITLED 此处显示有关此函数的摘要
%   此处显示详细说明
L = length(EB);
A = EB(1:L,1);
B = EB(1:L,2);
C = EB(1:L,3);
D = EB(1:L,4);
BinWidth = 0.04;

NE = B ./ (B + C - D);
H = histogram(NE);
H.Normalization = 'probability';
H.BinWidth = BinWidth;
axis([0 0.98 0 0.3])
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
plot(PSO,PEP * 0.3)

end

