function [z] = vector_thresholding(vector, K)
%QUESTION: for loop nec?  
    N = max(size(vector));
    magnitude = abs(vector);
    
    [magnitude, index] = sort(magnitude);
    
    %for i=1:N
        vector(index(1:N-K)) = 0;
        
  %  end
    z = vector;

end