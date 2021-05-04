
// Kronrod Quadrature (n=20) of an integral from a to b.
// a = Lower limit of integral
// b = Upper limit of integral
// grand() = External Function defining Integrand.
// vk = Value of Integral using 41 nodes
// vg = Value of Integral using 20 nodes
void
krab(double a, double b, double(*grand)(double), double* vk, double* vg) {
    const double wgb[] =
    {
        0.88070036e-2, 0.20300715e-1,
        0.31336024e-1, 0.41638371e-1,
        0.50965060e-1, 0.59097266e-1,
        0.65844319e-1, 0.71048055e-1,
        0.74586493e-1, 0.76376694e-1
    };

    const double wkb[] =
    {
        0.15367919e-2, 0.43001349e-2,
        0.73130846e-2, 0.10194187e-1,
        0.12941067e-1, 0.15643653e-1,
        0.18300085e-1, 0.20834437e-1,
        0.23217411e-1, 0.25472287e-1,
        0.27597553e-1, 0.29555700e-1,
        0.31326619e-1, 0.32917299e-1,
        0.34324336e-1, 0.35527212e-1,
        0.36515345e-1, 0.37291438e-1,
        0.37852249e-1, 0.38188934e-1
    };

    const double abb[] =
    {
        0.92460613e-2, 0.18014036e-1,
        0.29588683e-1, 0.43882786e-1,
        0.60861594e-1, 0.80441514e-1,
        0.10247929,    0.12683405,
        0.15338117,    0.18197315,
        0.21242977,    0.24456650,
        0.27820341,    0.31314696,
        0.34918607,    0.38610707,
        0.42369726,    0.46173674
    };
    double s = b - a;
    *vg = 0.0;
    *vk = 0.08300356e-1 * grand(a+0.5*s);

    DO 10 J=1,20
    ADD=S*ABB(J)
    ADD=GRAND(A+ADD)+GRAND(B-ADD)
    VK=VK+WKB(J)*ADD
    JJ=J/2
    IF(J-2*JJ) 10,5,10
  5 VG=VG+WGB(JJ)*ADD
10   CONTINUE
C
    VK=VK*S
    VG=VG*S
}
/*
!MS$INTEGER:4
      SUBROUTINE KRAB(A,B,GRAND,VK,VG)                                  
      IMPLICIT REAL *8(A-H,O-Z)                                         
C                                                                       
C     SUBROUTINE FOR KRONROD QUADRATURE (N=20) OF AN INTEGRAL FROM TO B.
C     A=LOWER LIMIT OF INTEGRAL                                         
C     B=UPPER LIMIT OF INTEGRAL                                         
C     GRAND=EXTERNAL FUNCTION DEFINING INTEGRAND.                       
C     VK=VALUE OF INTEGRAL USING 41 NODES                               
C     VG=VALUE OF INTEGRAL USING 20 NODES                               
C                                                                       
      DIMENSION ABB(20),WGB(10),WKB(20)                                 
      DATA WGB  /                         .88070036E-2 , .20300715E-1 , 
     1      .31336024E-1 , .41638371E-1 , .50965060E-1 , .59097266E-1 , 
     2      .65844319E-1 , .71048055E-1 , .74586493E-1 , .76376694E-1 / 
      DATA WKB /                          .15367919E-2 , .43001349E-2 , 
     1      .73130846E-2 , .10194187E-1 , .12941067E-1 , .15643653E-1 , 
     2      .18300085E-1 , .20834437E-1 , .23217411E-1 , .25472287E-1 , 
     3      .27597553E-1 , .29555700E-1 , .31326619E-1 , .32917299E-1 , 
     4      .34324336E-1 , .35527212E-1 , .36515345E-1 , .37291438E-1 , 
     5      .37852249E-1 , .38188934E-1 /                               
      DATA ABB /                          .57048210E-3 , .34357004E-2 , 
     1      .92460613E-2 , .18014036E-1 , .29588683E-1 , .43882786E-1 , 
     2      .60861594E-1 , .80441514E-1 , .10247929    , .12683405    , 
     3      .15338117    , .18197315    , .21242977    , .24456650    , 
     4      .27820341    , .31314696    , .34918607    , .38610707    , 
     5      .42369726    , .46173674    /                               
C                                                                       
      S=B-A                                                             
      VG=0.                                                             
      VK=.08300356E-1*GRAND(A+.5*S)                                     
C                                                                       
      DO 10 J=1,20                                                      
      ADD=S*ABB(J)                                                      
      ADD=GRAND(A+ADD)+GRAND(B-ADD)                                     
      VK=VK+WKB(J)*ADD                                                  
      JJ=J/2                                                            
      IF(J-2*JJ) 10,5,10                                                
    5 VG=VG+WGB(JJ)*ADD                                                 
 10   CONTINUE                                                          
C                                                                       
      VK=VK*S                                                           
      VG=VG*S                                                           
      RETURN                                                            
      END                                                               
*/
