#include "colors.inc"

#declare locX=14.358;
#declare locY=14.358;
#declare locZ=14.358;

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
<-0.707998,-2.31404,1.20035>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-0.000240447,-1.60758,1.20035>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-1.41576,-3.02049,1.20035>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-4.21538,1.75127,2.17248>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-4.05135,2.73773,2.17248>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-4.37941,0.764814,2.17248>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-1.30624,-2.43124,3.49323>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-1.79539,-1.55904,3.49323>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-0.8171,-3.30344,3.49323>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-5.41735,5.04506,-7.17538>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-5.85684,5.9433,-7.17538>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-4.97785,4.14681,-7.17538>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-1.12347,-1.25467,1.59872>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-1.83401,-0.551019,1.59872>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-0.412924,-1.95833,1.59872>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<6.76093,-6.8476,-6.03435>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<7.15921,-5.93034,-6.03435>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<6.36264,-7.76486,-6.03435>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<4.51246,-2.58226,2.44172>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<5.44169,-2.21275,2.44172>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<3.58324,-2.95177,2.44172>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-0.907327,-0.184568,1.92427>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-1.87534,-0.435452,1.92427>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<0.0606902,0.066315,1.92427>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-4.23095,0.0171686,-6.97369>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-4.9224,-0.705253,-6.97369>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-3.53949,0.73959,-6.97369>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<1.06029,-2.24635,6.54445>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<0.227067,-2.79928,6.54445>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<1.89352,-1.69342,6.54445>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-0.983209,-0.792072,-5.20657>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-0.259665,-1.48235,-5.20657>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-1.70675,-0.101794,-5.20657>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-5.43623,4.47697,2.9583>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-6.09866,5.22609,2.9583>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-4.7738,3.72785,2.9583>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-2.96393,6.1881,3.84099>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-3.07715,5.19453,3.84099>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-2.8507,7.18167,3.84099>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<6.57249,2.40599,5.57078>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<6.00183,1.58481,5.57078>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<7.14314,3.22718,5.57078>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-0.0121478,0.709659,6.69918>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<0.810795,0.141534,6.69918>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-0.83509,1.27778,6.69918>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-4.50807,0.819908,-5.33864>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-3.52371,0.996075,-5.33864>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-5.49243,0.643742,-5.33864>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-4.47467,3.14513,6.73927>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-3.94076,2.29959,6.73927>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-5.00858,3.99067,6.73927>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<7.03014,-4.79837,-5.92767>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<6.9711,-3.80012,-5.92767>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<7.08919,-5.79663,-5.92767>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<0.596469,-2.80096,1.38238>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<0.0639694,-3.64739,1.38238>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<1.12897,-1.95453,1.38238>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-1.68017,-0.198593,-6.51611>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-1.28588,0.720395,-6.51611>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-2.07445,-1.11758,-6.51611>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<1.72703,0.576702,6.70731>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<2.62291,1.02101,6.70731>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<0.831154,0.132398,6.70731>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<5.92592,-5.32325,-5.54907>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<6.08133,-4.3354,-5.54907>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<5.77051,-6.3111,-5.54907>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-1.79352,-7.10129,-1.57087>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-0.825845,-7.3535,-1.57087>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-2.76119,-6.84908,-1.57087>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<1.34063,-2.82117,3.66936>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<2.23852,-3.26139,3.66936>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<0.442738,-2.38095,3.66936>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<1.29665,-2.8522,2.4073>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<2.12801,-3.40794,2.4073>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<0.465298,-2.29646,2.4073>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-3.92342,0.291072,2.54549>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-4.28162,1.22472,2.54549>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-3.56523,-0.642576,2.54549>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<2.58242,-3.11923,-5.26343>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<1.94493,-2.34877,-5.26343>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<3.21992,-3.88969,-5.26343>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-1.79965,0.866836,-4.32448>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-0.80062,0.822735,-4.32448>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-2.79867,0.910938,-4.32448>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<3.25867,-3.39355,3.66275>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<2.26549,-3.27699,3.66275>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<4.25185,-3.51012,3.66275>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-5.03625,1.10822,6.40903>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-5.57228,1.95242,6.40903>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-4.50023,0.264014,6.40903>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<2.75821,-1.13178,6.53916>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<1.91384,-1.66755,6.53916>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<3.60257,-0.596019,6.53916>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<2.51832,-2.01239,5.34038>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<1.53859,-1.81206,5.34038>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<3.49804,-2.21271,5.34038>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-6.82773,1.07567,-4.18449>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-7.82726,1.10631,-4.18449>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-5.82819,1.04503,-4.18449>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<1.43722,-0.792035,6.55184>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<1.89648,-1.68034,6.55184>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<0.977973,0.0962712,6.55184>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-0.105145,-2.14971,2.54623>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<0.0304457,-3.14047,2.54623>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-0.240735,-1.15894,2.54623>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-4.44685,3.54067,1.42249>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-3.82243,2.75957,1.42249>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-5.07126,4.32176,1.42249>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-4.1357,-0.221179,1.47608>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-4.51061,0.70588,1.47608>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-3.76078,-1.14824,1.47608>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<2.06626,-3.65844,4.90884>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<2.9063,-3.11593,4.90884>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<1.22621,-4.20096,4.90884>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-4.76429,-5.49003,-0.371875>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-3.98955,-6.12231,-0.371875>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-5.53904,-4.85775,-0.371875>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-4.95117,2.17028,6.75962>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-5.94614,2.07012,6.75962>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-3.9562,2.27045,6.75962>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-5.63286,3.4891,1.42461>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-5.07746,4.32068,1.42461>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-6.18826,2.65751,1.42461>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-1.27307,-3.02261,2.30223>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-1.75557,-2.14672,2.30223>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-0.790557,-3.8985,2.30223>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-2.92253,2.32239,6.76346>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-1.92341,2.36425,6.76346>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-3.92166,2.28053,6.76346>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-2.77503,-1.42978,0.664153>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-2.39264,-2.35378,0.664153>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-3.15742,-0.505775,0.664153>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-2.69405,-1.50035,1.77369>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-1.71752,-1.71572,1.77369>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-3.67058,-1.28497,1.77369>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<2.34524,-3.37701,2.36217>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<1.66003,-2.64867,2.36217>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<3.03045,-4.10536,2.36217>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-0.491234,-1.78136,0.0465704>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<0.507859,-1.82393,0.0465704>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-1.49033,-1.73879,0.0465704>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-5.83819,1.42254,-6.59257>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-6.77566,1.77059,-6.59257>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-4.90071,1.0745,-6.59257>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-0.0471975,-0.870331,6.87432>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<0.936342,-1.05102,6.87432>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-1.03074,-0.689637,6.87432>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-5.86399,0.0380795,6.44501>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-6.33474,0.920344,6.44501>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-5.39324,-0.844185,6.44501>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<0.473403,-4.07671,2.53395>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<0.0442856,-3.17346,2.53395>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<0.902521,-4.97996,2.53395>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-3.0613,5.29505,-4.57972>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-3.45984,4.37789,-4.57972>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-2.66276,6.2122,-4.57972>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<1.26249,-3.96978,1.17521>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<0.299528,-3.70015,1.17521>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<2.22545,-4.23942,1.17521>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-0.817522,-5.86072,2.75876>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-1.33501,-5.00503,2.75876>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-0.30003,-6.71641,2.75876>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-5.27297,2.24301,3.22466>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-4.78204,3.11421,3.22466>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-5.7639,1.37182,3.22466>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-4.55285,5.85399,2.84589>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-4.93114,6.77968,2.84589>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-4.17457,4.9283,2.84589>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-2.19217,0.568752,2.71651>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-1.41967,-0.0662632,2.71651>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-2.96467,1.20377,2.71651>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-2.88532,-0.295415,1.91347>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-3.87388,-0.144567,1.91347>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-1.89677,-0.446264,1.91347>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<0.577418,-0.0679584,-6.34981>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-0.370119,-0.387605,-6.34981>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<1.52495,0.251689,-6.34981>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-2.59865,0.445889,-5.21895>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-3.4365,0.991783,-5.21895>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-1.76079,-0.100004,-5.21895>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-2.54986,2.15517,2.44085>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-3.03656,1.2816,2.44085>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-2.06316,3.02874,2.44085>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-2.62547,-3.82244,5.50056>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-3.21593,-4.62951,5.50056>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-2.03501,-3.01537,5.50056>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<6.61583,-6.93399,-1.54903>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<6.92992,-7.88338,-1.54903>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<6.30173,-5.9846,-1.54903>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-6.19749,6.69029,2.01639>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-5.59892,5.88922,2.01639>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-6.79606,7.49136,2.01639>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<1.50374,-1.92293,1.23415>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<1.91611,-2.83395,1.23415>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<1.09137,-1.01192,1.23415>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-6.06101,1.90279,6.44315>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-6.3257,0.938459,6.44315>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-5.79632,2.86713,6.44315>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-6.8699,0.338856,5.41235>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-6.62627,-0.631013,5.41235>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-7.11353,1.30872,5.41235>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-2.22476,-1.12667,2.97771>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-3.07431,-1.65416,2.97771>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-1.3752,-0.599169,2.97771>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-4.47044,5.22318,1.60832>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-4.98698,4.36692,1.60832>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-3.95391,6.07945,1.60832>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<1.51329,-1.81342,5.18936>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<1.1469,-2.74389,5.18936>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<1.87967,-0.882961,5.18936>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-5.1547,1.5418,1.45825>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-5.78184,2.32071,1.45825>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-4.52756,0.762887,1.45825>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-3.76766,0.0804104,-4.46743>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-4.21351,0.975521,-4.46743>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-3.32182,-0.8147,-4.46743>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-0.997577,0.569693,-5.21284>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-1.74708,-0.0923052,-5.21284>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-0.248072,1.23169,-5.21284>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-3.94307,1.46939,-6.58079>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-3.01923,1.85217,-6.58079>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-4.86691,1.08661,-6.58079>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

