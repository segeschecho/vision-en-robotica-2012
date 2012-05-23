function H =  DLT(xs1i, xs2i)
    n = size(xs1i, 2);
    %calculamos el centroide en X y en Y para los dos conjuntos de puntos.
    c_xs1 = [ sum(xs1i(1,:)) sum(xs1i(2,:)) ]/n;
    c_xs2 = [ sum(xs2i(1,:)) sum(xs2i(2,:)) ]/n;
    
    %movemos los puntos para que queden centrados en el origen.
    %armamos las matrices que hacen la traslacion y luego el escalamiento para que todos
    %los puntos esten dentro circulo de radio 1
    M_xs1_traslacion = [1 0 -c_xs1(1); 0 1 -c_xs1(2); 0 0 1];
    M_xs2_traslacion = [1 0 -c_xs2(1); 0 1 -c_xs2(2); 0 0 1];
    
    %calculamos la media de las distancias al centroide para cada conjunto que ya estan trasladados.
    d_xs1 = 0;
    d_xs2 = 0;
    
    for i = 1:n
        d_xs1 = d_xs1 + norm(xs1i(:, i));
        d_xs2 = d_xs2 + norm(xs2i(:, i));
    end
    
    d_xs1 = d_xs1 / n;
    d_xs2 = d_xs2 / n;
    
    s_xs1 = sqrt(2) / d_xs1;
    s_xs2 = sqrt(2) / d_xs1;
    
    M_xs1_escala = [s_xs1 0 0; 0 s_xs1 0; 0 0 1];
    M_xs2_escala = [s_xs2 0 0; 0 s_xs2 0; 0 0 1];
    
    %Se escalan los puntos segun lo obtenido
    H_xs1_monio = M_xs1_escala * M_xs1_traslacion;
    H_xs2_monio = M_xs2_escala * M_xs2_traslacion;
    
    xs1_normalizados = H_xs1_monio * xs1i;
    xs2_normalizados = H_xs2_monio * xs2i;
    
    H_punto = DLT_puro(xs1_normalizados, xs2_normalizados);

    H = inv(H_xs2_monio) * H_punto * H_xs1_monio;
    H = H / H(3, 3);
    
end