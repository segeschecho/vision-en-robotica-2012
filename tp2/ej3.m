function ej3(img1_file, img2_file, xs1i, xs2i, xs1, xs2, H)
    H_dlt = DLT(xs1i, xs2i);
    H_ransac = RANSAC(xs1i, xs2i);
    
    % Show results
    f = figure(3);
    set(f,'name','Ejercicio 3: DLT','numbertitle','off');
    showresults(H, H_dlt, xs1, xs2, img1_file, img2_file);
    
    f = figure(4);
    set(f,'name','Ejercicio 3: RANSAC','numbertitle','off');
    showresults(H, H_ransac, xs1, xs2, img1_file, img2_file);
end