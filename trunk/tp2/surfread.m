function [kpts1, kpts2] = surfread(kpts1_file, kpts2_file)
    fid = fopen(kpts1_file, 'r');
    kpts1 = fscanf(fid, '%f', [2, inf])';
    fclose(fid);
    fid = fopen(kpts2_file, 'r');
    kpts2 = fscanf(fid, '%f', [2, inf])';
    fclose(fid);
end