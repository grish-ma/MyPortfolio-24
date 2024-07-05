%% Lab 7

% 7.1
load("data.mat")
% Each row of data.mat corresponds to a signal
figure(1);
for c = 1:3
    plot(X(c, :));
    hold on;
end

title("(H1) Plot of X's Original Three Waveforms");
saveas (gcf, "Tx_waveforms.jpg")

% 7.1.1
%% Noiseless Channel
% Y contains 3 received wavforms
% H is the transformation from the X sent signals to Y
Y = H1*X;
figure(2);
for c = 1:3
    plot(Y(c, :));
    hold on;
end
title("(H1) Plot of Coded Y Waveforms: Noiseless");
saveas (gcf, "Ty_waveforms.jpg")

%% Decoding the Noiseless Channel
% Using the inverse of the transformation matrix H, we are going back from
% Y to X.
X2 = inv(H1)*Y;
figure(3);
for c = 1:3
    plot(X2(c, :));
    hold on;
end
title("(H1) Plot of Decoded X Waveforms: Noiseless");
saveas (gcf, "Tx2_waveforms.jpg")
%% Q5 ^
disp("Problem 7.2.2 - decoded signal in the absence of noise: ");
incorrect = 0;
bits = 0;
for r = 1:3 % loop through each row
    binSig = binary_decoder(X2(r, :));
    bS = binary_decoder(X(r, :));
    for c = 1:100 %loop through each column
        if (binSig(1, c) ~= bS(1, c))
            incorrect = incorrect + 1;
        end
        bits = bits + 1;
    end
end
disp("BER = " + incorrect/bits*100);

%% Noisy Channel
var = 1e-2;
N = sqrt(var)*randn(3, 1000);
Y = H1*X+ N;
figure(4);
for c = 1:3
    plot(Y(c, :));
    hold on;
end
title("(H1) Plot of Coded Y Waveforms: Added Noise");
saveas (gcf, "T_YH1N_waveforms.jpg")

%% Decoding a Noisy Channel
X2 = X + inv(H1)*N;
figure(5);
for c = 1:3
    plot(X2(c, :));
    hold on;
end
title("(H1) Plot of Decoded X Waveforms: Added Noise");
saveas (gcf, "T_X2H1N_waveforms.jpg")
%% Q5 ^
disp("Problem 7.2.4 - decoded signal in the presence of noise: ");
incorrect = 0;
bits = 0;
for r = 1:3 % loop through each row
    binSig = binary_decoder(X2(r, :));
    bS = binary_decoder(X(r, :));
    for c = 1:100 %loop through each column
        if (binSig(1, c) ~= bS(1, c))
            incorrect = incorrect + 1;
        end
        bits = bits + 1;
    end
end
disp("BER = " + incorrect/bits*100);

%% 7.1.2
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Noiseless Channel
% Y contains 3 received wavforms
% H is the transformation from the X sent signals to Y
Y = H2*X;
figure(6);
for c = 1:3
    plot(Y(c, :));
    hold on;
end
title("(H2) Plot of Coded Y Waveforms: Noiseless");
saveas (gcf, "TyH2_waveforms.jpg")

%% Decoding the Noiseless Channel
% Using the inverse of the transformation matrix H, we are going back from
% Y to X.
X2 = inv(H2)*Y;
figure(7);
for c = 1:3
    plot(X2(c, :));
    hold on;
end
title("(H2) Plot of Decoded X Waveforms: Noiseless");
saveas (gcf, "Tx2H2_waveforms.jpg")

%% Q5 ^
disp("Problem 7.3.2 - decoded signal in the absence of noise: ");
incorrect = 0;
bits = 0;
for r = 1:3 % loop through each row
    binSig = binary_decoder(X2(r, :));
    bS = binary_decoder(X(r, :));
    for c = 1:100 %loop through each column
        if (binSig(1, c) ~= bS(1, c))
            incorrect = incorrect + 1;
        end
        bits = bits + 1;
    end
end
disp("BER = " + incorrect/bits*100);

%% Noisy Channel
var = 1e-2;
N = sqrt(var)*randn(3, 1000);
Y = H2*X+ N;
figure(8);
for c = 1:3
    plot(Y(c, :));
    hold on;
end
title("(H2) Plot of Coded Y Waveforms: Added Noise");
saveas (gcf, "T_YH2N_waveforms.jpg")

%% Decoding a Noisy Channel
X2 = inv(H2)*Y;
figure(9);
for c = 1:3
    plot(X2(c, :));
    hold on;
end
title("(H2) Plot of Decoded X Waveforms: Added Noise");
saveas (gcf, "T_X2H2N_waveforms.jpg")
%% Q5 ^
disp("Problem 7.3.4 - decoded signal in the presence of noise: ");
incorrect = 0;
bits = 0;
for r = 1:3 % loop through each row
    binSig = binary_decoder(X2(r, :));
    bS = binary_decoder(X(r, :));
    for c = 1:100 %loop through each column
        if (binSig(1, c) ~= bS(1, c))
            incorrect = incorrect + 1;
        end
        bits = bits + 1;
    end
end
disp("BER = " + incorrect/bits*100);

%% 7.4 Precoding the transmitted signal
Xp = inv(H2)*X; % Xp is the precoded input
% "plot the signals obtained at the receiver"
% new output (X2):
Y = H2*Xp + N; % = X + N
figure(10);
for c = 1:3
    plot(Y(c, :));
    hold on;
end
title("(H2) Coded Y Waveforms of Precoded Xp: Added Noise");
saveas (gcf, "T_preX2H2N_waveforms.jpg")
%% Q5 ^
disp("Problem 7.4 - decoded signal in the presence of noise with precoding: ")
incorrect = 0;
bits = 0;
for r = 1:3 % loop through each row
    binSig = binary_decoder(Y(r, :));
    bS = binary_decoder(X(r, :));
    for c = 1:100 %loop through each column
        if (binSig(1, c) ~= bS(1, c))
            incorrect = incorrect + 1;
        end
        bits = bits + 1;
    end
end
disp("BER = " + incorrect/bits*100);
%% Q5 Decoding - binary Decoder
% Dispersed
