%display images

clear;
load('yale_data.mat');
number_subjects = 15;
number_faces =11;

subject_faces = [];

for i=1:number_subjects
   %% 2.1
    index_of_subject_face = subject(i*11-10); %%%complete this 
    current_face = reshape(faces(:,index_of_subject_face),64,64);
    subject_faces = [subject_faces,current_face];
    
end
figure(1);
title("Concatenated Image of First Image per Person") 
saveas(gcf, "All_15_Images_Concatenated.jpg")
imagesc(subject_faces);
axis equal;
axis tight;
colormap(gray);

%% 2.2
C = get_covar_matrix(faces);

%% 2.3
[V, D] = eigs(C, 10);

%% 2.4
eigenfaces = [];
for i = 1:10
    current_face = reshape(V(:, i), 64, 64);
    eigenfaces = [eigenfaces,current_face];
end

figure(2);
title("Eigenvector Images Corresponding to 10 Largest Eigenvalues") 
saveas(gcf, "Top_10_Images_Concatenated.jpg")
imagesc(eigenfaces);
axis equal;
axis tight;
colormap(gray);

%% 2.5
[V, D] = eigs(C, 20);
eigenfaces = [];
for i = 11:20
    current_face = reshape(V(:, i), 64, 64);
    eigenfaces = [eigenfaces, current_face];
end
figure(3);
title("Eigenvector Images Corresponding to Next 10 Largest Eigenvalues") 
saveas(gcf, "Second_10_Images_Concatenated.jpg")
imagesc(eigenfaces);
axis equal;
axis tight;
colormap(gray);

%% 3.1
subject_a = 7;
subject_b = 5;
vis_faces_a = [];
vis_faces_b = [];
faces_subject_a = faces(:, 67:77);
faces_subject_b = faces(:, 45:55);

for i=1:number_faces % 1 thru 11
    current_face = reshape(faces_subject_a(:, i), 64, 64);
    vis_faces_a = [vis_faces_a, current_face];

    current_face = reshape(faces_subject_b(:, i), 64, 64);
    vis_faces_b = [vis_faces_b, current_face];
end
figure(4);
title("All Images of Subject 7") 
saveas(gcf, "Subject7_Concatenated.jpg")

faces_subject_ab = [faces_subject_a, faces_subject_b];
vis_faces_ab = [vis_faces_a; vis_faces_b];

imagesc(vis_faces_ab);
axis equal;
axis tight;
colormap(gray);

%% 3.2
Ca = get_covar_matrix(faces_subject_a);
Cb = get_covar_matrix(faces_subject_b);
Cab = get_covar_matrix(faces_subject_ab);

%% 3.3
[Va, Da] = eigs(Ca, 5);
[Vb, Db] = eigs(Cb, 5);
[Vab, Dab] = eigs(Cab, 5);

eigenfaces = [];
eigenfaces_5a = [];
eigenfaces_5b = [];
eigenfaces_5ab = [];
for i = 1:5
    current_face = reshape(Va(:, i), 64, 64);
    eigenfaces_5a = [eigenfaces_5a, current_face];

    current_face = reshape(Vb(:, i), 64, 64);
    eigenfaces_5b = [eigenfaces_5b, current_face];

    current_face = reshape(Vab(:, i), 64, 64);
    eigenfaces_5ab = [eigenfaces_5ab, current_face];
end

figure(5);
title("Top 5 Eigenvectors")
saveas(gcf, "Top5_Concatenated.jpg")
eigenfaces = [eigenfaces_5a; eigenfaces_5b; eigenfaces_5ab];
imagesc(eigenfaces);
axis equal;
axis tight;
colormap(gray);

close all;