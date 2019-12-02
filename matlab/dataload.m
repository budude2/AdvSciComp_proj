iterations = 3;
rows_a = 36200;
cols_a = 40000;
r = 200;
c = 200;

fileID = fopen('x.bin','r');
x = fread(fileID, 'double');
fclose(fileID);

x = reshape(x, cols_a, iterations + 1);
imshow(mat2gray(reshape(x(:, iterations + 1), r, c)))