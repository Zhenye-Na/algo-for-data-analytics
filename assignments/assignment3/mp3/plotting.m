x = linspace(1, 400, 400);
r = importdata('r.txt');
b = importdata('b.txt');

figure
plot(x, r)
hold on
plot(x, b)