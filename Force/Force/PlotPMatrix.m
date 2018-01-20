clc
clear

load brute.txt;
P = brute(:,2);
% load score.txt;
% P = score(:,2);
Pmatrix = reshape(P,[32,32]);
Pmatrix = (sign(Pmatrix)+1).*Pmatrix./2;

% pcolor(Pmatrix)

figure
set(gcf,'unit','normalized','Position',[0.2,0.2,0.4,0.64])
h = bar3(Pmatrix);
for k=1:numel(h)
    zdata = h(k).ZData;
    h(k).CData = zdata;
    h(k).FaceColor = 'interp';
end

load Mymap;
colormap(Mymap);
axis([0.2 32.4 0.6 32.8 0 0.5])

tick = 1 : 32;
label = {'00000','00001','00010','00011','00100','00101','00110','00111',...
    '01000','01001','01010','01011','01100','01101','01110','01111',...
    '10000','10001','10010','10011','10100','10101','10110','10111',...
    '11000','11001','11010','11011','11100','11101','11110','11111'};
set(gca,'XTick',tick,...
    'YTick',tick,...
    'YTickLabel',label,...
    'XTickLabel',[],...
    'XAxisLocation','bottom',...
    'FontName','Times New Roman',...
    'FontSize',12,...
    'position',[0.15,0.15,0.8,0.8] )

h=gca;
rot=60;
b=get(h,'XTick');
% c=get(h,'YTick');
text(b,repmat(33.4,length(b),1),label,...
    'HorizontalAlignment','right','rotation',rot,...
    'FontName','Times New Roman',...
    'FontSize',12);

xlabel('Emp2 Strategy','FontSize',20,'position',[16,35.5,0])
ylabel('Emp1 Strategy','FontSize',20)
colorbar('FontSize',20)
view(0,90)