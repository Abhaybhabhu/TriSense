%% TriSense Roll Analysis - V0.1
% Purpose:
% Load TriSense IMU CSV data, clean the roll-axis gyroscope signal,
% smooth it, and compare left/right roll peaks.

clear; clc; close all;

%% Load data
filename = "C:\Users\abhay\OneDrive\Documents\TriSense\TriSense\data\raw\TRI_003.CSV";
data = readtable(filename);

% Convert time to seconds and start from zero
t = (data.time_ms - data.time_ms(1)) / 1000;

% Main lower-back roll candidate axis
gx = data.gx_dps;

%% Remove first 5 seconds as settling period
settleTime = 5;
validIdx = t >= settleTime;

t_clean = t(validIdx) - settleTime;
gx_clean = gx(validIdx);

%% Estimate gyro bias from first 3 seconds after settling
biasWindow = t_clean <= 3;
gx_bias = mean(gx_clean(biasWindow));

gx_corrected = gx_clean - gx_bias;

%% Smooth signal
% Moving average window.
% At ~20 Hz, 5 samples = 0.25 seconds.
windowSize = 5;
gx_smooth = movmean(gx_corrected, windowSize);

%% Detect positive and negative peaks
minPeakHeight = 20;      % deg/s, adjust if needed
minPeakDistance = 0.8;   % seconds, avoids detecting tiny repeated bumps

sampleRate = 1 / median(diff(t_clean));
minPeakDistanceSamples = round(minPeakDistance * sampleRate);

[posPeaks, posLocs] = findpeaks(gx_smooth, ...
    "MinPeakHeight", minPeakHeight, ...
    "MinPeakDistance", minPeakDistanceSamples);

[negPeaksAbs, negLocs] = findpeaks(-gx_smooth, ...
    "MinPeakHeight", minPeakHeight, ...
    "MinPeakDistance", minPeakDistanceSamples);

negPeaks = -negPeaksAbs;

%% Calculate simple symmetry metric
meanPos = mean(posPeaks);
meanNeg = mean(abs(negPeaks));

symmetryScore = 1 - abs(meanPos - meanNeg) / max(meanPos, meanNeg);

%% Print results
fprintf("Estimated sample rate: %.2f Hz\n", sampleRate);
fprintf("Gyro bias on gx: %.3f deg/s\n", gx_bias);
fprintf("Mean positive roll peak: %.2f deg/s\n", meanPos);
fprintf("Mean negative roll peak: %.2f deg/s\n", meanNeg);
fprintf("Roll symmetry score: %.3f\n", symmetryScore);

%% Plot raw vs corrected/smoothed
figure;
plot(t_clean, gx_corrected, "DisplayName", "Bias-corrected gx");
hold on;
plot(t_clean, gx_smooth, "LineWidth", 1.5, "DisplayName", "Smoothed gx");

plot(t_clean(posLocs), posPeaks, "o", "DisplayName", "Positive peaks");
plot(t_clean(negLocs), negPeaks, "o", "DisplayName", "Negative peaks");

xlabel("Time (s)");
ylabel("Angular velocity, gx (deg/s)");
title("TriSense Lower-Back Roll Signal");
legend;
grid on;