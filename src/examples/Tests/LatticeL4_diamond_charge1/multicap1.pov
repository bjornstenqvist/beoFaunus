#include "colors.inc"

#declare locX=12.9962;
#declare locY=12.9962;
#declare locZ=12.9962;

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
<4.82399,2.76326,0.446569>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<4.3595,3.09629,0.0424241>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<5.28848,2.43022,0.0424241>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<4.49095,2.29876,0.850714>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<5.15702,3.22775,0.850714>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<6.48282,5.61356,-5.93998>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<6.55922,5.04714,-6.34413>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<6.40642,6.17997,-6.34413>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<7.04924,5.68996,-5.53584>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<5.9164,5.53716,-5.53584>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<6.05669,6.10268,-4.71795>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<6.62291,6.18051,-5.1221>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<5.49046,6.02485,-5.1221>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<5.97886,6.66891,-4.31381>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<6.13452,5.53646,-4.31381>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-2.85771,3.39488,2.04044>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-2.39115,3.06475,1.6363>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-3.32428,3.725,1.6363>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-2.52759,3.86145,2.44459>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-3.18784,2.92831,2.44459>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<4.72264,4.43834,5.9223>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<4.32165,4.84562,5.51816>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<5.12363,4.03107,5.51816>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<4.31536,4.03736,6.32645>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<5.12992,4.83933,6.32645>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<0.444737,-0.0172349,3.06899>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<0.767387,-0.489002,2.66484>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<0.122086,0.454532,2.66484>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<0.916503,0.305415,3.47313>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-0.0270299,-0.339885,3.47313>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<5.8895,3.17694,-5.29041>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<6.45444,3.26363,-5.69456>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<5.32457,3.09026,-5.69456>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<5.80282,3.74188,-4.88627>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<5.97619,2.61201,-4.88627>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-6.34351,-3.72458,-3.61221>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-6.77892,-3.35432,-4.01636>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-5.9081,-4.09483,-4.01636>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-6.71376,-4.15998,-3.20807>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-5.97326,-3.28917,-3.20807>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-2.88475,3.22648,-2.62749>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-2.33248,3.37368,-3.03163>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-3.43701,3.07928,-3.03163>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-3.03194,3.77875,-2.22334>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-2.73755,2.67421,-2.22334>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-2.43132,3.90428,-1.74641>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-1.87588,4.03901,-2.15056>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-2.98676,3.76956,-2.15056>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-2.56605,4.45973,-1.34227>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-2.29659,3.34884,-1.34227>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<6.04851,5.7839,0.0759115>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<5.47956,5.72944,-0.328233>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<6.61746,5.83835,-0.328233>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<6.10297,5.21495,0.480056>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<5.99405,6.35284,0.480056>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-2.6906,4.92977,0.328152>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-3.07108,5.35627,-0.0759928>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-2.31012,4.50327,-0.0759928>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-3.1171,4.54929,0.732297>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-2.2641,5.31025,0.732297>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<6.03975,-6.33419,6.21575>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<6.43707,-5.92333,5.81161>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<5.64244,-6.74505,5.81161>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<5.62889,-5.93688,6.6199>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<6.45062,-6.73151,6.6199>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-0.746439,-1.4384,0.730286>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-0.683885,-2.00652,0.326141>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-0.808992,-0.870289,0.326141>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-0.178324,-1.37585,1.13443>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-1.31455,-1.50096,1.13443>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<5.80271,3.42947,1.26229>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<6.33206,3.64501,0.858144>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<5.27336,3.21394,0.858144>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<5.58717,3.95882,1.66643>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<6.01824,2.90012,1.66643>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-2.73054,4.10717,1.15125>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-2.33408,3.69548,0.747106>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-3.127,4.51885,0.747106>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-2.31885,4.50363,1.5554>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-3.14222,3.71071,1.5554>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-4.48973,1.54103,0.433557>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-4.93312,1.18038,0.0294125>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-4.04634,1.90169,0.0294125>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-4.12908,1.09764,0.837702>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-4.85038,1.98442,0.837702>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<4.8344,3.82836,-6.22424>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<4.3018,4.03574,-6.62838>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<5.367,3.62098,-6.62838>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<4.62703,3.29576,-5.82009>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<5.04178,4.36096,-5.82009>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-5.69621,3.53082,-5.1751>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-6.2353,3.72068,-5.57925>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-5.15712,3.34095,-5.57925>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-5.88608,2.99173,-4.77096>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-5.50635,4.06991,-4.77096>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-1.9441,-3.56319,-4.08975>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-1.42901,-3.81089,-4.4939>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-2.45918,-3.31549,-4.4939>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-1.6964,-3.04811,-3.68561>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-2.1918,-4.07828,-3.68561>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-4.4563,-1.1023,-2.05553>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-4.10131,-0.654367,-2.45967>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-4.81129,-1.55024,-2.45967>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-4.90424,-0.747313,-1.65138>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-4.00836,-1.4573,-1.65138>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<6.03433,-6.34464,4.1158>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<5.73061,-6.82881,3.71166>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<6.33805,-5.86047,3.71166>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<6.5185,-6.64836,4.51995>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<5.55016,-6.04092,4.51995>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<0.709108,1.60653,-0.57018>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<1.24229,1.40064,-0.974325>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<0.175931,1.81242,-0.974325>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<0.914995,2.13971,-0.166035>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<0.503221,1.07335,-0.166035>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-6.20308,2.88488,-4.11162>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-5.9321,2.38165,-4.51576>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-6.47406,3.3881,-4.51576>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-5.69986,3.15586,-3.70747>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-6.70631,2.6139,-3.70747>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<6.48717,-2.81167,-2.80847>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<6.0257,-2.47446,-3.21262>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<6.94865,-3.14888,-3.21262>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<6.14996,-3.27315,-2.40433>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<6.82438,-2.3502,-2.40433>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<5.62732,-1.12709,1.80077>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<5.28671,-0.668113,1.39662>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<5.96792,-1.58606,1.39662>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<5.16834,-1.46769,2.20491>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<6.08629,-0.786481,2.20491>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-3.7628,5.06096,-0.909458>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-3.73431,4.49012,-1.3136>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-3.79129,5.63179,-1.3136>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-3.19196,5.08945,-0.505313>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-4.33364,5.03246,-0.505313>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<5.83512,4.94167,6.14107>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<6.08006,4.42526,5.73693>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<5.59018,5.45807,5.73693>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<6.35152,5.18661,6.54522>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<5.31871,4.69673,6.54522>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-0.680891,-5.93631,-1.4147>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-0.52186,-5.38734,-1.81885>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-0.839922,-6.48529,-1.81885>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-1.22987,-5.77728,-1.01056>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-0.131914,-6.09535,-1.01056>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-2.59925,5.11011,-0.921531>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-2.6238,5.68113,-1.32568>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-2.5747,4.53909,-1.32568>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-3.17027,5.08556,-0.517386>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-2.02823,5.13466,-0.517386>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-4.04293,1.85883,-0.890744>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-4.60406,1.75019,-1.29489>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-3.4818,1.96747,-1.29489>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-3.93429,1.29771,-0.486599>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-4.15157,2.41996,-0.486599>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<1.60635,-0.20253,3.19962>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<2.04737,-0.566074,2.79547>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<1.16532,0.161013,2.79547>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<1.96989,0.238495,3.60376>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<1.2428,-0.643555,3.60376>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<0.500473,-6.26899,3.60161>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<0.820618,-5.79552,3.19747>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<0.180328,-6.74246,3.19747>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<0.0270023,-5.94885,4.00575>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<0.973943,-6.58914,4.00575>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

