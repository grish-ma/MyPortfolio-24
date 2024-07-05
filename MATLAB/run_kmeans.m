clear; close all; clc;
%% data loader
rng(0);
load('mnist_train.mat')

%% 8.1 MNIST Training Data

figure(1);
img1 = reshape(data(1,:),28,28);
imshow(img1);
saveas(gcf, "ImageType1");

figure(2);
img2 = reshape(data(2,:),28,28);
imshow(img2);
saveas(gcf, "ImageType2");

figure(3);
img3 = reshape(data(3,:),28,28);
imshow(img3);
saveas(gcf, "ImageType3");
%% 8.2
rng(0); % Sets the random number seed for repeatable results
[idx, C] = kmeans(data,3,'replicates',5); % Runs K-Means
% The above lies assigns values from 1 to 3 to assign each ROW
% in the "data" matrix a cluster (cluser 1, 2, or 3)
% Matris idx contains the category number for each row of data

% Matrix C's rows contain the center point for each cluster
figure(4);
img = reshape(C(1,:),28,28);
imshow(img);
saveas(gcf, "cluster_1.jpg");

figure(5);
img = reshape(C(2,:),28,28);
imshow(img);
saveas(gcf, "cluster_2.jpg");

figure(6);
img = reshape(C(3,:),28,28);
imshow(img);
saveas(gcf, "cluster_3.jpg");

% Comparing idx entries (assigned category) with "labels" 
% entries (true values)
% In "idx" --> "labels": 
% 1 --> 7, 2 --> 6, 3 --> 1
idx(idx==1) = 7; % Replace idx values of 1 with 7, etc.
idx(idx==2) = 6;
idx(idx==3) = 1;
idx = reshape(idx, 1892, 1);

% Calculating Error Rate
error = 0;
dataPts = 0;
for c = 1:1892
    if (idx(c) ~= labels(c))
        error = error +1;
        % display("row = " + c)
    end
    dataPts = dataPts +1;
end
display("error rate = " + error/dataPts*100);

% Identify 3 examples of errors
figure(7);
ex = reshape(data(19,:),28,28);
imshow(ex);
saveas(gcf, "error_1.jpg");

figure(8);
ex = reshape(data(40,:),28,28);
imshow(ex);
saveas(gcf, "error_2.jpg");

figure(9);
ex = reshape(data(62,:),28,28);
imshow(ex);
saveas(gcf, "error_3.jpg");
%% 8.3
rng(0);
load('mnist_test.mat')

% Find distance between three center points in C and
% each data_test row. Then classify based on min dist
dist = [312, 3];
idx_test = [312, 1];
for row = 1:312
    dist(row, 1) = norm(data_test(row, :)-C(1, :));
    dist(row, 2) = norm(data_test(row, :)-C(2, :));
    dist(row, 3) = norm(data_test(row, :)-C(3, :));
    % First column = distance from first center point, etc.
    % Find minimum distance and assign label value for the 
    % corresponding row in data_test
    [M, idx_test(row)] = min(dist(row, :));
end
idx_test = reshape(idx_test, 312, 1);

% "idx_test" --> "labels_test"
% 1 --> 7, 2 --> 6, 3 --> 1
idx_test(idx_test==1) = 7; % Replace idx_test values of 1 with 7, etc.
idx_test(idx_test==2) = 6;
idx_test(idx_test==3) = 1;

for x = 1:5
    disp(idx_test(x));
end

% Calculating Error Rate
error = 0;
dataPts = 0;
for c = 1:312
    if (idx_test(c) ~= labels_test(c))
        error = error + 1;
        % display("row = " + c)
    end
    dataPts = dataPts +1;
end
display("error rate for test = " + error/dataPts*100);

% Identify 3 examples of errors
figure(10);
ex = reshape(data(39,:),28,28);
imshow(ex);
saveas(gcf, "error_test1.jpg");

figure(11);
ex = reshape(data(48,:),28,28);
imshow(ex);
saveas(gcf, "error_test2.jpg");

figure(12);
ex = reshape(data(50,:),28,28);
imshow(ex);
saveas(gcf, "error_test3.jpg");

