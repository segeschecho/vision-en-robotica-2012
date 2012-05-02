function imagenRectificada = ej1(imagen, pl1, pl2, pl3, pm1, pm2, pm3)
   
   % calculamos P
   P = calcular_P_imagen_infinito(pl1, pl2, pl3);
   
   % calculamos Q
   Q = calcular_P_imagen_infinito(pm1, pm2, pm3);
   
   % calculamos l_infinito en la imagen y la normalizamos
   l_infinito_imagen = cross([P 1], [Q 1]);
   l_infinito_imagen = l_infinito_imagen / l_infinito_imagen(3);
   
   imagenRectificada = rectificar_imagen(imagen, l_infinito_imagen);
end