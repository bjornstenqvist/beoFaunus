#include "colors.inc"

#declare locX=16.6174;
#declare locY=16.6174;
#declare locZ=16.6174;

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
<-6.43774,1.27326,3.32261>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-7.27136,0.934037,3.32261>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-5.72715,0.720931,3.32261>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-6.3147,2.16481,3.32261>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-7.85217,0.219078,3.32173>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-8.74005,0.366257,3.32173>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-7.53569,-0.623442,3.32173>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-7.28077,0.914418,3.32173>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<5.18315,-3.8216,-3.0828>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<5.42469,-4.68858,-3.0828>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<5.81322,-3.17893,-3.0828>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<4.31156,-3.59728,-3.0828>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<8.08615,-4.55102,3.24216>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<8.98596,-4.53222,3.24216>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<7.61997,-3.78116,3.24216>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<7.65253,-5.33967,3.24216>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-6.89524,-3.11709,-1.94843>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-6.03215,-3.37219,-1.94843>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-7.10586,-2.24208,-1.94843>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-7.54771,-3.73699,-1.94843>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-7.41932,-0.553678,8.21832>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-7.79409,-1.37193,8.21832>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-6.5233,-0.469119,8.21832>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-7.94055,0.180017,8.21832>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<0.100808,-5.33306,2.3806>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<0.215857,-6.22568,2.3806>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<0.816311,-4.78712,2.3806>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-0.729745,-4.98639,2.3806>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<1.87878,-5.05278,2.51789>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<0.981374,-4.98451,2.51789>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<2.26836,-5.86409,2.51789>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<2.38661,-4.30974,2.51789>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-7.78739,1.59687,2.45066>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-8.62262,1.93212,2.45066>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-7.66011,0.705912,2.45066>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-7.07944,2.15257,2.45066>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-7.09915,7.4207,1.43012>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-6.73625,6.59711,1.43012>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-6.56735,8.14678,1.43012>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-7.99385,7.51822,1.43012>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-5.48465,-1.96138,2.79968>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-4.67179,-2.34773,2.79968>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-5.5565,-1.06426,2.79968>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-6.22567,-2.47217,2.79968>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<7.73264,-7.59958,1.19986>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<7.46746,-6.73953,1.19986>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<7.1204,-8.25925,1.19986>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<8.61005,-7.79996,1.19986>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<3.36392,-5.72929,2.33328>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<4.05509,-6.30573,2.33328>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<3.51754,-4.8425,2.33328>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<2.51912,-6.03965,2.33328>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-0.238885,1.775,7.13945>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<0.657593,1.69546,7.13945>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-0.618239,2.59115,7.13945>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-0.756009,1.0384,7.13945>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<7.29316,-5.31315,-1.98906>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<6.39355,-5.28674,-1.98906>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<7.7201,-6.10544,-1.98906>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<7.76584,-4.54727,-1.98906>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<6.67121,-6.99601,2.06222>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<6.42279,-6.13098,2.06222>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<6.04627,-7.64367,2.06222>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<7.54456,-7.21339,2.06222>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<2.04193,-2.72979,-4.1347>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<1.25028,-3.15791,-4.1347>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<2.80852,-3.20132,-4.1347>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<2.06699,-1.83014,-4.1347>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<7.2999,-5.88417,2.06207>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<6.43346,-6.12765,2.06207>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<7.94397,-6.5128,2.06207>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<7.52227,-5.01208,2.06207>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-5.67842,-8.19836,1.43905>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-6.53749,-8.46669,1.43905>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-5.01651,-8.80816,1.43905>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-5.48127,-7.32022,1.43905>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<1.43151,-5.6892,3.41342>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<1.99902,-6.38772,3.41342>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<1.75269,-4.84846,3.41342>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<0.542819,-5.83142,3.41342>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<8.17882,-5.10469,0.787786>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<7.93783,-5.97182,0.787786>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<9.05027,-4.87982,0.787786>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<7.54835,-4.46242,0.787786>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-6.94225,0.306642,-7.81727>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-7.8372,0.21152,-7.81727>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-6.41239,-0.420854,-7.81727>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-6.57714,1.12926,-7.81727>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-2.89181,-4.63888,0.499139>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-2.20922,-4.0523,0.499139>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-3.7411,-4.34103,0.499139>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-2.72511,-5.5233,0.499139>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-8.24288,-6.82802,1.20166>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-7.87901,-7.65119,1.20166>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-7.71194,-6.10132,1.20166>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-9.1377,-6.73156,1.20166>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<6.88067,-4.72332,-3.67813>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<6.18717,-4.14969,-3.67813>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<6.73065,-5.61073,-3.67813>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<7.7242,-4.40954,-3.67813>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-3.23445,3.91837,3.07371>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-4.12345,4.0587,3.07371>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-2.91149,3.07832,3.07371>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-2.66843,4.6181,3.07371>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-4.57688,-7.11982,1.41422>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-4.29871,-6.26389,1.41422>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-5.45723,-7.30688,1.41422>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-3.97471,-7.78869,1.41422>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-4.38284,1.09266,3.10249>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-3.77574,1.75706,3.10249>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-5.26178,1.28623,3.10249>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-4.111,0.234698,3.10249>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-5.9745,-5.3364,-7.87652>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-6.29396,-4.49501,-7.87652>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-6.54345,-6.03376,-7.87652>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-5.0861,-5.48045,-7.87652>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<0.537565,-4.77479,1.20004>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<0.0972879,-3.98983,1.20004>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<0.0779126,-5.54855,1.20004>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<1.4375,-4.78597,1.20004>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-7.33221,-5.53845,-7.90784>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-7.30951,-4.63874,-7.90784>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-8.12274,-5.96864,-7.90784>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-6.56439,-6.00797,-7.90784>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<6.00615,-4.44585,-1.96535>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<6.54011,-3.72136,-1.96535>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<5.11174,-4.34567,-1.96535>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<6.36659,-5.27051,-1.96535>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<5.30139,1.08221,1.7824>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<5.86541,0.380873,1.7824>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<5.62676,1.92134,1.7824>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<4.412,0.944429,1.7824>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<7.84581,-0.209272,8.22826>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<7.77875,-1.10677,8.22826>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<8.65659,0.181406,8.22826>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<7.10208,0.29755,8.22826>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-8.14857,-1.78193,8.25531>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-8.33667,-2.66206,8.25531>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-7.29231,-1.50477,8.25531>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-8.81672,-1.17897,8.25531>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<6.5958,-0.427159,-3.30013>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<5.85896,-0.943948,-3.30013>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<7.41177,-0.806884,-3.30013>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<6.51667,0.469355,-3.30013>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<6.84434,-3.70014,-7.87263>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<7.72715,-3.87523,-7.87263>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<6.55457,-2.84807,-7.87263>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<6.25131,-4.37713,-7.87263>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-3.05356,8.07593,8.21401>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-2.75448,8.92479,8.21401>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-3.93823,7.91052,8.21401>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-2.46797,7.3925,8.21401>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-8.02053,-5.39865,2.15371>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-7.2409,-5.84828,2.15371>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-8.02095,-4.49865,2.15371>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-8.79975,-5.84901,2.15371>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-3.5691,-5.67103,1.42125>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-4.27245,-6.23255,1.42125>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-2.73114,-5.99939,1.42125>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-3.70372,-4.78115,1.42125>
,0.7
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

