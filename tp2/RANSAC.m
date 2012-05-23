function H = RANSAC(xs1i, xs2i)
    %epsilon para la cantidad de outliers
    e = .99;
    p = .99;
    %cantidad de correspondencias a usar para estimar la homografia H.
    s = 4;
    %umbral para elegir inliers
    t = 5.99;
    % seteamos la cantidad de ciclos para el RANSAC
    N = 9999999;
    
    cant_correspondencias = size(xs1i, 2);
    indices_inliers_maximal = [];
    
    iteracion = 0;
    
    for iteracion=1:50
        iteracion
        %se eligen correspondencias al azar.
        [h1, h2] = generador_hipotesis(xs1i, xs2i, cant_correspondencias, s);
        
        %se calcula la homografia H
        H = DLT_puro(h1, h2);
        
        % si el determinante de H es cercano a cero H esta cerca de no ser inversible, entonces la
        % retroproyeccion va a ser muy mala.
        if abs(det(H)) > .1
            %calculamos las distancias para cada par de puntos(correspondencias)
            indices_inliers = calcular_indices_inliers(xs1i, xs2i, cant_correspondencias, H, t);
            
            % Se verifica si el soporte actual es mejor que
            if length(indices_inliers) > length(indices_inliers_maximal)
                indices_inliers_maximal = indices_inliers;
                
                s = length(indices_inliers);
                
                e = 1 - s / cant_correspondencias;
                N = log(1 - p) / log(1 - (1 - e)^s)
            end
        end
    end
    
    H = DLT(xs1i(:, indices_inliers_maximal), xs2i(:, indices_inliers_maximal));
end


function [h1, h2] = generador_hipotesis(xs1i, xs2i, cant_correspondencias, s)
    for i=1:s
        c = floor(rand() * cant_correspondencias) + 1;
        h1(:, i) = xs1i(:, c)
        h2(:, i) = xs2i(:, c);
    end
end

function dist = distancia(xi, xi_prima, H)
    H_inv = inv(H);
    %se calcula la distancia entre puntos de acuerdo a lo que está en la practica.
    a = xi;
    b = H_inv * xi_prima;
    
    dist = (a(1) - b(1))^2 + (a(2) - b(2))^2;
    
    c = xi_prima;
    d = H * xi;
    
    dist = dist + (c(1) - d(1))^2 + (c(2) - d(2))^2;
end


function indices_inliers = calcular_indices_inliers(xs1i, xs2i, cant_correspondencias, H, umbral)
    distancias = [];
    for i=1:cant_correspondencias
        d = distancia(xs1i(:, i), xs2i(:, i), H);
        
        distancias(i) = d;
    end
    
    %calculamos inliers (devuelve las posiciones que cumplen con la condicion)
    indices_inliers = find(distancias < umbral^2);
end