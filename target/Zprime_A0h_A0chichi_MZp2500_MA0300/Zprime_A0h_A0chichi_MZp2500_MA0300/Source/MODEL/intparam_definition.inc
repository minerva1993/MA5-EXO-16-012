ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c      written by the UFO converter
ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc

C     Parameters that should not be recomputed event by event.
C     
      IF(READLHA) THEN

        G = 2 * DSQRT(AS*PI)  ! for the first init

        MDL_TB__EXP__2 = MDL_TB**2

        MDL_CB = 1.000000D+00/SQRT(DCMPLX(1.000000D+00+MDL_TB__EXP__2))

        MDL_MPHIP = 3.000000D+02

        MDL_QZPHID = 0.000000D+00

        MDL_QZPHIP = 5.000000D-01

        MDL_QZPHIU = 5.000000D-01

        MDL_SA = -(1.000000D+00/SQRT(DCMPLX(1.000000D+00
     $   +MDL_TB__EXP__2)))

        MDL_SB = MDL_TB/SQRT(DCMPLX(1.000000D+00+MDL_TB__EXP__2))

        MDL_SA__EXP__2 = MDL_SA**2

        MDL_CA = SQRT(DCMPLX(1.000000D+00-MDL_SA__EXP__2))

        MDL_MZ__EXP__2 = MDL_MZ**2

        MDL_MZ__EXP__4 = MDL_MZ**4

        MDL_SQRT__2 = SQRT(DCMPLX(2.000000D+00))

        MDL_MZP__EXP__2 = MDL_MZP**2

        MDL_GZ__EXP__2 = MDL_GZ**2

        MDL_QZPHID__EXP__2 = MDL_QZPHID**2

        MDL_QZPHIU__EXP__2 = MDL_QZPHIU**2

        MDL_QZPHIP__EXP__2 = MDL_QZPHIP**2

        MDL_COMPLEXI = DCMPLX(0.000000D+00,1.000000D+00)

        MDL_CB__EXP__2 = MDL_CB**2

        MDL_SB__EXP__2 = MDL_SB**2

        MDL_CA__EXP__2 = MDL_CA**2

        MDL_AEW = 1.000000D+00/AEWM1

        MDL_MW = SQRT(DCMPLX(MDL_MZ__EXP__2/2.000000D+00
     $   +SQRT(DCMPLX(MDL_MZ__EXP__4/4.000000D+00-(MDL_AEW*PI
     $   *MDL_MZ__EXP__2)/(MDL_GF*MDL_SQRT__2)))))

        MDL_SQRT__AEW = SQRT(DCMPLX(MDL_AEW))

        MDL_EE = 2.000000D+00*MDL_SQRT__AEW*SQRT(DCMPLX(PI))

        MDL_MW__EXP__2 = MDL_MW**2

        MDL_SW2 = 1.000000D+00-MDL_MW__EXP__2/MDL_MZ__EXP__2

        MDL_CW = SQRT(DCMPLX(1.000000D+00-MDL_SW2))

        MDL_SQRT__SW2 = SQRT(DCMPLX(MDL_SW2))

        MDL_SW = MDL_SQRT__SW2

        MDL_G1 = MDL_EE/MDL_CW

        MDL_GW = MDL_EE/MDL_SW

        MDL_VD = (2.000000D+00*MDL_CB*MDL_MW*MDL_SW)/MDL_EE

        MDL_VEV = (2.000000D+00*MDL_MW*MDL_SW)/MDL_EE

        MDL_VU = (2.000000D+00*MDL_MW*MDL_SB*MDL_SW)/MDL_EE

        MDL_VD__EXP__2 = MDL_VD**2

        MDL_VU__EXP__2 = MDL_VU**2

        MDL_DELTAM2 = (MDL_EE*MDL_GZ*(MDL_QZPHID*MDL_VD__EXP__2
     $   +MDL_QZPHIU*MDL_VU__EXP__2))/(2.000000D+00*MDL_CW*MDL_SW)

        MDL_VP = SQRT(DCMPLX((MDL_MZP__EXP__2/MDL_GZ__EXP__2
     $   -MDL_QZPHID__EXP__2*MDL_VD__EXP__2-MDL_QZPHIU__EXP__2
     $   *MDL_VU__EXP__2)/MDL_QZPHIP__EXP__2))

        MDL_YB = (MDL_YMB*MDL_SQRT__2)/MDL_VD

        MDL_YT = (MDL_YMT*MDL_SQRT__2)/MDL_VU

        MDL_YTAU = (MDL_YMTAU*MDL_SQRT__2)/MDL_VD

        MDL_EP = MDL_DELTAM2/(-MDL_MZ__EXP__2+MDL_MZP__EXP__2)

        MDL_I1A33 = MDL_YB

        MDL_I2A33 = MDL_YT

        MDL_I3A33 = MDL_YT

        MDL_I4A33 = MDL_YB

        MDL_EE__EXP__2 = MDL_EE**2

        MDL_SW__EXP__2 = MDL_SW**2

        MDL_CW__EXP__2 = MDL_CW**2

      ENDIF
C     
C     Parameters that should be recomputed at an event by even basis.
C     
      AS = G**2/4/PI

      MDL_SQRT__AS = SQRT(DCMPLX(AS))

      MDL_G__EXP__2 = G**2

C     
C     Definition of the EW coupling used in the write out of aqed
C     
      GAL(1) = 3.5449077018110318D0 / DSQRT(AEWM1)
      GAL(2) = 1D0

