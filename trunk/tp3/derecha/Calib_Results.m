% Intrinsic and Extrinsic Camera Parameters
%
% This script file can be directly excecuted under Matlab to recover the camera intrinsic and extrinsic parameters.
% IMPORTANT: This file contains neither the structure of the calibration objects nor the image coordinates of the calibration points.
%            All those complementary variables are saved in the complete matlab data file Calib_Results.mat.
% For more information regarding the calibration model visit http://www.vision.caltech.edu/bouguetj/calib_doc/


%-- Focal length:
fc = [ 863.723639247620440 ; 863.143777442792725 ];

%-- Principal point:
cc = [ 324.525065195263437 ; 198.241050474855655 ];

%-- Skew coefficient:
alpha_c = 0.000000000000000;

%-- Distortion coefficients:
kc = [ -0.127129530642451 ; 0.234903021616571 ; -0.001594184997822 ; -0.001008790458472 ; 0.000000000000000 ];

%-- Focal length uncertainty:
fc_error = [ 3.421582283923981 ; 3.412325685837134 ];

%-- Principal point uncertainty:
cc_error = [ 3.456568216531628 ; 3.336833276389630 ];

%-- Skew coefficient uncertainty:
alpha_c_error = 0.000000000000000;

%-- Distortion coefficients uncertainty:
kc_error = [ 0.013868921886144 ; 0.098639443889617 ; 0.001039398027209 ; 0.001082662572845 ; 0.000000000000000 ];

%-- Image size:
nx = 640;
ny = 480;


%-- Various other variables (may be ignored if you do not use the Matlab Calibration Toolbox):
%-- Those variables are used to control which intrinsic parameters should be optimized

n_ima = 20;						% Number of calibration images
est_fc = [ 1 ; 1 ];					% Estimation indicator of the two focal variables
est_aspect_ratio = 1;				% Estimation indicator of the aspect ratio fc(2)/fc(1)
center_optim = 1;					% Estimation indicator of the principal point
est_alpha = 0;						% Estimation indicator of the skew coefficient
est_dist = [ 1 ; 1 ; 1 ; 1 ; 0 ];	% Estimation indicator of the distortion coefficients


%-- Extrinsic parameters:
%-- The rotation (omc_kk) and the translation (Tc_kk) vectors for every calibration image and their uncertainties

%-- Image #1:
omc_1 = [ -2.091394e+00 ; -2.148468e+00 ; 5.118164e-03 ];
Tc_1  = [ -2.112245e+02 ; -1.137267e+02 ; 7.265132e+02 ];
omc_error_1 = [ 3.087070e-03 ; 3.089840e-03 ; 6.615792e-03 ];
Tc_error_1  = [ 2.909898e+00 ; 2.860758e+00 ; 3.128519e+00 ];

%-- Image #2:
omc_2 = [ -2.071558e+00 ; -2.159435e+00 ; 3.308251e-02 ];
Tc_2  = [ -2.135501e+02 ; -1.185427e+02 ; 7.371370e+02 ];
omc_error_2 = [ 3.110136e-03 ; 3.085049e-03 ; 6.624564e-03 ];
Tc_error_2  = [ 2.951001e+00 ; 2.902114e+00 ; 3.167206e+00 ];

%-- Image #3:
omc_3 = [ NaN ; NaN ; NaN ];
Tc_3  = [ NaN ; NaN ; NaN ];
omc_error_3 = [ NaN ; NaN ; NaN ];
Tc_error_3  = [ NaN ; NaN ; NaN ];

%-- Image #4:
omc_4 = [ 1.712426e+00 ; 1.861318e+00 ; 2.287144e-01 ];
Tc_4  = [ -2.329147e+02 ; -1.521012e+02 ; 9.965014e+02 ];
omc_error_4 = [ 2.973859e-03 ; 3.159451e-03 ; 5.663000e-03 ];
Tc_error_4  = [ 4.009228e+00 ; 3.905318e+00 ; 4.334691e+00 ];

%-- Image #5:
omc_5 = [ -1.908205e+00 ; -1.789979e+00 ; -4.461313e-01 ];
Tc_5  = [ -1.902788e+02 ; -1.532758e+02 ; 9.495201e+02 ];
omc_error_5 = [ 2.937405e-03 ; 3.308523e-03 ; 5.629193e-03 ];
Tc_error_5  = [ 3.822865e+00 ; 3.717569e+00 ; 4.182436e+00 ];

%-- Image #6:
omc_6 = [ 1.769870e+00 ; 1.905045e+00 ; -7.673868e-01 ];
Tc_6  = [ -2.169935e+02 ; -4.030857e+01 ; 1.117683e+03 ];
omc_error_6 = [ 2.335651e-03 ; 3.677200e-03 ; 5.682695e-03 ];
Tc_error_6  = [ 4.455358e+00 ; 4.364103e+00 ; 4.183465e+00 ];

%-- Image #7:
omc_7 = [ NaN ; NaN ; NaN ];
Tc_7  = [ NaN ; NaN ; NaN ];
omc_error_7 = [ NaN ; NaN ; NaN ];
Tc_error_7  = [ NaN ; NaN ; NaN ];

