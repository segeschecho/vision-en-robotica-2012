function mostrar_imagen(imagen, map, numero, titulo)
	hold off;
	f = figure(numero);
	set(f,'name',titulo,'numbertitle','off');
	imshow(imagen, map);
	hold on;
end