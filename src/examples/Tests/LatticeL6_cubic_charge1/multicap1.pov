#include "colors.inc"

#declare locX=11.174;
#declare locY=11.174;
#declare locZ=11.174;

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
<1.68815,-4.72234,4.60182>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<2.14007,-4.32766,4.60182>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<1.23624,-5.11702,4.60182>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<1.29347,-4.27042,4.60182>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<2.08283,-5.17425,4.60182>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<1.68815,-4.72234,5.20182>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<1.68815,-4.72234,4.00182>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<3.88115,-5.39037,1.7135>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<4.36673,-5.03793,1.7135>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<3.39558,-5.74282,1.7135>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<3.52871,-4.9048,1.7135>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<4.2336,-5.87595,1.7135>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<3.88115,-5.39037,2.3135>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<3.88115,-5.39037,1.1135>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<5.25701,-2.49823,-1.29241>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<5.65698,-2.05099,-1.29241>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<4.85704,-2.94546,-1.29241>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<4.80977,-2.09825,-1.29241>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<5.70425,-2.8982,-1.29241>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<5.25701,-2.49823,-0.692406>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<5.25701,-2.49823,-1.89241>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-4.07972,-3.38931,0.784559>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-3.70655,-3.85915,0.784559>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-4.4529,-2.91948,0.784559>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-3.60989,-3.01614,0.784559>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-4.54955,-3.76249,0.784559>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-4.07972,-3.38931,1.38456>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-4.07972,-3.38931,0.184559>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<5.31107,-2.52679,0.0363158>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<5.88445,-2.70353,0.0363158>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<4.73769,-2.35005,0.0363158>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<5.48781,-1.95342,0.0363158>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<5.13433,-3.10017,0.0363158>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<5.31107,-2.52679,0.636316>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<5.31107,-2.52679,-0.563684>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<1.66,4.53985,3.11177>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<1.98942,4.03837,3.11177>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<1.33057,5.04133,3.11177>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<2.16147,4.86927,3.11177>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<1.15852,4.21042,3.11177>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<1.66,4.53985,3.71177>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<1.66,4.53985,2.51177>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-1.83069,-3.96335,-1.34803>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-2.26006,-4.38245,-1.34803>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-1.40132,-3.54426,-1.34803>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-1.41159,-4.39272,-1.34803>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-2.24979,-3.53399,-1.34803>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-1.83069,-3.96335,-0.748031>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-1.83069,-3.96335,-1.94803>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<2.97123,-4.45473,3.3423>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<2.44545,-4.16567,3.3423>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<3.49701,-4.74379,3.3423>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<2.68217,-4.98051,3.3423>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<3.26029,-3.92895,3.3423>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<2.97123,-4.45473,3.9423>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<2.97123,-4.45473,2.7423>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-1.42806,-2.03332,-4.84057>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-1.4005,-1.43395,-4.84057>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-1.45563,-2.63268,-4.84057>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-2.02743,-2.00575,-4.84057>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-0.828697,-2.06088,-4.84057>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-1.42806,-2.03332,-4.24057>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-1.42806,-2.03332,-5.44057>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<3.86682,-0.293828,-1.55927>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<3.34999,-0.5986,-1.55927>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<4.38365,0.0109435,-1.55927>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<4.17159,-0.810659,-1.55927>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<3.56205,0.223003,-1.55927>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<3.86682,-0.293828,-0.95927>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<3.86682,-0.293828,-2.15927>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<1.5145,-0.636553,-4.09974>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<2.10812,-0.723859,-4.09974>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<0.920888,-0.549247,-4.09974>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<1.60181,-0.0429393,-4.09974>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<1.4272,-1.23017,-4.09974>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<1.5145,-0.636553,-3.49974>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<1.5145,-0.636553,-4.69974>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-1.52726,-2.12857,-3.4776>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-1.75481,-2.68375,-3.4776>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-1.29971,-1.57339,-3.4776>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-0.972084,-2.35612,-3.4776>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-2.08244,-1.90102,-3.4776>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-1.52726,-2.12857,-2.8776>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-1.52726,-2.12857,-4.0776>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<1.29855,-3.45304,2.57754>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<1.57989,-3.98299,2.57754>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<1.0172,-2.92309,2.57754>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<1.82849,-3.1717,2.57754>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<0.768598,-3.73439,2.57754>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<1.29855,-3.45304,3.17754>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<1.29855,-3.45304,1.97754>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<1.8189,-4.09068,3.35323>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<2.41598,-4.14982,3.35323>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<1.22182,-4.03155,3.35323>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<1.87803,-3.4936,3.35323>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<1.75977,-4.68776,3.35323>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<1.8189,-4.09068,3.95323>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<1.8189,-4.09068,2.75323>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<5.38112,-4.57408,-0.11564>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<5.42988,-3.97607,-0.11564>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<5.33236,-5.1721,-0.11564>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<4.78311,-4.52533,-0.11564>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<5.97914,-4.62284,-0.11564>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<5.38112,-4.57408,0.48436>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<5.38112,-4.57408,-0.71564>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<2.62387,0.267148,-5.29438>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<2.0306,0.356792,-5.29438>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<3.21713,0.177504,-5.29438>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<2.53422,-0.326117,-5.29438>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<2.71351,0.860414,-5.29438>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<2.62387,0.267148,-4.69438>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<2.62387,0.267148,-5.89438>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<4.94038,-4.35881,-5.1708>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<4.90622,-3.75978,-5.1708>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<4.97454,-4.95784,-5.1708>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<4.34136,-4.39297,-5.1708>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<5.53941,-4.32465,-5.1708>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<4.94038,-4.35881,-4.5708>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<4.94038,-4.35881,-5.7708>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<3.72077,-3.85356,5.5551>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<3.54978,-4.42867,5.5551>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<3.89177,-3.27844,5.5551>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<4.29589,-4.02455,5.5551>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<3.14566,-3.68256,5.5551>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<3.72077,-3.85356,6.1551>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<3.72077,-3.85356,4.9551>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<2.27029,-2.59929,1.81492>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<2.57626,-3.11541,1.81492>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<1.96432,-2.08317,1.81492>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<2.78641,-2.29332,1.81492>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<1.75417,-2.90526,1.81492>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<2.27029,-2.59929,2.41492>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<2.27029,-2.59929,1.21492>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<3.01406,4.93108,1.66207>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<2.42012,5.01613,1.66207>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<3.60801,4.84604,1.66207>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<2.92902,4.33714,1.66207>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<3.09911,5.52502,1.66207>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<3.01406,4.93108,2.26207>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<3.01406,4.93108,1.06207>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-5.0481,-3.09993,0.792083>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-4.85792,-3.66899,0.792083>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-5.23828,-2.53086,0.792083>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-4.47904,-2.90975,0.792083>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-5.61716,-3.29011,0.792083>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-5.0481,-3.09993,1.39208>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-5.0481,-3.09993,0.192083>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-2.11045,-5.32592,4.60419>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-1.51498,-5.25236,4.60419>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-2.70592,-5.39949,4.60419>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-2.18402,-4.73045,4.60419>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-2.03688,-5.9214,4.60419>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-2.11045,-5.32592,5.20419>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-2.11045,-5.32592,4.00419>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-1.9733,-2.8258,-1.20834>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-1.44869,-3.11697,-1.20834>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-2.49792,-2.53463,-1.20834>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-1.68213,-2.30119,-1.20834>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-2.26448,-3.35041,-1.20834>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-1.9733,-2.8258,-0.608343>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-1.9733,-2.8258,-1.80834>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<5.03342,-3.97622,-1.30354>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<4.75238,-4.50633,-1.30354>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<5.31447,-3.44611,-1.30354>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<5.56353,-4.25726,-1.30354>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<4.50332,-3.69517,-1.30354>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<5.03342,-3.97622,-0.703535>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<5.03342,-3.97622,-1.90354>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-1.59057,-2.12978,-2.11569>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-1.8994,-2.64419,-2.11569>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-1.28174,-1.61536,-2.11569>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-1.07615,-2.43861,-2.11569>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-2.10498,-1.82095,-2.11569>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-1.59057,-2.12978,-1.51569>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-1.59057,-2.12978,-2.71569>
,0.4
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

