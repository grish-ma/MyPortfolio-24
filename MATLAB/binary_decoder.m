function bits = binary_decoder(signal)
%Returns the binary sequence corresponding to the waveform

% samples per symbol
sps = 10;

% total length of the waveform
N = length(signal);

% waveform shape for correlation
waveform_template = ones(sps, 1);

% Total number of symbols in the waveform
bit_length = round(N/sps);

bits = zeros(1, bit_length);

% Convert the signal
signal = 2*(signal-0.5);

for i=1:sps:N
    correlation = signal(1, i:i+sps-1)*waveform_template;
    index = round(i/sps)+1;
    if correlation > 0
        bits(index) = 1;
    else
        bits(index) = 0;
    end
end

