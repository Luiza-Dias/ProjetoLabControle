clc
clear all
pkg load control

G = tf([3497489.7],[1 10934.02 115952.84])
t = (0:0.01:7);
r = ones(1,701)*3.1;
Y = lsim(G,r,t);
plot(t,Y, 'r-')
hold on
vel = ones(1,701)*83.78;
plot(t,vel, 'k-')
