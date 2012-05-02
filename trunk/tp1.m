function tp1
    limpiar_todo();

    [X,map,puntos] = leer_puntos_de_imagen('piso.gif' , 6);
    gmap = rgb2gray(map);

    imagenRectificada = ej1(X, puntos(1, :), puntos(2, :), puntos(3, :), puntos(4, :), puntos(5, :), puntos(6, :));

    f = figure(2);
    set(f,'name','Imagen Rectificada','numbertitle','off');

    imshow(imagenRectificada, gmap);

%    [X,map,puntos] = leer_puntos_de_imagen('piso.gif' , 8);
%    gmap = rgb2gray(map);

%    imagenRectificada = ej2(X, puntos(1, :), puntos(2, :), puntos(3, :), puntos(4, :), puntos(5, :), puntos(6, :), puntos(7, :), puntos(8, :));

%    f = figure(2);
%    set(f,'name','Imagen Rectificada','numbertitle','off');

%    imshow(imagenRectificada, gmap);
end