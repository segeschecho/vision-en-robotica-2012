function imagenRectificada = rectificar_imagen(imagen, l_infinito_imagen)
   % calculamos H
   Hp = [1 0 0; 0 1 0; l_infinito_imagen];

   [ sizeY sizeX ] = size(imagen);

   % hacemos una primer pasada para calcular el
   % tamaño de la imagen final rectificado
   maxyo = 0;
   maxxo = 0;


   for yr = 1:sizeY
       for xr = 1:sizeX
           t = Hp*[xr; yr; 1];
           t(1) = t(1)/t(3);
           t(2) = t(2)/t(3);
           xo = round(t(1));
           yo = round(t(2));
           if( xo > maxxo )
               maxxo = xo;
           end
           if( yo > maxyo )
               maxyo = yo;
           end
       end
   end
   
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