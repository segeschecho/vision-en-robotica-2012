
function showresults(H, Hest, xs1, xs2, img1_file, img2_file)

    % Show matches
    img1 = imread(img1_file);
    img2 = imread(img2_file);
    [high width] = size(img1);
    img = [img1;img2]; 
    imshow(img);
    
    % Draw matches: correct (green), correct but with wrong H estimated (blue), incorrect (red) 
    epsilon = 5;
    nbpoints = size(xs1);
    nbpoints = nbpoints(1);
    for i=1:nbpoints,
        x1 = xs1(i,:);
        x2 = xs2(i,:);
        x1_hom = [x1(1) x1(2) 1];
        x1_prima = H * x1_hom';
        x1_prima_inhom = x1_prima / x1_prima(3);
        x1_prima_est = Hest * x1_hom';
        x1_prima_inhom_est = x1_prima_est / x1_prima_est(3);
        err1 = norm(x1_prima_inhom-[x2 1]');
        err2 = norm(x1_prima_inhom-x1_prima_inhom_est);
        x1est = [x1_prima_inhom_est(1) x1_prima_inhom_est(2)];
        if err1 > epsilon
            line([x1(1) x1est(1)], [x1(2) x1est(2)+high], 'Marker','.','LineStyle','-', 'Color','r');
        else
            if err2 > epsilon
                line([x1(1) x1est(1)], [x1(2) x1est(2)+high], 'Marker','.','LineStyle','-', 'Color','b');
            else
                line([x1(1) x1est(1)], [x1(2) x1est(2)+high], 'Marker','.','LineStyle','-', 'Color','g');
            end
            
        end
    end

end
