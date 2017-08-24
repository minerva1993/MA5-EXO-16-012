ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c      written by the UFO converter
ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc

      SUBROUTINE COUP1()

      IMPLICIT NONE
      INCLUDE 'model_functions.inc'

      DOUBLE PRECISION PI, ZERO
      PARAMETER  (PI=3.141592653589793D0)
      PARAMETER  (ZERO=0D0)
      INCLUDE 'input.inc'
      INCLUDE 'coupl.inc'
      GC_51 = -(MDL_CW*MDL_EE*MDL_EP*MDL_COMPLEXI)/(2.000000D+00
     $ *MDL_SW)
      GC_52 = (MDL_CW*MDL_EE*MDL_EP*MDL_COMPLEXI)/(2.000000D+00*MDL_SW)
      GC_57 = (MDL_EE*MDL_EP*MDL_COMPLEXI*MDL_SW)/(6.000000D+00*MDL_CW)
      GC_71 = (MDL_CA*MDL_CB*MDL_GZ)/2.000000D+00+(MDL_CA*MDL_CB
     $ *MDL_CW*MDL_EE*MDL_EP)/(2.000000D+00*MDL_SW)+(MDL_CW*MDL_EE
     $ *MDL_EP*MDL_SA*MDL_SB)/(2.000000D+00*MDL_SW)+(MDL_CA*MDL_CB
     $ *MDL_EE*MDL_EP*MDL_SW)/(2.000000D+00*MDL_CW)+(MDL_EE*MDL_EP
     $ *MDL_SA*MDL_SB*MDL_SW)/(2.000000D+00*MDL_CW)
      GC_9 = -MDL_GX
      GC_10 = (MDL_COMPLEXI*MDL_GZ)/2.000000D+00
      END