difference {
sphere {
<-5.36003,6.29626,6.01712>
,0.5
texture { pigment { colorLock } }
finish {ambient ambientv
 phong phongv }
}
union {
sphere {
<-4.36964,6.43454,6.01712>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
sphere {
<-6.35043,6.15798,6.01712>
,0.8
texture { pigment { colorLockCavity } }
finish {ambient ambientv
 phong phongv }
 }
}
}

sphere {
<1.14398,-2.75699,5.19967>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<3.57622,-2.95116,2.43652>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-1.75732,-0.100527,-5.21745>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-3.03835,1.26676,2.44065>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<0.824232,0.135741,6.70539>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-5.58511,5.88518,2.01138>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-1.88252,-0.445677,1.92183>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-4.50502,0.745525,1.44833>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-5.07421,4.32418,1.42316>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-6.33498,0.926828,6.44879>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<0.0333071,-1.59391,1.22477>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<6.35975,6.59068,-6.03585>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-3.75891,-1.15343,1.47442>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-3.93311,2.28468,6.76101>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<7.08111,-5.80724,-5.91765>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-5.86163,5.94796,-7.16945>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-1.7569,-2.14557,2.30328>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<2.25616,-3.27616,3.66809>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-4.88944,1.07777,-6.59861>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-3.51152,1.00829,-5.34179>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<1.90405,-1.68749,6.55188>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<0.0406439,-3.15374,2.55051>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-4.15019,4.92359,2.84984>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<1.92732,-2.84461,1.22787>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
sphere {
<-0.402235,-0.38116,-6.30115>
,0.8
texture { pigment { colorKey } }
finish {ambient ambientv
 phong phongv }
}
