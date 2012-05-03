function  puntos = leer_puntos_de_imagen(cantidad_de_puntos, puntos_por_linea)
		colores = {'red', 'blue', 'green', 'yellow'};
	 %Se obtienen los puntos de la imagen
   %puntos es una matriz donde cada fila es un punto
   puntos = zeros(cantidad_de_puntos, 2);
   for i = 1:cantidad_de_puntos
      p = getMouseXY()
      puntos(i, :) = p;
			linea = floor((i-1)/puntos_por_linea)+1
			mod(i,puntos_por_linea)
			if mod(i-1,puntos_por_linea) > 0
				
				pline=plot([puntos(i-1, 1), puntos(i, 1)], [puntos(i-1,2), puntos(i,2)]);
				set(pline,'Color',colores{linea},'LineWidth',4);
			end
	 end
   

end