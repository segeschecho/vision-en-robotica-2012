% P_imagen_infinito es un punto en R2 que representa un punto de fuga
function P_imagen_infinito = calcular_P_imagen_infinito(p1_imagen, p2_imagen, p3_imagen)
   
   a_imagen = norm(p1_imagen - p2_imagen); % esto es a
   b_imagen = norm(p1_imagen - p3_imagen); % esto es a+b
   
   % se interpretan las distancias como si estuviesen en una linea
   % luego se pasan a espacio P1 para encontrar el punto en el infinito
   % dentro de esa recta.
   % los tres puntos son de la forma: (0,1), (a, 1) (a+b, 1)
   % ACLARACION: por ahora no se usa y queda comentado
   %l0 = [0 1];
   %la = [a_imagen 1];
   %lb = [b_imagen 1];

   % buscamos la matriz de trasformacion H que mapea las razones del
   % mundo a la imagen.
   % Esta matriz es del estilo:
   % H = [h11 0;
   %      h21 1].
   % El 0 es porque quiero mandar el 0 al 0, por lo tanto H*[0 1] = [0 k],
   % y de ahi se deduce que h12 = 0.
   % El 1 es porque me alcanza con la matriz a menos de una constante
   % multiplicativa

   h21 = (b_imagen - 2*a_imagen)/(2*(a_imagen - b_imagen));
   h11 = b_imagen*(2*h21 + 1)/2;

   H = [h11 0; h21 1];
   
   % sacamos el punto en el infinito
   lp = H*[1 0]';
   
   p = lp(1)/lp(2);

   % ahora calculamos los Px y Py del punto en el infinito
   % para eso, calculamos el vector director de la recta en la imagen,
   % lo normalizamos y luego lo escalamos y movemos al primer punto
   % clickeado de la recta.
   
   direccion_l = (p2_imagen - p1_imagen) / norm(p2_imagen - p1_imagen);
   
   p_escalado = direccion_l * p;
   
   P_imagen_infinito = p_escalado + p1_imagen;
   
end