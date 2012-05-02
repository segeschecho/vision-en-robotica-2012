function [imagen, map, puntos] = leer_puntos_de_imagen(nombre_imagen, cantidad_de_puntos)
    %leemos loa imagen del piso en perspectiva
    [imagen,map] = imread(nombre_imagen);

    %mostramos la imagen para buscar los puntos.
    f = figure(1);
    set(f,'name','Imagen Original','numbertitle','off');

    imshow(imagen, map);
    hold on;

    %Se obtienen los puntos de la imagen
    %puntos es una matriz donde cada fila es un punto
    puntos = zeros(cantidad_de_puntos, 2);
    for i = 1:cantidad_de_puntos
      p = getMouseXY()
      puntos(i, :) = p;
    end
end