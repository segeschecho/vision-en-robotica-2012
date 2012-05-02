% devuelve una imagen rectificada a menos de una afinidad partiendo de 2
% pares de rectas paralelas para hallar l_infinito y luego la H
function imagenRectificada = ej2(imagen, pl1, pl2, pm1, pm2, pr1, pr2, ps1, ps2)
    %cada una de las rectas es de la forma ax + by + c = 0
    % se obtienen las rectas que pasan por cada par.
    % l paralela a m
    % r paralela a s
    l = obtener_recta(pl1, pl2);
    m = obtener_recta(pm1, pm2);
    
    r = obtener_recta(pr1, pr2);
    s = obtener_recta(ps1, ps2);
    
    % normalizamos
    l = l / l(3);
    m = m / m(3);
    
    r = r / r(3);
    s = s / s(3);
    
    % se obtiene la interseccion de las rectas que tendria que ser en el
    % infinito
    p_inf1 = cross(l, m);
    p_inf2 = cross(r, s);
    
    l_infinito = cross(p_inf1, p_inf2);
    l_infinito_imagen = l_infinito / l_infinito(3);
    
    imagenRectificada = rectificar_imagen(imagen, l_infinito_imagen);
end