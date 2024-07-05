function C = get_covar_matrix(X)
% Problem 1:
% This function takes as input matrix X, which is m x n, and outputs
% the covariance matrix.

% Compute mean xbar vector using columns of X
xbar = mean(X'); % mean() returns a row vector of means of each column
xbar = xbar';

% Remove mean from all X column vectors
y = xbar;
n = size(X, 2);
C = zeros(size(y, 1));
for c = 1:n
    y(:, c) = X(:, c) - xbar;

    % Compute covariance vector
    C = C + y(:, c)*(y(:, c)');

end

C = 1/n * C;
end