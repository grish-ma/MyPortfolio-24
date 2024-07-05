%% Lab_9
%% 4.1
figure(1)
plot(daily_temp)
title("Daily Temperatures from Jan 1st, 2000 - Dec 31st, 2016")
saveas(gcf, "plot_AllTemps")

%% 4.2
% Using the temperature data from 2000-2015, and setting B = 7
[A, b] = matrix_equation(daily_temp(1:5844, 1), 7);
Beta = inv(A'*A)*(A'*b)

%% 4.3
figure(2)
plot(b)  % True Temp
hold on
b_hat = A * Beta;  % Predicted Temp
plot(b_hat)
legend('b', 'b_hat');
title("True Against Predicted Temperature: 2000-2015")
saveas(gcf, "True-Predicted")

%% 4.4
% Compute the error e = norm(diff) / N using the 
% norm() function (which computes |v|^2 for a vector v. 
% Here N is the length of b
N = numel(b);
diff = b_hat-b;
error_e = norm(diff) / N

%% 5.1
[C, d] = matrix_equation(daily_temp(5845:6210, 1), 7);
% I have not recalculated Beta
d_hat = C * Beta;  % PREDICTED

%% 5.2
figure(3)
plot(d)  % TRUE Temp
hold on
plot(d_hat)
legend('d', 'd_hat');
title("True Against Predicted Temperature: 2016")
saveas(gcf, "True-Predicted-2")

%% 5.3
% f = (|d_hat−d|^2) / M
M = numel(d);
diff = d_hat-d;
error_f = norm(diff) / M
close all;
%% 6.1
for B = 1:10
    [newA, newB] = matrix_equation(daily_temp(1:5844, 1), B);
    Beta_hat = inv(newA'*newA)*(newA'*newB);
    disp("Beta_hat " + B + " = " + Beta_hat);
    
    [newC, newD] = matrix_equation(daily_temp(5845:6210, 1), B);

    %% 6.2 
    % for each Beta_hat, compute newB_hat = newA*Beta_hat and 
    % newD_hat = newC*Beta_hat.
    newB_hat = newA*Beta_hat;
    newD_hat = newC*Beta_hat;

    % Compute ei and fi
    % ei = |newB_hat−b|^2 / N
    N = numel(newB);
    diff = newB_hat-newB;
    e(B) = norm(diff) / N;

    % f = (|d_hat−d|^2) / M
    M = numel(newD);
    diff = newD_hat-newD;
    f(B) = norm(diff) / M;
end

%% 6.3
figure(4)
plot(e)
hold on
plot(f)
legend('ei', 'fi');
title("Error Rates: Predicted vs True as B increases")
saveas(gcf, "True-Predicted-3")