sphere {
<-5.27024,1.28742,3.10459>
,0.7
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-4.64385,-2.34711,2.79509>
,0.7
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-7.27561,0.933176,3.32308>
,0.7
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<0.677085,1.70046,7.14119>
,0.7
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<2.27255,-5.87842,2.51988>
,0.7
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<7.46373,-6.72892,1.19997>
,0.7
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-4.27494,-6.24564,1.41782>
,0.7
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<1.23796,-3.16546,-4.12953>
,0.7
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<7.65053,-5.3521,3.23835>
,0.7
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-6.55911,8.15432,1.43107>
,0.7
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-2.44148,7.38181,8.22999>
,0.7
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<7.08483,0.309277,8.2269>
,0.7
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<6.37548,-5.27937,-1.97803>
,0.7
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<0.0675389,-5.56519,1.20161>
,0.7
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<6.42051,-6.12799,2.06211>
,0.7
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<2.06714,-1.81047,-4.14327>
,0.7
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-7.11856,-2.2276,-1.943>
,0.7
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<0.526449,-5.82643,3.39845>
,0.7
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<6.25065,-4.41654,-7.85962>
,0.7
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<7.95863,1.92725,2.47451>
,0.7
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-7.06736,2.15575,2.44656>
,0.7
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-7.94435,0.185471,8.21938>
,0.7
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-7.70388,-6.07857,1.18435>
,0.7
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-6.71917,6.57638,1.43833>
,0.7
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-1.25113,3.28974,3.55633>
,0.7
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<7.43101,-0.809254,-3.29984>
,0.7
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-2.71672,-5.5294,0.490584>
,0.7
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<7.78227,-1.14404,8.22932>
,0.7
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-6.58121,1.15417,-7.82511>
,0.7
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-6.41197,-0.422085,-7.8174>
,0.7
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<7.86905,0.369613,3.32977>
,0.7
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-3.76822,1.7615,3.09826>
,0.7
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<5.63238,1.93604,1.78079>
,0.7
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-4.10298,0.22747,3.10214>
,0.7
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-7.99019,-7.79855,1.20037>
,0.7
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<7.73012,-4.40562,-3.67785>
,0.7
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-7.56513,-3.74307,-1.96112>
,0.7
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<7.73941,-6.11613,-1.98819>
,0.7
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<4.06475,-6.31729,2.34566>
,0.7
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-5.47999,-7.31594,1.44145>
,0.7
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-2.90797,3.06756,3.07721>
,0.7
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<2.3945,-4.28008,2.51074>
,0.7
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-0.762746,1.02848,7.14102>
,0.7
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-8.03034,-4.4891,2.15223>
,0.7
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-8.1347,-5.96743,-7.91147>
,0.7
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-5.55904,-1.05782,2.79724>
,0.7
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<5.09179,-4.31514,-1.95544>
,0.7
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-4.12713,4.06172,3.07256>
,0.7
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-3.94681,7.90778,8.21923>
,0.7
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<0.826243,-4.77452,2.37602>
,0.7
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-6.55366,-6.03618,-7.87647>
,0.7
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<4.30733,-3.58791,-3.08444>
,0.7
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<5.83609,-0.948295,-3.30024>
,0.7
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<7.74903,-3.89057,-7.90232>
,0.7
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<2.64717,5.6003,5.7198>
,0.7
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<0.0846289,-3.95887,1.19911>
,0.7
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<7.54646,-4.46152,0.787106>
,0.7
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<7.09962,-8.25917,1.20491>
,0.7
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-3.70802,-4.76627,1.42547>
,0.7
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<6.1815,-4.13156,-3.67414>
,0.7
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
