function imagenRectificada = ej3(imagen, pl1, pl2, pm1, pm2 ,pn1, pn2, po1, po2)

    % todo lo siguiente es de de la imagen a menos de una afinidad, homogeneizado
    l = obtener_recta(pl1, pl2);
    m = obtener_recta(pm1, pm2);
    n = obtener_recta(pn1, pn2);
    o = obtener_recta(po1, po2);

    A = [l(1)*m(1)    l(1)*m(2)+l(2)*m(1); 
         n(1)*o(1)    n(1)*o(2)+n(2)*o(1)];
    b = [-l(2)*m(2)  -n(2)*o(2)];

    S_vector = A\b';

    S = [S_vector(1) S_vector(2); S_vector(2) 1];

    % K debe ser la triangular superior
    K = chol(S)';

    Ha = [K(1) K(2) 0; K(3) K(4) 0; 0 0 1];
    
    [sizeY, sizeX] = size(imagen);

    maxxo = 0;
    maxyo = 0;
    minxo = 0;
    minyo = 0;

    for ya = 1:sizeY
        for xa = 1:sizeX
           t = Ha\[xa; ya; 1];
           t(1) = t(1)/t(3);
           t(2) = t(2)/t(3);
           xo = int16(t(1));
           yo = int16(t(2));
           if( xo < minxo )
               minxo = xo;
           end
           if( yo < minyo )
               minyo = yo;
           end
           if( xo > maxxo )
               maxxo = xo;
           end
           if( yo > maxyo )
               maxyo = yo;
           end
        end
    end

    minxo = abs(minxo);
    minyo = abs(minyo);

    imagenRectificada = zeros(maxyo,maxxo);

    for ya = 1:sizeY
        for xa = 1:sizeX
            t = Ha\[xa; ya; 1];
            t(1) = t(1)/t(3);
            t(2) = t(2)/t(3);
            xr = int16(t(1));
            yr = int16(t(2));
            imagenRectificada(yr+minyo, xr+minxo) = imagen(ya, xa);
        end
    end

end