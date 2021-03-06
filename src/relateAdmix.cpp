#include <math.h>
#include <fstream>



void relateAdmix(double tolStop,int nSites,int K,int nIter,int useSq,int& numIter,int *geno1,int *geno2, double *a1,double *a2,double *start,double **f,double tol){


  int print=0;
  int totSites=0;
  int *keepSites = new int[nSites];
  int npop=K;

  for(int i=0;i<nSites;i++){
    if(geno1[i]==3 || geno2[i]==3)
      keepSites[i] = 0;
    else{
      keepSites[i] = 1;
      totSites++;
    }
  }
  if(print){
    for(int k=0;k<K;k++){
      fprintf(stderr,"a: K=%d\t%f\t%f\n",k,a1[k],a2[k]);
    }
    fprintf(stderr,"start = %f\t%f\t%f\n",start[0],start[1],start[2]);
    for(int i=0;i<nSites;i++){
      for(int k=0;k<K;k++){
	if(f[i][k]>1 | f[i][k] <0)
	  fprintf(stderr,"freq is fucked\n");
      }
    }
    fprintf(stderr,"\ntotSites=%d\n",totSites);
  }



 
int* g11=new int[nSites];
int* g21=new int[nSites];
int* g12=new int[nSites];
int* g22=new int[nSites];




double* tempPart=new double[totSites*3];
for(int i=0;i<totSites*3;i++)
  tempPart[i] =0;



////////// prep for speed 1
double* Pm11=new double[nSites*npop];
double* Pm12=new double[nSites*npop];
double* Pm21=new double[nSites*npop];
double* Pm22=new double[nSites*npop];



 for(int i=0;i<nSites;i++){
   
   // probablity of data
   g11[i]=0;
   g21[i]=0;
   g12[i]=0;
   g22[i]=0;

   if(geno1[i]<1)
     g11[i] = 1;
   if(geno1[i]<2)
     g12[i] = 1;
   if(geno2[i]<1)
     g21[i] = 1;
   if(geno2[i]<2)
     g22[i] = 1;
   for(int a11=0;a11<npop;a11++){
    if(g11[i]==1)
      Pm11[nSites*a11+i] = (g11[i]-f[i][a11]);
    else
      Pm11[nSites*a11+i] = (g11[i]+f[i][a11]);

    if(g21[i]==1)
      Pm12[nSites*a11+i] = (g21[i]-f[i][a11]);
    else
      Pm12[nSites*a11+i] = (g21[i]+f[i][a11]);

    if(g12[i]==1)
      Pm21[nSites*a11+i] = (g12[i]-f[i][a11]);
    else
      Pm21[nSites*a11+i] = (g12[i]+f[i][a11]);

    if(g22[i]==1)
      Pm22[nSites*a11+i] = (g22[i]-f[i][a11]);
    else
      Pm22[nSites*a11+i] = (g22[i]+f[i][a11]);
    //    fprintf(stderr,"fuck %f\t%d\t%f\n",Pm11[a11*nSites+i],g11[i],f[i][a11]);
    //   if(Pm11[a11*nSites+i]>100)
    //  Rprintf("fuck %f\t%d\t%f\n",Pm11[a11*nSites+i],g11[i],f[a11][i]);
   }
}
 // Rprintf("sadfsadf %f\tg=%d\t%f\n",Pm11[npop*1+9],g11[9],f[1][9]);



//////////done prep 1

// return(ans);



for(int a11=0;a11<npop;a11++){
  if(a1[a11] <  tol)
    continue;
for(int a12=0;a12<npop;a12++){
  if(a1[a12] <  tol)
    continue;
for(int a21=0;a21<npop;a21++){
  if(a2[a21] <  tol)
    continue;
for(int a22=0;a22<npop;a22++){
  if(a2[a22] <  tol)
    continue;
   double Pa0 = a1[a11]*a1[a12]*a2[a21]*a2[a22];
   double Pa1 = a1[a11]*a1[a12]*a2[a21]*a2[a22];
   double Pa2 = a1[a11]*a1[a12]*a2[a21]*a2[a22];
   double Sum1=0;
   double Sum2=0;
   for(int k=0;k<npop;k++){
      Sum1+=a1[k]*a2[k];
      Sum2+=a1[k]*a2[k];
   }
   Pa1 /=Sum1;
   Pa2  = Pa1/Sum2;
   int k1keep =1;
   int k2keep =1;
   if(a11!=a21)
     k1keep=0;
   if(a12!=a22)
     k2keep=0;
   int count=0;
  for(int i=0;i<nSites;i++){
   // probablity of data
 
    if(keepSites[i]==0)    
      continue;
    tempPart[0*totSites + count] += Pa0*Pm11[a11*nSites+i]*Pm12[a21*nSites+i]*Pm21[a12*nSites+i]*Pm22[a22*nSites+i];
    if(g11[i]==g21[i] &k1keep)
      tempPart[1*totSites + count] += Pa1*Pm11[a11*nSites+i]*Pm21[a12*nSites+i]*Pm22[a22*nSites+i];//k1=1 k2=0
    //      tempPart[2*totSites + count] += Pa1*Pm11[a11*nSites+i]*Pm21[a12*nSites+i]*Pm22[a22*nSites+i];//k1=1 k2=0
    if(g12[i]==g22[i] & k2keep){
      tempPart[1*totSites + count] += Pa1*Pm11[a11*nSites+i]*Pm12[a21*nSites+i]*Pm21[a12*nSites+i];//k1=0 k2=1
      if(g11[i]==g21[i] &k1keep)
	tempPart[2*totSites + count] += Pa2*Pm11[a11*nSites+i]*Pm21[a12*nSites+i];//k1=1 k2=1
    }
    count++;

  }
 }}}}
 int count = 0;
 for(int i=0;i<nSites;i++){
   int mult = 1;
   if(geno1[i] == 1)
     mult *=2;
   if(geno2[i] == 1)
     mult *=2;
  
     if(keepSites[i]==0)    
       continue;

     tempPart[0*totSites+count] *=mult;
     tempPart[1*totSites+count] *=mult;
     tempPart[2*totSites+count] *=mult;
  
     if(geno1[i] == 1 && geno2[i] == 1){
       tempPart[1*totSites+count] /=2;
       tempPart[2*totSites+count] /=2; 
     }
     count++;

 }
 //return(ans);
//// the em part


 int stepMax = 1;
 int mstep = 4;
 int stepMin = 1;
 
 double x[3];
 

 double p0[3];
 double p1[3];
 double q1[3];
 double q2[3];
 double sr2;
 double sq2;
 double sv2;
 double ttol=0.0000001;
 double norma;
 double alpha;
 double siteSum;
 double Pr0;
 double Pr1;
 double Pr2;

 ////// Test if k0>0.999
 x[0]=0.999;
 x[1]=0.0005;
 x[2]=0.0005;
 Pr0 = x[0];
 Pr1 = x[1]/2;
 Pr2 = x[2];

 for(int j=0;j<3;j++)
   x[j] = 0;
 
 for(int i=0;i<totSites;i++){
   
   siteSum=tempPart[i]*Pr0 +
     tempPart[1*totSites + i]*Pr1+
     tempPart[2*totSites + i]*Pr2;
   
   x[0] += tempPart[i]*Pr0/siteSum;
   x[1] += tempPart[1*totSites + i]*Pr1/siteSum;
   x[2] += tempPart[2*totSites + i]*Pr2/siteSum;
 }
 
 for(int j=0;j<3;j++)
   x[j] /= totSites;
 
 ///// start accelerated EM if k0<0.999
 if(x[1]<0.0005 && x[21]<0.0005 ){
   tolStop=0;
   numIter=1;
 }
 else{
   for(int j=0;j<3;j++)
     x[j] = start[j];
   
   for(int iter=0;iter<nIter;iter++){
     numIter=iter;

 
     Pr0 = x[0];
     Pr1 = x[1]/2;
     Pr2 = x[2];
     
     for(int j=0;j<3;j++)
       x[j] = 0;
     
     for(int i=0;i<totSites;i++){
       
       siteSum=tempPart[i]*Pr0 +
	 tempPart[1*totSites + i]*Pr1+
	 tempPart[2*totSites + i]*Pr2;
       
       x[0] += tempPart[i]*Pr0/siteSum;
       x[1] += tempPart[1*totSites + i]*Pr1/siteSum;
       x[2] += tempPart[2*totSites + i]*Pr2/siteSum;
     }


     for(int j=0;j<3;j++)
       x[j] /= totSites;
     //////  acceleration
     if(useSq && iter%3==2){
    
       sr2=0;
       sq2=0;
       sv2=0;

       //get stop sizes
       for(int j=0;j<3;j++){
	 q1[j] = p1[j] - p0[j];
	 sr2+= q1[j]*q1[j];
	 q2[j] = x[j] - p1[j];
	 sq2+= q2[j]*q2[j];
	 sv2+= (q2[j]-q1[j])*(q2[j]-q1[j]); 
       }

       //Stop the algorithm if the step size less than tolStop
       if(sqrt(sr2)<tolStop || sqrt(sq2)<tolStop || (p1[0]>0.999 & q2[0]>0)){
	 tolStop=sr2;
	 break;
       }
       
       //calc alpha and map into [1,stepMax] if needed
       alpha = sqrt(sr2/sv2);
       if(alpha>stepMax)
	 alpha=stepMax;
       if(alpha<1)
	 alpha=1;

       //the magical step
       for(int j=0;j<3;j++)
	 x[j] = p0[j] + 2 * alpha * q1[j] + alpha*alpha * (q2[j] - q1[j]);
       
       //in the rare instans that the boundarys are crossed. map into [ttol,1-ttol]
       for(int j=0;j<3;j++){
	 if(x[j]<ttol)
	   x[j]=ttol;
	 if(x[j]>1-ttol)
	   x[j]=1-ttol;

       }
       norma=x[0]+x[1]+x[2];
       for(int j=0;j<3;j++)
	 x[j] /= norma;

       //change step size
       if (alpha == stepMax) 
	 stepMax = mstep * stepMax;
     }
     
     if(useSq && iter%3==0){
       for(int j=0;j<3;j++)
	 p0[j] = x[j];
     }
     if(useSq && iter%3==1){
       for(int j=0;j<3;j++)
	 p1[j] = x[j];
     }
     
   }

 }


for(int j=0;j<3;j++)
  start[j] = x[j];

delete[] tempPart;
delete[] g11;
delete[] g12;
delete[] g21;
delete[] g22;
delete[] Pm11;
delete[] Pm21;
delete[] Pm12;
delete[] Pm22;
delete[] keepSites;

}



