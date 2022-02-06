#include "colors.inc"

#declare locX=19.7449;
#declare locY=19.7449;
#declare locZ=19.7449;

#declare colorLock=color rgb<0.05, 0, 0.8>;
#declare colorLockCavity=color rgb<0.8, 0.05, 0>;
#declare colorKey=color rgb<0.75, 0.75, 0>;

#declare ambientv=0.3;
#declare phongv=0.1;

background { color rgb <0.0, 0.0, 0.0> }

camera {
location <locX,locY,locZ>
look_at <0,0,0>
}

light_source {
<locX,locY,locZ>
color White
}

difference {
sphere {
<1.29635,9.42884,1.46824>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<1.50462,8.45077,1.46824>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<1.08809,10.4069,1.46824>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<9.47931,-7.69158,-0.431138>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<8.84939,-6.91491,-0.431138>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<10.1092,-8.46825,-0.431138>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<6.16138,9.79238,-8.65406>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<5.70417,8.90302,-8.65406>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<6.61859,10.6817,-8.65406>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<1.66,-8.16465,-2.28894>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<0.989426,-8.90649,-2.28894>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<2.33058,-7.42281,-2.28894>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<1.89485,-0.153176,5.51137>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<2.88785,-0.271322,5.51137>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<0.901853,-0.0350289,5.51137>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<9.44398,-7.25671,-4.80269>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<10.0341,-6.4494,-4.80269>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<8.85385,-8.06402,-4.80269>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<7.69352,-3.50174,-5.51224>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<6.73915,-3.80036,-5.51224>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<8.64789,-3.20312,-5.51224>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<7.69707,-7.86469,-0.864955>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<7.1521,-8.70314,-0.864955>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<8.24205,-7.02623,-0.864955>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<8.35002,6.1044,-3.91446>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<9.0491,5.38935,-3.91446>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<7.65093,6.81944,-3.91446>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<8.68741,-9.59778,8.74823>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<9.41986,-8.91697,8.74823>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<7.95495,-10.2786,8.74823>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<0.598512,5.09417,4.00423>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<0.833018,6.06629,4.00423>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<0.364006,4.12206,4.00423>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-9.21315,-7.74953,1.45028>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-9.56777,-8.68454,1.45028>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-8.85852,-6.81452,1.45028>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<9.18883,-6.2718,0.555649>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<8.442,-5.60679,0.555649>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<9.93566,-6.93682,0.555649>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-9.27227,-8.81182,8.73132>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-8.27284,-8.77813,8.73132>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-10.2717,-8.84551,8.73132>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<9.36823,-6.44153,8.86709>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<10.2608,-5.99062,8.86709>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<8.47566,-6.89244,8.86709>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<4.6739,4.95377,-7.47424>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<5.17981,4.09118,-7.47424>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<4.16799,5.81636,-7.47424>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<0.252464,-0.844786,5.46205>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<0.878037,-0.0646211,5.46205>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-0.37311,-1.62495,5.46205>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<0.425654,-7.6702,-3.121>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<0.0763292,-6.7332,-3.121>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<0.774978,-8.6072,-3.121>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<8.63234,-4.54518,0.355053>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<8.47534,-5.53278,0.355053>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<8.78933,-3.55758,0.355053>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-5.5176,-3.95518,5.85928>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-5.53667,-2.95536,5.85928>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-5.49853,-4.955,5.85928>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<8.143,-9.47381,-9.36384>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<7.32664,-10.0513,-9.36384>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<8.95936,-8.89627,-9.36384>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-0.513126,-3.06963,4.45793>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-0.908745,-2.15122,4.45793>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-0.117507,-3.98804,4.45793>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<7.98483,-8.93241,-5.29798>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<8.28267,-9.88702,-5.29798>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<7.68698,-7.9778,-5.29798>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<3.23438,6.26819,-7.48105>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<4.14061,5.8454,-7.48105>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<2.32815,6.69098,-7.48105>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<9.69669,9.42576,7.83846>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<8.72136,9.20505,7.83846>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<10.672,9.64647,7.83846>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<8.64684,-2.55679,-8.14992>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<7.69168,-2.85288,-8.14992>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<9.60199,-2.26069,-8.14992>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<5.45788,9.81065,2.66314>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<4.50286,10.1072,2.66314>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<6.4129,9.51411,2.66314>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<6.59545,3.07532,0.822699>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<6.401,4.05623,0.822699>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<6.78989,2.0944,0.822699>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<8.31825,6.15826,-9.83506>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<7.42275,6.60334,-9.83506>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<9.21374,5.71319,-9.83506>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-4.62215,2.09612,7.0684>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-4.0354,1.28635,7.0684>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-5.2089,2.90589,7.0684>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-0.0150983,9.85123,1.89851>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<0.202332,8.87516,1.89851>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-0.232528,10.8273,1.89851>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<8.77867,-7.8851,8.88813>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<9.05751,-8.84544,8.88813>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<8.49984,-6.92476,8.88813>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-8.45216,-1.07399,3.30024>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-8.0631,-1.99521,3.30024>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-8.84122,-0.152782,3.30024>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<8.19314,4.20754,-3.01944>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<8.00822,5.19029,-3.01944>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<8.37807,3.22479,-3.01944>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-0.629942,1.81049,-2.29036>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-0.146524,0.935103,-2.29036>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-1.11336,2.68588,-2.29036>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<7.89945,2.11713,7.09778>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<7.07617,1.54949,7.09778>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<8.72272,2.68477,7.09778>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<2.9468,-9.55687,-1.57422>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<2.01536,-9.19295,-1.57422>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<3.87823,-9.92078,-1.57422>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-8.11552,-9.68798,9.63641>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-8.86083,-9.02126,9.63641>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-7.37021,-10.3547,9.63641>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-3.52517,0.0537732,-9.60948>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-2.60687,0.449657,-9.60948>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-4.44347,-0.342111,-9.60948>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<1.50201,-3.68363,-8.17213>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<0.863407,-2.91409,-8.17213>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<2.14061,-4.45317,-8.17213>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<6.47436,-2.3181,-4.42193>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<6.63838,-1.33164,-4.42193>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<6.31033,-3.30455,-4.42193>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<7.23493,-4.06417,-0.910318>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<8.16275,-4.4372,-0.910318>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<6.30711,-3.69114,-0.910318>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<0.556154,-8.48025,-9.56712>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-0.207032,-7.83407,-9.56712>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<1.31934,-9.12643,-9.56712>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<8.05199,-6.85793,0.442995>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<7.08298,-7.10493,0.442995>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<9.02101,-6.61093,0.442995>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<3.5119,9.31241,3.20343>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<2.53367,9.51996,3.20343>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<4.49012,9.10486,3.20343>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<5.58472,-5.0189,-7.57885>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<5.34275,-4.04862,-7.57885>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<5.82668,-5.98919,-7.57885>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-3.88709,5.36316,-0.472035>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-3.18596,4.65013,-0.472035>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-4.58822,6.0762,-0.472035>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<1.7613,-9.02662,-3.22427>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<0.824664,-8.67632,-3.22427>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<2.69794,-9.37693,-3.22427>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-6.81756,-2.37586,-3.72776>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-5.85059,-2.12098,-3.72776>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-7.78453,-2.63075,-3.72776>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<9.43601,-4.08764,-0.943899>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<9.86331,-3.18353,-0.943899>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<9.00871,-4.99175,-0.943899>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

sphere {
<6.39804,4.07334,0.824031>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-1.73764,-7.48024,-6.46629>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<7.62305,6.81247,-3.9088>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-1.11614,2.68649,-2.2914>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<6.44308,-8.99791,2.70457>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<2.5218,9.52247,3.19213>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<0.886669,-0.0430938,5.49572>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<5.34303,-4.0393,-7.592>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<1.0927,-9.32087,1.45791>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-7.36733,9.3868,9.63531>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<8.24593,-7.01705,-0.865902>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<1.18186,-6.94156,-2.2819>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-4.59334,6.08359,-0.476953>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<5.53234,8.96486,1.66676>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<2.0126,-9.19196,-1.57075>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<2.889,-9.15894,-4.00091>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<8.41585,-5.60319,0.526015>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<9.21963,5.71198,-9.83444>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-0.0280951,6.11422,1.962>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<8.71673,9.20058,7.83934>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<9.86751,-3.17924,-0.944581>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<3.90434,9.79074,-1.58502>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-8.6153,6.77243,-0.936971>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<4.16184,5.82025,-7.47572>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<6.63629,-1.32285,-4.42487>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-5.27571,-7.77023,4.3313>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<0.779607,-8.6144,-3.12147>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-7.25896,-0.331427,0.551215>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<9.43416,-8.8972,8.74809>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<8.47018,-6.89915,8.87082>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<1.16702,-3.95912,-6.93632>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-5.67912,4.34759,-8.55161>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<5.70261,8.89886,-8.65488>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-3.65863,1.30865,-9.23685>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<8.18224,-4.43078,-0.907249>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<4.50638,9.0952,3.20905>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<1.32721,-9.12757,-9.56686>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<7.99121,5.20602,-3.03825>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<0.198272,8.85733,1.90507>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<8.8473,-8.07038,-4.80142>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<8.16815,-5.30087,9.78572>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<0.190015,-3.59221,3.4619>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-1.6786,-2.95584,3.8731>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<7.30028,9.68506,-9.35637>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-7.61745,-1.4428,-4.16231>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-9.78556,-6.94014,0.562524>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<7.6939,-2.78749,-4.33711>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<2.31367,6.6943,-7.4829>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-8.25659,-8.75886,8.73209>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<0.346342,-2.24012,5.22773>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
