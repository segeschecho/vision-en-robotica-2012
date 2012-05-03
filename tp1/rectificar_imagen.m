function imagenRectificada = rectificar_imagen(imagen, l_infinito_imagen)
   % calculamos H
   Hp = [1 0 0; 0 1 0; l_infinito_imagen];

   [ sizeY sizeX ] = size(imagen);

   % hacemos una primer pasada para calcular el
   % tamaño de la imagen final rectificado

	 %%%ej: esi = esquina superior izquierda
	 esi = Hp*[1 1 1]';
	 esd = Hp*[ 1 sizeX 1]';
	 eii = Hp*[sizeY 1  1]';
	 eid = Hp*[ sizeY  sizeX 1]';
	 eii = int16((eii(1:2))/eii(3));
	 esi = int16((esi(1:2))/esi(3));
	 esd = int16((esd(1:2))/esd(3));
	 eid = int16((eid(1:2))/eid(3));
	 
	 d_x_s = abs( esi(1) - esd(1));
	 d_x_i = abs( eii(1) - eid(1));
	 
	 d_y_i = abs( esi(2) - eii(2));
	 d_y_d = abs( esd(2) - eid(2));
	 
	 maxxo = min(1000, max(d_x_s, d_x_i));
	 maxyo = min(1000, max(d_y_i, d_y_d));
	 
   % finalmente generamos la imagen rectificada
   imagenRectificada = zeros(maxyo, maxxo);

   for yr = 1:sizeY
       for xr = 1:sizeX
           t = Hp*[xr; yr; 1];
           t(1) = t(1)/t(3);
           t(2) = t(2)/t(3);
           xo = round(t(1));
           yo = round(t(2));

           imagenRectificada(yo+1, xo+1) = imagen(yr, xr);
       end


   end
end