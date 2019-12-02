% testART
close all

I = phantom(200);

theta     = 0:1:179;
[r, c]    = size(I); 
NperTheta = r + c;

[A, b] = MakeCT(I, 0, 179, 2, NperTheta, 0.5);

% fileID = fopen('A.bin','w');
% fwrite(fileID, A, 'double');
% fclose(fileID);
% 
% fileID = fopen('b.bin','w');
% fwrite(fileID, b, 'double');
% fclose(fileID);