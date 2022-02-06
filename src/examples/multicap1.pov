#include "colors.inc"

#declare locX=9.12819;
#declare locY=9.12819;
#declare locZ=9.12819;

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
<2.89636,-0.604179,-2.71369>
,0.7
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<2.4318,-1.25548,-2.71369>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<3.36091,0.0471178,-2.71369>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-4.02152,3.67854,-3.033>
,0.7
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-4.29107,4.43177,-3.033>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-3.75197,2.92532,-3.033>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-3.412,-1.42808,-2.7028>
,0.7
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-2.62164,-1.55188,-2.7028>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-4.20236,-1.30428,-2.7028>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-4.19039,1.98818,-2.71608>
,0.7
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-4.65888,1.33971,-2.71608>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-3.7219,2.63665,-2.71608>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-2.16978,0.200942,-2.89834>
,0.7
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-2.96952,0.180344,-2.89834>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-1.37005,0.221541,-2.89834>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-3.55349,0.762065,-2.90289>
,0.7
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-2.99154,0.192673,-2.90289>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-4.11545,1.33146,-2.90289>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<3.91908,0.69678,-2.71365>
,0.7
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<4.44013,1.30383,-2.71365>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<3.39803,0.0897334,-2.71365>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<4.26681,-0.831186,-2.70621>
,0.7
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<4.91629,-1.29828,-2.70621>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<3.61733,-0.364091,-2.70621>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-0.624105,-2.97361,2.42761>
,0.7
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-1.20778,-3.52072,2.42761>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-0.0404326,-2.4265,2.42761>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-3.78185,-4.02391,-3.03904>
,0.7
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-3.27897,-3.40172,-3.03904>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-4.28473,-4.64609,-3.03904>
,0.5
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

sphere {
<4.45232,1.32115,-2.72352>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-2.42996,0.043348,-4.0743>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-4.20581,-1.30424,-2.70268>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-4.30131,4.46553,-3.0322>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-1.21632,-3.52625,2.42902>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<2.42955,-1.25745,-2.714>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-3.71447,2.64278,-2.71463>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-2.98801,0.184761,-2.90402>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-1.36393,0.222231,-2.89755>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<3.38918,0.0631816,-2.71883>
,0.5
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
