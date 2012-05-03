% obtiene la recta en P2 que pasa por los dos puntos.
function l = obtener_recta(p1, p2)
    %Se pasan los punto a P2 para encontrar las rectas correspondientes a
    %cada par de puntos (p# = [px py]).
    a = [p1 1];
    b = [p2 1];
    
    l = cross(a, b);
end