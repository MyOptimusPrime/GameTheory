clc
clear

x=1:10;
y=cos(x);
plot(x,y)
h=gca;
rot=45;
a=get(h,'XTickLabel');
set(h,'XTickLabel',[]);
b=get(h,'XTick');
c=get(h,'YTick');
text(b,repmat(c(1)-.1*(c(2)-c(1)),length(b),1),a,'HorizontalAlignment','right','rotation',rot);