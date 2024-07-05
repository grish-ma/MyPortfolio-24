function [ e ] = matrix_error( X, Y )
%computes error between two matrices

e = norm(X-Y,'fro');

end