sphere {
<-2.09539,-1.89421,-3.47563>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<4.3743,-5.03655,1.71283>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<3.62747,4.84896,1.6699>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<4.80431,-2.09539,-1.29445>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<1.20797,-4.02704,3.36015>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<3.34496,-0.60201,-1.5576>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<3.87869,-5.39456,1.1032>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-1.39845,-1.42225,-4.84137>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-1.97559,-2.82483,-0.603085>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-2.13204,-1.81489,-2.11257>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<2.4324,-4.15626,3.3469>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-1.43939,-3.11861,-1.21056>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<5.25645,-2.49814,-1.92732>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<5.3201,-2.52492,0.653449>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<2.61937,0.268008,5.26937>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<2.27658,-2.59542,1.20231>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<2.67825,-4.99657,3.33819>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<4.74443,-4.5168,-1.30927>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-1.26468,-1.60303,-2.12163>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<5.02896,-3.97491,-0.689317>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-0.793475,-2.0577,-4.83997>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<5.33446,-3.43505,-1.30883>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<2.57854,-3.12247,1.81696>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<2.16535,4.87253,3.11399>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<5.498,-1.92182,0.0436082>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<1.60964,-4.00455,2.58723>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<4.94067,-4.35994,-4.56383>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-4.45854,-2.90587,0.786573>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<3.72133,-3.85597,4.94479>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<3.87642,-0.282847,-2.18374>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<4.32407,-4.3969,-5.17438>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-2.10906,-5.3269,5.20831>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<2.62572,0.264153,-4.68286>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<3.89806,-3.25934,5.56397>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<2.00879,4.01659,3.1064>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<5.54733,-4.32503,-5.17238>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<2.72346,0.874759,-5.29444>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-1.58922,-2.13479,-2.73129>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<1.60536,-0.0272798,-4.10448>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-0.965051,-2.35557,-3.47788>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<4.49274,-3.68316,-1.30505>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-2.51029,-2.53016,-1.20945>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<5.02872,-3.97774,-1.92074>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-2.1101,-5.32538,4.00189>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<5.31975,-5.18863,-0.121331>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<1.66049,4.54228,3.71835>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<1.87724,-3.488,3.35413>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<2.14373,-4.31955,4.60017>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<5.37531,-4.57614,0.510254>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-2.25784,-3.52855,-1.3438>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<1.42477,-1.23592,-4.09728>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<1.68935,-4.71897,3.99659>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-2.71852,-5.40055,4.60404>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<2.09459,-5.18066,4.59906>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<3.10319,5.54382,1.65959>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<0.7513,-3.74125,2.56829>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<5.25834,-2.50897,-0.673278>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-1.40075,-4.40359,-1.34616>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<3.86461,-0.299534,-0.941067>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<2.01856,0.360094,-5.29547>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<1.69341,-4.71791,5.22908>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<3.51617,-4.89578,1.72035>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<3.88486,-5.39084,2.32763>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-4.85334,-3.6796,0.789431>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-1.43033,-2.03562,-4.22751>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-3.60535,-3.01419,0.785641>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-2.03896,-2.00853,-4.84507>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<2.41119,5.0177,1.65909>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-1.98103,-2.82218,-1.82541>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-5.26463,-2.71602,0.029291>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-2.2725,-4.39394,-1.34446>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-1.50439,-5.24909,4.60125>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<5.52973,-3.28471,0.801378>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-1.5962,-2.13203,-1.49597>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<0.995667,-2.89485,2.56944>
,0.4
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