sphere {
<6.13925,-3.28438,-2.39919>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<0.020749,-5.94706,4.00928>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<5.32376,3.09028,-5.69506>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<6.12874,-0.781243,2.21541>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<0.767581,-0.492284,2.66351>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-4.93799,1.1801,0.02686>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<0.174544,6.24878,3.18548>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-1.85019,4.02888,-2.1645>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-5.50484,4.07364,-4.76713>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-6.45319,5.01004,-6.34375>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<5.91458,5.53686,-5.53399>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<4.47918,2.30007,0.866845>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-2.19613,-4.08653,-3.65274>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<5.12824,4.01471,5.51283>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<2.06313,-0.566096,2.78003>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<5.97315,-1.59233,1.39054>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-6.16791,-2.34072,-2.40025>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-6.47314,3.40101,-4.52005>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-2.31872,4.50757,1.5586>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-2.56719,4.50027,-1.32336>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-1.68892,-3.03422,-3.68311>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<5.64064,6.24697,5.80719>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-2.73217,2.66808,-2.2194>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<0.110084,0.465779,2.66093>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-2.4713,-3.31161,-4.50223>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<6.48035,3.25101,-5.70492>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-3.18101,5.08677,-0.500986>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-3.19393,2.92358,2.44668>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-1.31771,-1.50121,1.13754>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<5.31117,4.68788,-6.43768>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-1.2396,-5.77687,-1.00903>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<1.9755,0.244129,3.60947>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<1.24474,1.39654,-0.97953>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<6.34041,-5.85868,3.71064>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-6.47121,6.34703,4.5267>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-5.96697,-3.27729,-3.19493>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-5.68932,3.16037,-3.70495>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<5.98625,6.36446,0.482531>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-4.00363,-1.45904,-1.64404>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-3.46341,1.98088,-1.31097>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-6.34894,6.17541,-5.12555>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-3.33412,3.73092,1.62701>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<4.2946,4.04409,6.35778>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<6.33654,3.64666,0.852172>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-4.33663,5.03368,-0.501931>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-4.62749,1.76599,-1.3057>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<0.168059,1.81255,-0.982157>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<6.44277,-5.90794,5.8105>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-2.30108,4.49557,-0.128427>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<6.27146,-4.1602,-3.1988>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-6.37752,5.83788,-0.331383>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<6.18962,-3.35316,-4.04847>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-2.62323,5.68575,-1.33322>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-4.8625,1.99313,0.846818>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-4.08821,-0.638538,-2.45628>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<5.16269,3.23351,0.854153>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-0.519633,-5.37061,-1.82892>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-3.03631,3.79405,-2.20071>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<6.44906,6.25423,-6.3671>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-0.119584,-6.10114,-1.00976>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-4.91146,-0.734403,-1.64038>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-0.682691,-2.01502,0.317538>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-4.12513,1.09299,0.838691>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-3.13901,4.52842,0.747033>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<6.03111,2.87129,1.6804>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<5.97699,-6.31768,-4.31175>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<5.45765,5.73055,-0.332211>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
