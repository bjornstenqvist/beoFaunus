#include "colors.inc"

#declare locX=16.349;
#declare locY=16.349;
#declare locZ=16.349;

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
<6.23654,-6.54777,3.81614>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<5.66083,-7.36542,3.81614>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<6.81226,-5.73013,3.81614>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<2.06064,-4.85195,0.969089>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<1.39839,-4.10267,0.969089>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<2.7229,-5.60122,0.969089>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-5.49468,-1.26661,-0.444728>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-4.73371,-0.617822,-0.444728>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-6.25564,-1.9154,-0.444728>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-7.74781,0.496668,1.57741>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-8.60515,-0.0180944,1.57741>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-6.89048,1.01143,1.57741>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<0.938117,-3.16911,0.978395>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<0.491192,-2.27454,0.978395>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<1.38504,-4.06368,0.978395>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<0.0259442,-1.99941,2.07375>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-0.864675,-1.54466,2.07375>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<0.916564,-2.45416,2.07375>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-4.40162,-4.92857,-1.85112>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-5.37778,-4.71149,-1.85112>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-3.42547,-5.14566,-1.85112>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-4.32474,-3.01132,-0.518749>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-3.43773,-3.47308,-0.518749>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-5.21174,-2.54955,-0.518749>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-3.57003,-7.37723,-4.78772>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-4.3662,-6.77214,-4.78772>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-2.77387,-7.98231,-4.78772>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-5.63512,-4.95007,-0.509248>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-4.91184,-5.64064,-0.509248>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-6.35839,-4.25951,-0.509248>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<2.96704,-3.97715,4.84201>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<3.96587,-4.0255,4.84201>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<1.96821,-3.9288,4.84201>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-4.36586,-1.54961,-0.424422>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-4.69105,-0.603967,-0.424422>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-4.04066,-2.49526,-0.424422>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<2.4761,-0.21916,1.43545>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<1.76874,0.487694,1.43545>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<3.18346,-0.926014,1.43545>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-4.82859,2.59668,-0.120522>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-4.83193,3.59667,-0.120522>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-4.82525,1.59668,-0.120522>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<3.39138,-0.305714,2.58052>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<2.39705,-0.19943,2.58052>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<4.38572,-0.411998,2.58052>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<4.3209,-4.96391,4.83447>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<4.66779,-5.90181,4.83447>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<3.97401,-4.026,4.83447>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<3.91979,-5.70899,5.96459>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<4.88514,-5.44802,5.96459>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<2.95445,-5.96997,5.96459>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<5.57746,-0.609015,0.493543>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<4.80285,0.0234277,0.493543>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<6.35206,-1.24146,0.493543>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<5.89422,-5.19368,4.9348>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<6.55928,-5.94047,4.9348>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<5.22916,-4.4469,4.9348>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<5.96608,-6.752,4.92777>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<5.36688,-7.55261,4.92777>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<6.56527,-5.9514,4.92777>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-5.12763,0.264237,1.59869>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-4.70656,-0.642791,1.59869>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-5.5487,1.17126,1.59869>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-5.56643,-0.0329954,-0.436351>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-6.40342,0.514221,-0.436351>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-4.72944,-0.580212,-0.436351>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<5.23306,-0.564529,1.57049>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<6.04338,-1.15051,1.57049>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<4.42274,0.0214567,1.57049>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<1.63926,-1.73455,2.07444>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<2.3461,-1.02717,2.07444>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<0.932419,-2.44192,2.07444>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<3.18557,-1.94852,1.44278>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<3.17469,-2.94846,1.44278>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<3.19645,-0.94858,1.44278>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<5.10648,-5.89076,7.18717>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<5.98686,-6.36502,7.18717>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<4.22609,-5.4165,7.18717>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-5.06631,-3.76541,-1.84427>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-4.73774,-2.82093,-1.84427>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-5.39488,-4.70989,-1.84427>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<6.89744,-0.593636,1.58112>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<7.72426,-0.0311657,1.58112>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<6.07063,-1.15611,1.58112>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<3.53701,-4.05071,3.61352>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<4.5369,-4.06528,3.61352>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<2.53711,-4.03613,3.61352>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<1.31934,-2.22994,0.856411>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<1.13787,-3.21334,0.856411>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<1.50081,-1.24655,0.856411>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<3.76315,-0.0195499,0.444289>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<2.76318,-0.0274423,0.444289>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<4.76312,-0.0116575,0.444289>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-7.89661,-1.11476,1.47052>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-7.26748,-0.337461,1.47052>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-8.52574,-1.89206,1.47052>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-6.30733,1.52671,-0.434405>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-6.2023,2.52118,-0.434405>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-6.41236,0.53224,-0.434405>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<8.14757,-1.5551,0.27919>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<7.8213,-2.50038,0.27919>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<8.47384,-0.609825,0.27919>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<1.25615,-3.40853,2.07725>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<1.59612,-4.34897,2.07725>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<0.916186,-2.46809,2.07725>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<4.92115,0.421542,1.72506>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<3.93199,0.274687,1.72506>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<5.91031,0.568396,1.72506>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<2.61714,-0.914545,0.273794>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<3.04966,-1.81617,0.273794>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<2.18462,-0.0129192,0.273794>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<5.89685,-7.72579,-7.89447>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<4.99815,-7.28722,-7.89447>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<6.79554,-8.16437,-7.89447>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<7.5401,-0.221435,0.273126>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<8.46837,-0.593344,0.273126>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<6.61183,0.150474,0.273126>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<5.71504,-6.59863,-7.89799>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<6.44663,-5.91688,-7.89799>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<4.98346,-7.28038,-7.89799>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-6.4321,0.573611,2.80789>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-6.90659,1.45388,2.80789>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-5.95762,-0.306655,2.80789>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-6.3426,-4.36527,-1.83608>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-7.28399,-4.02794,-1.83608>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-5.40121,-4.7026,-1.83608>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-4.94482,-0.200576,2.80254>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-5.93698,-0.32558,2.80254>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-3.95266,-0.0755724,2.80254>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<4.45471,-4.5439,5.97411>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<4.01502,-3.64575,5.97411>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<4.89439,-5.44205,5.97411>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<3.83171,-1.54835,2.87103>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<4.71243,-2.022,2.87103>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<2.951,-1.0747,2.87103>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-3.02416,7.38142,-4.78541>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-3.2785,6.4143,-4.78541>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-2.76983,8.34853,-4.78541>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<1.38648,-0.77173,2.75477>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<2.19742,-0.186611,2.75477>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<0.575528,-1.35685,2.75477>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<2.94149,-3.12162,2.75602>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<3.19948,-4.08777,2.75602>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<2.6835,-2.15547,2.75602>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<6.55166,7.19616,-7.91258>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<6.80863,8.16258,-7.91258>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<6.29468,6.22974,-7.91258>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-5.44631,-1.37946,1.57913>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-6.18168,-2.05712,1.57913>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-4.71093,-0.701804,1.57913>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-5.80435,-3.41,-0.513716>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-6.35359,-4.24566,-0.513716>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-5.25511,-2.57433,-0.513716>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<4.36597,-6.48126,-7.90046>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<3.75588,-5.68893,-7.90046>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<4.97607,-7.27359,-7.90046>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<5.5463,-2.03084,1.57024>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<6.05251,-1.16844,1.57024>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<5.04009,-2.89325,1.57024>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<2.45036,-1.16702,2.75893>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<2.67633,-2.14116,2.75893>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<2.22438,-0.19289,2.75893>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-7.12821,-3.58131,-0.513923>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-6.37971,-4.24445,-0.513923>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-7.8767,-2.91817,-0.513923>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-6.86056,-0.477528,0.275903>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-5.86659,-0.367836,0.275903>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-7.85452,-0.587219,0.275903>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<5.80249,-5.90378,5.96946>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<4.9105,-5.45174,5.96946>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<6.69449,-6.35582,5.96946>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-4.15359,-1.50885,1.57448>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-4.70511,-0.674685,1.57448>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-3.60207,-2.34301,1.57448>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<7.55601,-5.67292,4.92933>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<6.59296,-5.94223,4.92933>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<8.51907,-5.40362,4.92933>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<5.26471,-0.919598,2.58043>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<6.1232,-1.43243,2.58043>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<4.40622,-0.406771,2.58043>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<7.2294,-2.74195,1.48069>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<7.77895,-1.9065,1.48069>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<6.67984,-3.57741,1.48069>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-5.30763,-2.58419,1.57009>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-6.1711,-2.07979,1.57009>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-4.44415,-3.08858,1.57009>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-6.30844,-2.22338,0.210911>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-7.27001,-2.49795,0.210911>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-5.34687,-1.94882,0.210911>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-2.92034,-7.55931,-3.56686>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-3.70467,-6.93895,-3.56686>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-2.13602,-8.17966,-3.56686>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-1.94488,-7.41348,-4.78896>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-1.12308,-6.84372,-4.78896>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-2.76669,-7.98324,-4.78896>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-7.74246,-2.48964,1.48737>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-6.93057,-3.07345,1.48737>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-8.55434,-1.90582,1.48737>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

