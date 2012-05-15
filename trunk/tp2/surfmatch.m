

function [p1 p2] = surfmatch(desc1_file, desc2_file, kpt1, kpt2)
    
    % Read descriptors files
    fid = fopen(desc1_file, 'r');
    desc1 = fscanf(fid, '%f', [64, inf])';
    fclose(fid);
    fid = fopen(desc2_file, 'r');
    desc2 = fscanf(fid, '%f', [64, inf])';
    fclose(fid);
    
    [nb_desc1 cols] = size(desc1);
    [nb_desc2 cols] = size(desc2);

    % Establish matches
    desc1 = desc1';
    desc2 = desc2';
    matches_index = [];   
    for j=1:nb_desc1,
        desc = repmat(desc1(:,j), 1, nb_desc2);
        distances = sqrt( sum((desc-desc2).*(desc-desc2) ));
        min_distance = min(distances);
        min_distance_index = find(distances==min_distance);
        distances(min_distance_index) = [];
        second_min_distance = min(distances);
        if (min_distance * 1.3 < second_min_distance)
            matches_index = [matches_index ; [ j min_distance_index(1)]];
        end
    end
    
    p1 = kpt1(matches_index(:,1),:);
    p2 = kpt2(matches_index(:,2),:);
end

