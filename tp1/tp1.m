function tp1(nombre_imagen)
		limpiar_todo();
						
		lista_de_ejercicios = {'Ejercicio 1','Ejercicio 2','Ejercicio 3'};
		promt = 'Trabajo Practico Nº 1 - Benitti Raul, Gonzalez Emiliano, Gonzalez Sergio, Pire Tahiu';
		[Selection ok] = listdlg('PromptString', promt,'ListString',lista_de_ejercicios, 'SelectionMode', 'single' , 'ListSize', [ 400 50], 'Name', 'Vision en Robotica 2012C1');
		
		if ok == 1
			%leemos lo imagen (proyeccion del mundo)
			[imagen,map] = imread(nombre_imagen);
	       
      switch Selection(1)
				case 1
					%resultados del ejercicio 1
				  %mostramos la imagen original y buscamos los puntos.
					mostrar_imagen(imagen, map, 1,'Imagen Original')
					puntos = leer_puntos_de_imagen(6,3);
					%rectificamos la imagen con el metodo de las razones cruzadas
					imagenRectificada = ej1(imagen, puntos(1, :), puntos(2, :), puntos(3, :), puntos(4, :), puntos(5, :), puntos(6, :));
					%mostramos la imagen rectificada (en este caso, a menos de una afinidad)
					mostrar_imagen(imagenRectificada, map, 2,'Imagen rectificada a menos de una Afinidad');
				case 2
					%resultados del ejercicio 2
				  %mostramos la imagen original y buscamos los puntos.
					mostrar_imagen(imagen, map, 1,'Imagen Original')
					puntos = leer_puntos_de_imagen(8,2);
					%rectificamos la imagen con usando dos pares de lineas paralelas 
					%(en el mundo) para obtener los puntos en el infinito
					imagenRectificada = ej2(imagen, puntos(1, :), puntos(2, :), puntos(3, :), puntos(4, :), puntos(5, :), puntos(6, :), puntos(7, :), puntos(8, :));
					%mostramos la imagen rectificada (en este caso, a menos de una afinidad)
					mostrar_imagen(imagenRectificada, map, 2,'Imagen rectificada a menos de una Afinidad');
				case 3
					% resultados del ejercicio 3
					% mostramos la imagen original y tomamos 6 puntos para rectificar a menos de una afinidad
					mostrar_imagen(imagen, map, 1,'Imagen Original (Paso 1/3)')
					puntos = leer_puntos_de_imagen(6,3);
					% rectificamos la imagen con el metodo de las razones cruzadas, a menos de una afinidad
					% mostramos la imagen resultante, y seleccionamos 2 pares de lineas para rectificar
					% a menos de una similaridad
					imagen_a_menos_de_una_afinidad = ej1(imagen, puntos(1, :), puntos(2, :), puntos(3, :), puntos(4, :), puntos(5, :), puntos(6, :));
					mostrar_imagen(imagen_a_menos_de_una_afinidad, map, 2,'Imagen rectificada a menos de una Afinidad (Paso 2/3)');
					%rectificamos la imagen a menos de una similaridad
					puntos = leer_puntos_de_imagen(8,2);
					imagenRectificada = ej3(imagen_a_menos_de_una_afinidad, puntos(1, :), puntos(2, :), puntos(3, :), puntos(4, :), puntos(5, :), puntos(6, :), puntos(7, :), puntos(8, :));
					%mostramos la imagen rectificada (en este caso, a menos de una afinidad)
					mostrar_imagen(imagenRectificada, map, 3,'Imagen rectificada a menos de una Similaridad (Paso 3/3)');
			end
		end
end