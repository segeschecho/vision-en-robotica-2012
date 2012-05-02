function tp1
    limpiar_todo();

    [X,map,puntos] = leer_puntos_de_imagen('piso.gif' , 6);

    imagen_a_una_afinidad = ej1(X, puntos(1, :), puntos(2, :), puntos(3, :), puntos(4, :), puntos(5, :), puntos(6, :));

    f = figure(2);
    set(f,'name','Imagen A Una Afinidad','numbertitle','off');

    imshow(imagen_a_una_afinidad, map);

%    [X,map,puntos] = leer_puntos_de_imagen('piso.gif' , 8);
%    gmap = rgb2gray(map);

%    imagenRectificada = ej2(X, puntos(1, :), puntos(2, :), puntos(3, :), puntos(4, :), puntos(5, :), puntos(6, :), puntos(7, :), puntos(8, :));

%    f = figure(2);
%    set(f,'name','Imagen Rectificada','numbertitle','off');

%    imshow(imagenRectificada, gmap);

    puntos = zeros(8, 2);
    for i = 1:8
      p = getMouseXY()
      puntos(i, :) = p;
    end

%    imagenRectificada = ej3(X, p1, p2, p3, p4, p5, p6, p7, p8);
    imagenRectificada = ej3(imagen_a_una_afinidad, puntos(1, :), puntos(2, :), puntos(3, :), puntos(4, :), puntos(5, :), puntos(6, :), puntos(7, :), puntos(8, :));

    f = figure(3);
    set(f,'name','Imagen Rectificada','numbertitle','off');

    imshow(imagenRectificada, map);

end