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

			 %%%ej: esi = esquina superior izquierda
	 esi = Ha\[1 1 1]';
	 esd = Ha\[ 1 sizeX 1]';
	 eii = Ha\[sizeY 1  1]';
	 eid = Ha\[ sizeY  sizeX 1]';
	 eii = int16((eii(1:2))/eii(3));
	 esi = int16((esi(1:2))/esi(3));
	 esd = int16((esd(1:2))/esd(3));
	 eid = int16((eid(1:2))/eid(3));
	 
	 d_x_s = abs( esi(1) - esd(1));
	 d_x_i = abs( eii(1) - eid(1));
	 
	 d_y_i = abs( esi(2) - eii(2));
	 d_y_d = abs( esd(2) - eid(2));
	 
	maxxo = min(1000, max(d_x_s, d_x_i));
	maxyo = min(1000, max(d_y_i, d_y_d));

    imagenRectificada = zeros(maxyo,maxxo);

    for ya = 1:sizeY
        for xa = 1:sizeX
            t = Ha\[xa; ya; 1];
            t(1) = t(1)/t(3);
            t(2) = t(2)/t(3);
            xr = int16(t(1));
            yr = int16(t(2));
            imagenRectificada(yr+maxyo+1, xr+maxxo+1) = imagen(ya, xa);
        end
    end

end