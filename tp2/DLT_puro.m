function H = DLT_puro(xs1i, xs2i)
    n = size(xs1i, 2);
    % se reciben las correspondencias en los parametros de la funcion
    %Se arma la matriz A de dimension 2n x 9
    ceros = [0 0 0];
    
    for i = 1:n
        
        xi = xs1i(:, i)';
        
        A(2*i - 1,:) = [ceros -xs2i(3,i)*xi xs2i(2,i)*xi];

        A(2*i,:) = [xs2i(3,i)*xi ceros -xs2i(1,i)*xi];
    end
    
    
    [U, D, V] = svd(A);

    H = reshape(V(:, 9), 3, 3)'; % [1 2 3 4 5 6] = [1 3 5; 2 4 6]'
end