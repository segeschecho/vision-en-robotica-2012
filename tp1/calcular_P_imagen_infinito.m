% P_imagen_infinito es un punto en R2 que representa un punto de fuga
function P_imagen_infinito = calcular_P_imagen_infinito(p1_imagen, p2_imagen, p3_imagen)
  
	 % Se interpetran los puntos como si estuvieran sobre una linea. 
	 % y vamos a buscar el punto de fuga sobre esa linea.
	 % Sabemos que en el mundo tenemos los puntos colineales a, b y c, 
	 % y en la imagen sus proyecciones a', b' y c' (tambien colineales,
	 % pues las proyecciones envian rectas en rectas)
	 %
	 % Mundo -> Imagen
	 %   a   ->   a'
	 %   b   ->   b' 
	 %   c   ->   c' 
	 %
	 % Si ahora consideramos:
	 % 1) la recta (a, b, c) en el mundo (imagen "real") y 
	 % 		consideramos que dist(a,b) = dist(b,c) = 1, podemos dar
	 % 		a la recta un marco de referencia tal que:
	 % 			a equivale a 0 
	 %		 	b equivale a 1 
	 % 			c equivale a 2 
	 %
	 % 2) la recta (a', b', c') en la imagen (distorsionada) 
	 % 		con un marco de referencia tal que:
	 % 			a' equivale a 0 = dist(a',a')
	 %		 	b' equivale a a_recta_img = dist(a',b')
	 % 			c' equivale a b_recta_img = dist(a',b') + dist(b',c')
	 %
	 % tenemos que:
	 %
	 % Recta en el Mundo -> Recta en la imagen Imagen
	 %   			0   			 ->            0
	 %   			1   			 ->         a_recta
	 %   			2          ->         b_recta
	 %
	 % Buscamos ahora la matriz de trasformacion H2x2 que mapea los puntos 
	 % (homogenizados) de esa manera. Esta es del estilo:
   % H = [h11 h11; 
   %      h21   1]
   % (el 1 es porque alcanza con la matriz a menos de una constante
   % multiplicativa)
	 % 
   % Considerando que queremos que H mapee 0 -> 0 
	 %
	 %   		H*[0 1] = [0 k]  => h11*0 + h21*1 = 0  =>  h21 = 0 
 	 %
	 % resulta que 
   %                          H = [h11  0; 
   %                               h21  1]
	 %
 	 % Por lo tanto, debemos hallar solo h11 y h21, de manera que se cumpla:
	 %
	 %   si a_recta = x1/y1 , entonces  [h11  0; h21 1]*[1, 1] = [x1 y1]
	 %   si b_recta = x2/y2 , entonces  [h11  0; h21 1]*[2, 1] = [x2 y2]
	 %
	 % Entonces:
	 %   a_recta = x1/y1 = (h11)/(h21+1)  => h11 = a_recta (h21 + 1)
	 %   b_recta = 2*h11 / 2*h21 + 1      => 2*h11 = 2*b_recta*2*h21 + b_recta
	 %
	 % => 2*a_recta(h21 + 1) = 2*b_recta*2*h21 + b_recta
	 %
	 % Luego, 
	 %    h21 = (b_recta - 2*a_recta)/2*(a_recta - b_recta)
	 %    h11 = a_recta (h21 + 1)
	 %
	 % Una vez que tenemos H2x2, podemos calcular la proyeccion del 
	 % punto en el infinito sobre la recta, para luego calcular las
	 % coordenadas del punto en la imagen (que es lo que buscabamos)
	 
	 a_recta = norm(p1_imagen - p2_imagen); % esto es a
   b_recta = a_recta + norm(p2_imagen - p3_imagen); % esto es a+b
    
   h21 = (b_recta - 2*a_recta)/(2*(a_recta - b_recta));
   h11 = a_recta*(h21 + 1);

   H = [h11 0; h21 1];
   
   % sacamos el punto en el infinito
   lp = H*[1 0]';
   
   p = lp(1)/lp(2);

   % Ahora calculamos los Px y Py del punto en el infinito
   % Para eso, calculamos el vector director de la recta en la imagen,
   % lo normalizamos y luego lo escalamos y movemos al primer punto
   % clickeado de la recta.
   
   direccion_l = (p2_imagen - p1_imagen) / norm(p2_imagen - p1_imagen);
   
   p_escalado = direccion_l * p;
   
   P_imagen_infinito = p_escalado + p1_imagen;
   
end