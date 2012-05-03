function  puntos = leer_puntos_de_imagen(cantidad_de_puntos)

	 %Se obtienen los puntos de la imagen
   %puntos es una matriz donde cada fila es un punto
   puntos = zeros(cantidad_de_puntos, 2);
   for i = 1:cantidad_de_puntos
      p = getMouseXY()
      puntos(i, :) = p;
   end
end