clear all;
close all;

% PID Constants:
Kp = 5;
Ki = 2.5;
Kd = 2;
N = 1;
SP = 5000;

% Process constants:
Kproc = 1;
Tau = 0.5;
Kpert = 0.1;
Pert = 15000;

sim('PID_model');
yp = ans.yp;
t = ans.tout;
Perturb = ans.Perturb;
e = ans.e;
u = ans.u;
sp = ans.sp;
figure;

subplot(4,1,1);
plot(e);
xlabel('Time');
ylabel('Error signal e(t)');
title('e(t)');
grid on;

subplot(4,1,2);
plot(u);
xlabel('Time');
ylabel('Controller output u(t)');
title('u(t)');
grid on;

subplot(4,1,3);
plot(Perturb);
xlabel('Time');
ylabel('Pertubation Perturb(t)');
title('Perturb(t)');
grid on;

subplot(4,1,4);
plot(yp);
xlabel('Time');
ylabel('Process out yp');
title('yp(t)');
hold on;
plot(sp);
grid on;