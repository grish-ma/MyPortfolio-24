function [A, b] = matrix_equation(X, B)
% Already done: define A and b as described in the assignment.
% You can use loops and concatenation to make this work.

    n = numel(X);
    A = [];
    b = [];
    
    for k=1:n-B
        ak = X(B+k-1:-1:k)'; % x(B), x(B+1) to x(N-1)
        A = [A; ak];
        
        bk = X(B+k); % bk goes from X(B+1) to X(N) .
        b = [b; bk];
    end
end
