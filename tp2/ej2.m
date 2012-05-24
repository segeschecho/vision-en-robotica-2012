function ej2(img1_file, img2_file, xs1i, H)
    %ejercicio 2: imagen 1 contra 2
    xs2i = H * xs1i;
    
    %convertimos los puntos de la imagen destino a R2
    xs1 = [xs1i(1, :) ./ xs1i(3, :) ; xs1i(2, :) ./ xs1i(3, :)];
    xs2 = [xs2i(1, :) ./ xs2i(3, :) ; xs2i(2, :) ./ xs2i(3, :)];
    
    H_dlt = DLT(xs1i, xs2i);
    
    H_ransac = RANSAC(xs1i, xs2i);
    
    % Show results
    f = figure(1);
    set(f,'name','Ejercicio 2: DLT','numbertitle','off');
    showresults(H,H_dlt,xs1',xs2',img1_file, img2_file);
    
    f = figure(2);
    set(f,'name','Ejercicio 2: RANSAC','numbertitle','off');
    showresults(H,H_ransac,xs1',xs2',img1_file, img2_file);

end