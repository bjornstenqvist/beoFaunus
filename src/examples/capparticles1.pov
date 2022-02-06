#include "colors.inc"

#declare locX=10;
#declare locY=10;
#declare locZ=10;

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
<0.0323001,1.00428,4.87356>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<0.5323,1.00428,4.87356>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-0.4677,1.00428,4.87356>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-4.80818,1.11503,-4.30936>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-4.30818,1.11503,-4.30936>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-5.30818,1.11503,-4.30936>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-2.74779,-0.383189,-2.80727>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-2.24779,-0.383189,-2.80727>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-3.24779,-0.383189,-2.80727>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

sphere {
<1.63606,-3.74103,0>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<4.20875,3.07531,0>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
