
function tp2(img1_file, img2_file, des1_file, des2_file, kpts1_file, kpts2_file, H_file)

    % Read Homography
    H = load(H_file);
    
    % Read surf keypoints
    [kpts1, kpts2] = surfread(kpts1_file, kpts2_file);
    
    % Establish matching with the descriptors of the keypoints
    [xs1 xs2] = surfmatch(des1_file, des2_file, kpts1, kpts2);
    
    % Estimate Homography
    xs1i = [xs1 ones(size(xs1,1),1)];
    xs2i = [xs2 ones(size(xs2,1),1)];
    
    ej2(img1_file, img2_file, xs1i', H);
    
    ej3(img1_file, img2_file, xs1i', xs2i', xs1, xs2, H);
end