%-- Image #8:
omc_8 = [ 1.976334e+00 ; 1.132788e+00 ; -2.376670e-01 ];
Tc_8  = [ -3.125254e+02 ; -3.802623e+01 ; 1.018322e+03 ];
omc_error_8 = [ 3.334565e-03 ; 2.887186e-03 ; 4.967760e-03 ];
Tc_error_8  = [ 4.083358e+00 ; 4.020464e+00 ; 4.391292e+00 ];

%-- Image #9:
omc_9 = [ 2.091753e+00 ; 1.831746e+00 ; 3.230489e-01 ];
Tc_9  = [ -2.095201e+02 ; -1.251356e+02 ; 7.548381e+02 ];
omc_error_9 = [ 3.269575e-03 ; 2.614016e-03 ; 6.294470e-03 ];
Tc_error_9  = [ 3.048498e+00 ; 2.968454e+00 ; 3.400429e+00 ];

%-- Image #10:
omc_10 = [ 2.092256e+00 ; 1.830373e+00 ; 3.104592e-01 ];
Tc_10  = [ -2.148865e+02 ; -1.306923e+02 ; 7.568496e+02 ];
omc_error_10 = [ 3.239669e-03 ; 2.645481e-03 ; 6.304424e-03 ];
Tc_error_10  = [ 3.059472e+00 ; 2.978545e+00 ; 3.412444e+00 ];

%-- Image #11:
omc_11 = [ NaN ; NaN ; NaN ];
Tc_11  = [ NaN ; NaN ; NaN ];
omc_error_11 = [ NaN ; NaN ; NaN ];
Tc_error_11  = [ NaN ; NaN ; NaN ];

%-- Image #12:
omc_12 = [ NaN ; NaN ; NaN ];
Tc_12  = [ NaN ; NaN ; NaN ];
omc_error_12 = [ NaN ; NaN ; NaN ];
Tc_error_12  = [ NaN ; NaN ; NaN ];

%-- Image #13:
omc_13 = [ 2.058853e+00 ; 1.999782e+00 ; -3.776696e-02 ];
Tc_13  = [ -3.534880e+02 ; -1.318458e+02 ; 1.153001e+03 ];
omc_error_13 = [ 3.781236e-03 ; 4.478217e-03 ; 8.911560e-03 ];
Tc_error_13  = [ 4.639281e+00 ; 4.569089e+00 ; 5.059127e+00 ];

%-- Image #14:
omc_14 = [ 4.315566e-02 ; 2.994571e+00 ; -3.600320e-01 ];
Tc_14  = [ -4.384710e+01 ; -4.465793e+01 ; 1.174128e+03 ];
omc_error_14 = [ 1.235745e-03 ; 6.246153e-03 ; 9.355354e-03 ];
Tc_error_14  = [ 4.691851e+00 ; 4.520442e+00 ; 4.817270e+00 ];

%-- Image #15:
omc_15 = [ 2.093853e+00 ; 2.073996e+00 ; -2.324166e-01 ];
Tc_15  = [ -7.770860e+01 ; -7.546985e+01 ; 1.049969e+03 ];
omc_error_15 = [ 3.881952e-03 ; 3.573709e-03 ; 8.416726e-03 ];
Tc_error_15  = [ 4.207214e+00 ; 4.046495e+00 ; 4.224531e+00 ];

%-- Image #16:
omc_16 = [ 2.133776e+00 ; 2.173489e+00 ; -4.234800e-01 ];
Tc_16  = [ -2.219524e+01 ; -2.148508e+02 ; 1.137456e+03 ];
omc_error_16 = [ 3.547463e-03 ; 3.871136e-03 ; 8.156602e-03 ];
Tc_error_16  = [ 4.597415e+00 ; 4.398346e+00 ; 4.477390e+00 ];

%-- Image #17:
omc_17 = [ -1.918968e+00 ; -1.801910e+00 ; -3.948296e-01 ];
Tc_17  = [ -1.843779e+02 ; -1.715524e+02 ; 9.850159e+02 ];
omc_error_17 = [ 2.985321e-03 ; 3.283020e-03 ; 5.704239e-03 ];
Tc_error_17  = [ 3.969513e+00 ; 3.856742e+00 ; 4.307749e+00 ];

%-- Image #18:
omc_18 = [ NaN ; NaN ; NaN ];
Tc_18  = [ NaN ; NaN ; NaN ];
omc_error_18 = [ NaN ; NaN ; NaN ];
Tc_error_18  = [ NaN ; NaN ; NaN ];

%-- Image #19:
omc_19 = [ NaN ; NaN ; NaN ];
Tc_19  = [ NaN ; NaN ; NaN ];
omc_error_19 = [ NaN ; NaN ; NaN ];
Tc_error_19  = [ NaN ; NaN ; NaN ];

%-- Image #20:
omc_20 = [ -2.619298e+00 ; -1.344708e+00 ; 1.614361e-01 ];
Tc_20  = [ -3.334572e+02 ; -9.293298e+00 ; 1.092972e+03 ];
omc_error_20 = [ 4.573646e-03 ; 2.025370e-03 ; 7.485413e-03 ];
Tc_error_20  = [ 4.384825e+00 ; 4.307685e+00 ; 4.584964e+00 ];

