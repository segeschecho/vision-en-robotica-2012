% obtiene la recta en P2 que pasa por los dos puntos.
function l = obtener_recta(p1, p2)
    %Se pasan los punto a P2 para encontrar las rectas correspondientes a
    %cada par de puntos.
    p21 = [p1 1];
    p22 = [p2 1];
    
    l = cross(p21, p22);
end