sphere {
<-4.71435,-0.593029,-0.423658>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-4.8305,3.60072,-0.121555>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<3.97124,-4.02152,4.83867>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<6.80694,8.1655,-7.9113>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<4.39742,-0.410921,2.58207>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<5.65551,-7.3693,3.8146>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-6.37164,-4.25123,-0.51>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<1.39365,-4.09955,0.967942>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<6.57833,-5.94838,4.92279>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<3.19274,-0.939903,1.44183>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<7.78683,-1.90121,1.48536>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<2.68108,-2.1469,2.76027>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-6.18335,-2.0664,1.58093>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<7.73047,-0.024683,1.57684>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<1.50131,-1.24204,0.855363>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-6.20633,2.5714,-0.400428>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<4.89631,-5.44811,5.9726>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<0.91873,-2.45492,2.07356>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-5.22148,-2.54268,-0.521543>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<5.98953,-6.36657,7.18872>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<4.98114,-7.28167,-7.89968>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-6.88717,1.01516,1.57628>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-4.70306,-0.651394,1.60365>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-7.87494,-0.597523,0.277227>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-5.95613,-0.322785,2.8028>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-2.7666,-7.9927,-4.78398>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-3.71656,-6.91597,-3.56701>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<4.79518,0.0248258,0.485056>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-7.30199,-2.49921,0.212032>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<6.05854,-1.15803,1.57429>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<2.5321,-4.03575,3.61273>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<2.22115,-0.173144,2.76251>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-6.41135,0.518763,-0.43279>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-5.39656,-4.7124,-1.84358>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
