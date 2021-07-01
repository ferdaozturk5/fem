///  FEM.CPP
///
///  Example program for EEE456 Electromagnetic Modelling
///
///  11th May 2021

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double** matrix(int N, int M);
void free_matrix(double** q);
void cgm(double **A, double *B, double *X, int N, int ITMAX, double TOL,double *AX);

int main()
{
    FILE *ToF;
    const int ne=237; // total number of elements
    const int n1=52; // number of nodes on Gamma_1 (Dirichlet boundary condition)
    const int nn=146; // total number of nodes (global)

    int ITMAX=100;
    double TOL=1E-6;
    int e,i,j;      // temporary variables
    int n[4][ne+1]; // n(i,e):connectivity array
                    // i:local number of a node
                    // e:element number
                    // n(i,e):global number of the node
    int nd[n1+1];   // global node numbers corresponding to Gamma_1
    double p[n1+1];
    double x[nn+1],y[nn+1]; // coordinates of global node numbers
    double **K;     // K[nn+1][nn+1]; // global matrix
    double b[nn+1];
    double X[nn+1];
    double AX[nn+1]; // Solution matrix; AX is not necessary
    double Ke[4][4]; // elemental matrix
    double be[4], ce[4];
    double deltae;

    K=matrix(nn+1,nn+1);

/// Input x and y coordinates of global node numbers
    x[1]=0;       y[1]=0;
    x[2]=1;       y[2]=0;
    x[3]=2;       y[3]=0;
    x[4]=3;       y[4]=-1;
    x[5]=4;       y[5]=-2;
    x[6]=4;       y[6]=-3;
    x[7]=5;       y[7]=-4;
    x[8]=6;       y[8]=-4;
    x[9]=7;       y[9]=-4;
    x[10]=8;       y[10]=-4;
    x[11]=9;       y[11]=-4;
    x[12]=10;       y[12]=-4;
    x[13]=11;       y[13]=-3;
    x[14]=11;       y[14]=-2;
    x[15]=12;       y[15]=-1;
    x[16]=13;       y[16]=0;
    x[17]=14;       y[17]=0;
    x[18]=15;       y[18]=0;
    x[19]=15;       y[19]=-1;
    x[20]=15;       y[20]=-2;
    x[21]=15;       y[21]=-3;
    x[22]=15;       y[22]=-4;
    x[23]=15;       y[23]=-5;
    x[24]=15;       y[24]=-6;
    x[25]=15;       y[25]=-7;
    x[26]=15;       y[26]=-8;
    x[27]=15;       y[27]=-9;
    x[28]=15;       y[28]=-10;
    x[29]=14;       y[29]=-10;
    x[30]=13;       y[30]=-10;
    x[31]=12;       y[31]=-10;
    x[32]=11;       y[32]=-10;
    x[33]=10;       y[33]=-10;
    x[34]=9;       y[34]=-10;
    x[35]=8;       y[35]=-10;
    x[36]=7;       y[36]=-10;
    x[37]=6;       y[37]=-10;
    x[38]=5;       y[38]=-10;
    x[39]=4;       y[39]=-10;
    x[40]=3;       y[40]=-10;
    x[41]=2;       y[41]=-10;
    x[42]=1;       y[42]=-10;
    x[43]=0;       y[43]=-10;
    x[44]=0;       y[44]=-9;
    x[45]=0;       y[45]=-8;
    x[46]=0;       y[46]=-7;
    x[47]=0;       y[47]=-6;
    x[48]=0;       y[48]=-5;
    x[49]=0;       y[49]=-4;
    x[50]=0;       y[50]=-3;
    x[51]=0;       y[51]=-2;
    x[52]=0;       y[52]=-1;
    x[53]=1;       y[53]=-1;
    x[54]=2;       y[54]=-1;
    x[55]=13;       y[55]=-1;
    x[56]=14;       y[56]=-1;
    x[57]=1;       y[57]=-2;
    x[58]=2;       y[58]=-2;
    x[59]=3;       y[59]=-2;
    x[60]=12;       y[60]=-2;
    x[61]=13;       y[61]=-2;
    x[62]=14;       y[62]=-2;
    x[63]=1;       y[63]=-3;
    x[64]=2;       y[64]=-3;
    x[65]=3;       y[65]=-3;
    x[66]=12;       y[66]=-3;
    x[67]=13;       y[67]=-3;
    x[68]=14;       y[68]=-3;
    x[69]=1;       y[69]=-4;
    x[70]=2;       y[70]=-4;
    x[71]=3;       y[71]=-4;
    x[72]=4;       y[72]=-4;
    x[73]=11;       y[73]=-4;
    x[74]=12;       y[74]=-4;
    x[75]=13;       y[75]=-4;
    x[76]=14;       y[76]=-4;
    x[77]=1;       y[77]=-5;
    x[78]=2;       y[78]=-5;
    x[79]=3;       y[79]=-5;
    x[80]=4;       y[80]=-5;
    x[81]=5;       y[81]=-5;
    x[82]=6;       y[82]=-5;
    x[83]=7;       y[83]=-5;
    x[84]=8;       y[84]=-5;
    x[85]=9;       y[85]=-5;
    x[86]=10;       y[86]=-5;
    x[87]=11;       y[87]=-5;
    x[88]=12;       y[88]=-5;
    x[89]=13;       y[89]=-5;
    x[90]=14;       y[90]=-5;
    x[91]=1;       y[91]=-6;
    x[92]=2;       y[92]=-6;
    x[93]=3;       y[93]=-6;
    x[94]=4;       y[94]=-6;
    x[95]=5;       y[95]=-6;
    x[96]=6;       y[96]=-6;
    x[97]=7;       y[97]=-6;
    x[98]=8;       y[98]=-6;
    x[99]=9;       y[99]=-6;
    x[100]=10;       y[100]=-6;
    x[101]=11;       y[101]=-6;
    x[102]=12;       y[102]=-6;
    x[103]=13;       y[103]=-6;
    x[104]=14;       y[104]=-6;
    x[105]=1;       y[105]=-7;
    x[106]=2;       y[106]=-7;
    x[107]=3;       y[107]=-7;
    x[108]=4;       y[108]=-7;
    x[109]=5;       y[109]=-7;
    x[110]=6;       y[110]=-7;
    x[111]=7;       y[111]=-7;
    x[112]=8;       y[112]=-7;
    x[113]=9;       y[113]=-7;
    x[114]=10;       y[114]=-7;
    x[115]=11;       y[115]=-7;
    x[116]=12;       y[116]=-7;
    x[117]=13;       y[117]=-7;
    x[118]=14;       y[118]=-7;
    x[119]=1;       y[119]=-8;
    x[120]=2;       y[120]=-8;
    x[121]=3;       y[121]=-8;
    x[122]=4;       y[122]=-8;
    x[123]=5;       y[123]=-8;
    x[124]=6;       y[124]=-8;
    x[125]=7;       y[125]=-8;
    x[126]=8;       y[126]=-8;
    x[127]=9;       y[127]=-8;
    x[128]=10;       y[128]=-8;
    x[129]=11;       y[129]=-8;
    x[130]=12;       y[130]=-8;
    x[131]=13;       y[131]=-8;
    x[132]=14;       y[132]=-8;
    x[133]=1;       y[133]=-9;
    x[134]=2;       y[134]=-9;
    x[135]=3;       y[135]=-9;
    x[136]=4;       y[136]=-9;
    x[137]=5;       y[137]=-9;
    x[138]=6;       y[138]=-9;
    x[139]=7;       y[139]=-9;
    x[140]=8;       y[140]=-9;
    x[141]=9;       y[141]=-9;
    x[142]=10;       y[142]=-9;
    x[143]=11;       y[143]=-9;
    x[144]=12;       y[144]=-9;
    x[145]=13;       y[145]=-9;
    x[146]=14;       y[146]=-9;


/// Input n(i,e):connectivity array
    n[1][1]=1;              n[2][1]=52;       n[3][1]=53;
    n[1][2]=1;              n[2][2]=53;       n[3][2]=2;
    n[1][3]=2;              n[2][3]=53;       n[3][3]=54;
    n[1][4]=2;              n[2][4]=54;       n[3][4]=3;
    n[1][5]=3;              n[2][5]=54;       n[3][5]=4;
    n[1][6]=16;             n[2][6]=15;       n[3][6]=55;
    n[1][7]=16;             n[2][7]=55;       n[3][7]=56;
    n[1][8]=16;             n[2][8]=56;       n[3][8]=17;
    n[1][9]=17;             n[2][9]=56;       n[3][9]=19;
    n[1][10]=17;            n[2][10]=19;       n[3][10]=18;
    n[1][11]=52;            n[2][11]=51;       n[3][11]=57;
    n[1][12]=52;            n[2][12]=57;       n[3][12]=53;
    n[1][13]=53;            n[2][13]=57;       n[3][13]=58;
    n[1][14]=53;            n[2][14]=58;       n[3][14]=54;
    n[1][15]=54;            n[2][15]=58;       n[3][15]=59;
    n[1][16]=54;            n[2][16]=59;       n[3][16]=4;
    n[1][17]=4;             n[2][17]=59;       n[3][17]=5;
    n[1][18]=15;            n[2][18]=14;       n[3][18]=60;
    n[1][19]=15;            n[2][19]=60;       n[3][19]=61;
    n[1][20]=15;            n[2][20]=61;       n[3][20]=55;
    n[1][21]=55;            n[2][21]=61;       n[3][21]=62;
    n[1][22]=55;            n[2][22]=62;       n[3][22]=56;
    n[1][23]=56;            n[2][23]=62;       n[3][23]=20;
    n[1][24]=56;            n[2][24]=20;       n[3][24]=19;
    n[1][25]=51;            n[2][25]=50;       n[3][25]=63;
    n[1][26]=51;            n[2][26]=63;       n[3][26]=57;
    n[1][27]=57;            n[2][27]=63;       n[3][27]=64;
    n[1][28]=57;            n[2][28]=64;       n[3][28]=58;
    n[1][29]=58;            n[2][29]=64;       n[3][29]=65;
    n[1][30]=58;            n[2][30]=65;       n[3][30]=59;
    n[1][31]=59;            n[2][31]=65;       n[3][31]=6;
    n[1][32]=59;            n[2][32]=6;       n[3][32]=5;
    n[1][33]=14;            n[2][33]=13;       n[3][33]=66;
    n[1][34]=14;            n[2][34]=66;       n[3][34]=60;
    n[1][35]=60;            n[2][35]=66;       n[3][35]=67;
    n[1][36]=60;            n[2][36]=67;       n[3][36]=61;
    n[1][37]=61;            n[2][37]=67;       n[3][37]=68;
    n[1][38]=61;            n[2][38]=68;       n[3][38]=62;
    n[1][39]=62;            n[2][39]=68;       n[3][39]=21;
    n[1][40]=62;            n[2][40]=21;       n[3][40]=20;
    n[1][41]=50;            n[2][41]=49;       n[3][41]=69;
    n[1][42]=50;            n[2][42]=69;       n[3][42]=63;
    n[1][43]=63;            n[2][43]=69;       n[3][43]=70;
    n[1][44]=63;            n[2][44]=70;       n[3][44]=64;
    n[1][45]=64;            n[2][45]=70;       n[3][45]=71;
    n[1][46]=64;            n[2][46]=71;       n[3][46]=65;
    n[1][47]=65;            n[2][47]=71;       n[3][47]=72;
    n[1][48]=65;            n[2][48]=72;       n[3][48]=6;
    n[1][49]=6;             n[2][49]=72;       n[3][49]=7;
    n[1][50]=13;            n[2][50]=12;       n[3][50]=73;
    n[1][51]=13;            n[2][51]=73;       n[3][51]=74;
    n[1][52]=13;            n[2][52]=74;       n[3][52]=66;
    n[1][53]=66;            n[2][53]=74;       n[3][53]=75;
    n[1][54]=66;            n[2][54]=75;       n[3][54]=67;
    n[1][55]=67;            n[2][55]=75;       n[3][55]=76;
    n[1][56]=67;            n[2][56]=76;       n[3][56]=68;
    n[1][57]=68;            n[2][57]=76;       n[3][57]=22;
    n[1][58]=68;            n[2][58]=22;       n[3][58]=21;
    n[1][59]=49;            n[2][59]=48;       n[3][59]=77;
    n[1][60]=49;            n[2][60]=77;       n[3][60]=69;
    n[1][61]=69;            n[2][61]=77;       n[3][61]=78;
    n[1][62]=69;            n[2][62]=78;       n[3][62]=70;
    n[1][63]=70;            n[2][63]=78;       n[3][63]=79;
    n[1][64]=70;            n[2][64]=79;       n[3][64]=71;
    n[1][65]=71;            n[2][65]=79;       n[3][65]=80;
    n[1][66]=71;            n[2][66]=80;       n[3][66]=72;
    n[1][67]=72;            n[2][67]=80;       n[3][67]=81;
    n[1][68]=72;            n[2][68]=81;       n[3][68]=7;
    n[1][69]=7;             n[2][69]=81;       n[3][69]=82;
    n[1][70]=7;             n[2][70]=82;       n[3][70]=8;
    n[1][71]=8;             n[2][71]=82;       n[3][71]=83;
    n[1][72]=8;             n[2][72]=83;       n[3][72]=9;
    n[1][73]=9;             n[2][73]=83;       n[3][73]=84;
    n[1][74]=9;             n[2][74]=84;       n[3][74]=10;
    n[1][75]=10;            n[2][75]=84;       n[3][75]=85;
    n[1][76]=10;            n[2][76]=85;       n[3][76]=11;
    n[1][77]=11;            n[2][77]=85;       n[3][77]=86;
    n[1][78]=11;            n[2][78]=86;       n[3][78]=12;
    n[1][79]=12;            n[2][79]=86;       n[3][79]=87;
    n[1][80]=12;            n[2][80]=87;       n[3][80]=73;
    n[1][81]=73;            n[2][81]=87;       n[3][81]=88;
    n[1][82]=73;            n[2][82]=88;       n[3][82]=74;
    n[1][83]=74;            n[2][83]=88;       n[3][83]=89;
    n[1][84]=74;            n[2][84]=89;       n[3][84]=75;
    n[1][85]=75;            n[2][85]=89;       n[3][85]=90;
    n[1][86]=75;            n[2][86]=90;       n[3][86]=76;
    n[1][87]=76;            n[2][87]=90;       n[3][87]=23;
    n[1][88]=76;            n[2][88]=23;       n[3][88]=22;
    n[1][89]=48;            n[2][89]=47;       n[3][89]=91;
    n[1][90]=48;            n[2][90]=91;       n[3][90]=77;
    n[1][91]=77;            n[2][91]=91;       n[3][91]=92;
    n[1][92]=77;            n[2][92]=92;       n[3][92]=78;
    n[1][93]=78;            n[2][93]=92;       n[3][93]=93;
    n[1][94]=78;            n[2][94]=93;       n[3][94]=79;
    n[1][95]=79;            n[2][95]=93;       n[3][95]=94;
    n[1][96]=79;            n[2][96]=94;       n[3][96]=80;
    n[1][97]=80;            n[2][97]=94;       n[3][97]=95;
    n[1][98]=80;            n[2][98]=95;       n[3][98]=81;
    n[1][99]=81;            n[2][99]=95;       n[3][99]=96;
    n[1][100]=81;           n[2][100]=96;       n[3][100]=82;
    n[1][101]=82;           n[2][101]=96;       n[3][101]=97;
    n[1][102]=82;           n[2][102]=97;       n[3][102]=83;
    n[1][103]=83;           n[2][103]=97;       n[3][103]=98;
    n[1][104]=83;           n[2][104]=98;       n[3][104]=84;
    n[1][105]=84;           n[2][105]=98;       n[3][105]=99;
    n[1][106]=84;           n[2][106]=99;       n[3][106]=85;
    n[1][107]=85;           n[2][107]=99;       n[3][107]=100;
    n[1][108]=85;           n[2][108]=100;       n[3][108]=86;
    n[1][109]=86;           n[2][109]=100;       n[3][109]=101;
    n[1][110]=86;           n[2][110]=101;       n[3][110]=87;
    n[1][111]=87;           n[2][111]=101;       n[3][111]=102;
    n[1][112]=87;           n[2][112]=102;       n[3][112]=88;
    n[1][113]=88;           n[2][113]=102;       n[3][113]=103;
    n[1][114]=88;           n[2][114]=103;       n[3][114]=89;
    n[1][115]=89;           n[2][115]=103;       n[3][115]=104;
    n[1][116]=89;           n[2][116]=104;       n[3][116]=90;
    n[1][117]=90;           n[2][117]=104;       n[3][117]=24;
    n[1][118]=90;           n[2][118]=24;       n[3][118]=23;
    n[1][119]=47;           n[2][119]=46;       n[3][119]=105;
    n[1][120]=47;           n[2][120]=105;          n[3][120]=91;
    n[1][121]=91;           n[2][121]=105;          n[3][121]=106;
    n[1][122]=91;           n[2][122]=106;          n[3][122]=92;
    n[1][123]=92;           n[2][123]=106;          n[3][123]=107;
    n[1][124]=92;           n[2][124]=107;          n[3][124]=93;
    n[1][125]=93;           n[2][125]=107;          n[3][125]=108;
    n[1][126]=93;           n[2][126]=108;          n[3][126]=94;
    n[1][127]=94;           n[2][127]=108;          n[3][127]=109;
    n[1][128]=94;           n[2][128]=109;          n[3][128]=95;
    n[1][129]=95;           n[2][129]=109;          n[3][129]=110;
    n[1][130]=95;           n[2][130]=110;          n[3][130]=96;
    n[1][131]=96;           n[2][131]=110;          n[3][131]=111;
    n[1][132]=96;           n[2][132]=111;          n[3][132]=97;
    n[1][133]=97;           n[2][133]=111;          n[3][133]=112;
    n[1][134]=97;           n[2][134]=112;          n[3][134]=98;
    n[1][135]=98;           n[2][135]=112;          n[3][135]=113;
    n[1][136]=98;           n[2][136]=113;          n[3][136]=99;
    n[1][137]=99;           n[2][137]=113;          n[3][137]=114;
    n[1][138]=99;           n[2][138]=114;          n[3][138]=100;
    n[1][139]=100;          n[2][139]=114;          n[3][139]=115;
    n[1][140]=100;          n[2][140]=115;          n[3][140]=101;
    n[1][141]=101;          n[2][141]=115;          n[3][141]=116;
    n[1][142]=101;          n[2][142]=116;          n[3][142]=102;
    n[1][143]=102;          n[2][143]=116;          n[3][143]=117;
    n[1][144]=102;          n[2][144]=117;          n[3][144]=103;
    n[1][145]=103;          n[2][145]=117;          n[3][145]=118;
    n[1][146]=103;          n[2][146]=118;          n[3][146]=104;
    n[1][147]=104;          n[2][147]=118;          n[3][147]=25;
    n[1][148]=104;          n[2][148]=25;           n[3][148]=24;
    n[1][149]=46;           n[2][149]=45;           n[3][149]=119;
    n[1][150]=46;           n[2][150]=119;          n[3][150]=105;
    n[1][151]=105;          n[2][151]=119;          n[3][151]=120;
    n[1][152]=105;          n[2][152]=120;          n[3][152]=106;
    n[1][153]=106;          n[2][153]=120;          n[3][153]=121;
    n[1][154]=106;          n[2][154]=121;          n[3][154]=107;
    n[1][155]=107;          n[2][155]=121;          n[3][155]=122;
    n[1][156]=107;          n[2][156]=122;          n[3][156]=108;
    n[1][157]=108;          n[2][157]=122;          n[3][157]=123;
    n[1][158]=108;          n[2][158]=123;          n[3][158]=109;
    n[1][159]=109;          n[2][159]=123;          n[3][159]=124;
    n[1][160]=109;          n[2][160]=124;          n[3][160]=110;
    n[1][161]=110;          n[2][161]=124;          n[3][161]=125;
    n[1][162]=110;          n[2][162]=125;          n[3][162]=111;
    n[1][163]=111;          n[2][163]=125;          n[3][163]=126;
    n[1][164]=111;          n[2][164]=126;          n[3][164]=112;
    n[1][165]=112;          n[2][165]=126;          n[3][165]=127;
    n[1][166]=112;          n[2][166]=127;          n[3][166]=113;
    n[1][167]=113;          n[2][167]=127;          n[3][167]=128;
    n[1][168]=113;          n[2][168]=128;          n[3][168]=114;
    n[1][169]=114;          n[2][169]=128;          n[3][169]=129;
    n[1][170]=114;          n[2][170]=129;          n[3][170]=115;
    n[1][171]=115;          n[2][171]=129;          n[3][171]=130;
    n[1][172]=115;          n[2][172]=130;          n[3][172]=116;
    n[1][173]=116;          n[2][173]=130;          n[3][173]=131;
    n[1][174]=116;          n[2][174]=131;          n[3][174]=117;
    n[1][175]=117;          n[2][175]=131;          n[3][175]=132;
    n[1][176]=117;          n[2][176]=132;          n[3][176]=118;
    n[1][177]=118;          n[2][177]=132;          n[3][177]=26;
    n[1][178]=118;          n[2][178]=26;           n[3][178]=25;
    n[1][179]=45;           n[2][179]=44;           n[3][179]=133;
    n[1][180]=45;           n[2][180]=133;          n[3][180]=119;
    n[1][181]=119;          n[2][181]=133;          n[3][181]=134;
    n[1][182]=119;          n[2][182]=134;          n[3][182]=120;
    n[1][183]=120;          n[2][183]=134;          n[3][183]=135;
    n[1][184]=120;          n[2][184]=135;          n[3][184]=121;
    n[1][185]=121;          n[2][185]=135;          n[3][185]=136;
    n[1][186]=121;          n[2][186]=136;          n[3][186]=122;
    n[1][187]=122;          n[2][187]=136;          n[3][187]=137;
    n[1][188]=122;          n[2][188]=137;          n[3][188]=123;
    n[1][189]=123;          n[2][189]=137;          n[3][189]=138;
    n[1][190]=123;          n[2][190]=138;          n[3][190]=124;
    n[1][191]=124;          n[2][191]=138;          n[3][191]=139;
    n[1][192]=124;          n[2][192]=139;          n[3][192]=125;
    n[1][193]=125;          n[2][193]=139;          n[3][193]=140;
    n[1][194]=125;          n[2][194]=140;          n[3][194]=126;
    n[1][195]=126;          n[2][195]=140;          n[3][195]=141;
    n[1][196]=126;          n[2][196]=141;          n[3][196]=127;
    n[1][197]=127;          n[2][197]=141;          n[3][197]=142;
    n[1][198]=127;          n[2][198]=142;          n[3][198]=128;
    n[1][199]=128;          n[2][199]=142;          n[3][199]=143;
    n[1][200]=128;          n[2][200]=143;          n[3][200]=129;
    n[1][201]=129;          n[2][201]=143;          n[3][201]=144;
    n[1][202]=129;          n[2][202]=144;          n[3][202]=130;
    n[1][203]=130;          n[2][203]=144;          n[3][203]=145;
    n[1][204]=130;          n[2][204]=145;          n[3][204]=131;
    n[1][205]=131;          n[2][205]=145;          n[3][205]=146;
    n[1][206]=131;          n[2][206]=146;          n[3][206]=132;
    n[1][207]=132;          n[2][207]=146;          n[3][207]=27;
    n[1][208]=132;          n[2][208]=27;           n[3][208]=26;
    n[1][209]=44;           n[2][209]=43;           n[3][209]=42;
    n[1][210]=44;           n[2][210]=42;           n[3][210]=133;
    n[1][211]=133;          n[2][211]=42;           n[3][211]=41;
    n[1][212]=133;          n[2][212]=41;           n[3][212]=134;
    n[1][213]=134;          n[2][213]=41;           n[3][213]=40;
    n[1][214]=134;          n[2][214]=40;           n[3][214]=135;
    n[1][215]=135;          n[2][215]=40;           n[3][215]=39;
    n[1][216]=135;          n[2][216]=39;           n[3][216]=136;
    n[1][217]=136;          n[2][217]=39;           n[3][217]=38;
    n[1][218]=136;          n[2][218]=38;           n[3][218]=137;
    n[1][219]=137;          n[2][219]=38;           n[3][219]=37;
    n[1][220]=137;          n[2][220]=37;           n[3][220]=138;
    n[1][221]=138;          n[2][221]=37;           n[3][221]=36;
    n[1][222]=138;          n[2][222]=36;           n[3][222]=139;
    n[1][223]=139;          n[2][223]=36;           n[3][223]=35;
    n[1][224]=139;          n[2][224]=35;           n[3][224]=140;
    n[1][225]=140;          n[2][225]=35;           n[3][225]=34;
    n[1][226]=140;          n[2][226]=34;           n[3][226]=141;
    n[1][227]=141;          n[2][227]=34;           n[3][227]=33;
    n[1][228]=141;          n[2][228]=33;           n[3][228]=142;
    n[1][229]=142;          n[2][229]=33;           n[3][229]=32;
    n[1][230]=142;          n[2][230]=32;           n[3][230]=143;
    n[1][231]=143;          n[2][231]=32;           n[3][231]=31;
    n[1][232]=143;          n[2][232]=31;           n[3][232]=144;
    n[1][233]=144;          n[2][233]=31;           n[3][233]=30;
    n[1][234]=144;          n[2][234]=30;           n[3][234]=145;
    n[1][235]=145;          n[2][235]=30;           n[3][235]=29;
    n[1][236]=145;          n[2][236]=29;           n[3][236]=146;
    n[1][237]=146;          n[2][237]=29;           n[3][237]=28;
    n[1][238]=146;          n[2][238]=28;           n[3][238]=27;


/// Input boundary conditions on Gamma_1
    nd[1]=1;            p[1]=20;
    nd[2]=2;            p[2]=20;
    nd[3]=3;            p[3]=20;
    nd[4]=4;            p[4]=20;
    nd[5]=5;            p[5]=20;
    nd[6]=6;            p[6]=20;
    nd[7]=7;            p[7]=20;
    nd[8]=8;            p[8]=20;
    nd[9]=9;            p[9]=20;
    nd[10]=10;          p[10]=20;
    nd[11]=11;          p[11]=20;
    nd[12]=12;          p[12]=20;
    nd[13]=13;          p[13]=20;
    nd[14]=14;          p[14]=20;
    nd[15]=15;          p[15]=20;
    nd[16]=16;          p[16]=20;
    nd[17]=17;          p[17]=20;
    nd[18]=18;          p[18]=20;
    nd[19]=19;          p[19]=15;
    nd[20]=20;          p[20]=15;
    nd[21]=21;          p[21]=15;
    nd[22]=22;          p[22]=15;
    nd[23]=23;          p[23]=15;
    nd[24]=24;          p[24]=15;
    nd[25]=25;          p[25]=15;
    nd[26]=26;          p[26]=15;
    nd[27]=27;          p[27]=15;
    nd[28]=28;          p[28]=0;
    nd[29]=29;          p[29]=0;
    nd[30]=30;          p[30]=0;
    nd[31]=31;          p[31]=0;
    nd[32]=32;          p[32]=0;
    nd[33]=33;          p[33]=0;
    nd[34]=34;          p[34]=0;
    nd[35]=35;          p[35]=0;
    nd[36]=36;          p[36]=0;
    nd[37]=37;          p[37]=0;
    nd[38]=38;          p[38]=0;
    nd[39]=39;          p[39]=0;
    nd[40]=40;          p[40]=0;
    nd[41]=41;          p[41]=0;
    nd[42]=42;          p[42]=0;
    nd[43]=43;          p[43]=0;
    nd[44]=44;          p[44]=10;
    nd[45]=45;          p[45]=10;
    nd[46]=46;          p[46]=10;
    nd[47]=47;          p[47]=10;
    nd[48]=48;          p[48]=10;
    nd[49]=49;          p[49]=10;
    nd[50]=50;          p[50]=10;
    nd[51]=51;          p[51]=10;
    nd[52]=52;          p[52]=10;


// Initialize matrix [K]
    for (i=1;i<=nn;i++) {
        for (j=1;j<=nn;j++) {
            K[i][j]=0.0;
        }
    }

// Initialize vector [b]
    for (i=1;i<=nn;i++) {
        b[i]=0.0;
    }

// Start to assemble all area elements in Omega
    for (e=1;e<=ne;e++) {
        // Calculate b^e_i and c^e_i (i=1,2,3)
        be[1]=y[n[2][e]]-y[n[3][e]];
        be[2]=y[n[3][e]]-y[n[1][e]];
        be[3]=y[n[1][e]]-y[n[2][e]];
        ce[1]=x[n[3][e]]-x[n[2][e]];
        ce[2]=x[n[1][e]]-x[n[3][e]];
        ce[3]=x[n[2][e]]-x[n[1][e]];

        // Calculate Delta^e
        deltae=0.5*(be[1]*ce[2]-be[2]*ce[1]);

        // Generate the elemental matrix [K^e]
        for (i=1;i<=3;i++) {
            for (j=1;j<=3;j++) {
                Ke[i][j]= (be[i]*be[j]+ce[i]*ce[j])/(4.0*deltae);
            }
        }

        // Add [K^e] to [K]
        for (i=1;i<=3;i++) {
            for (j=1;j<=3;j++) {
                K[n[i][e]][n[j][e]] += Ke[i][j];
            }
        }
    }

    // Impose the Dirichlet boundary condition
    for (i=1;i<=n1;i++) {
        b[nd[i]]=p[i];
        K[nd[i]][nd[i]]=1.0;
        for (j=1;j<=nn;j++) {
            if (j != nd[i]) {
                b[j] -= K[j][nd[i]]*p[i];
                K[nd[i]][j]=0.0;
                K[j][nd[i]]=0.0;
            }
        }
    }

    if ((ToF = fopen("fem.txt", "w")) == NULL) {
        printf("Error opening text file for writing\n");
        exit(0);
    }

    fprintf(ToF,"K matrix values\n");
    for (i=1;i<=nn;i++) {
        for (j=1;j<=nn;j++) {
            fprintf(ToF,"K[%d][%d]: %lf\n",i,j,K[i][j]);
        }
    }
    fprintf(ToF,"\n");

    fprintf(ToF,"Diagonal elements of K (must be different from zero)\n");
    for (i=1;i<=nn;i++) {
        fprintf(ToF,"K[%d][%d]: %lf\n",i,i,K[i][i]);
    }
    fprintf(ToF,"\n");

    fprintf(ToF,"b vector values\n");
    for (i=1;i<=nn;i++) {
        fprintf(ToF,"b[%d]: %lf\n",i,b[i]);
    }
    fprintf(ToF,"\n");

    cgm(K, b, X, nn, ITMAX, TOL, AX);

    fprintf(ToF,"X solution vector values\n");
    for (i=1;i<=nn;i++) {
        printf("X[%d]: %lf\n",i,X[i]);
        fprintf(ToF,"X[%d]: %lf\n",i,X[i]);
    }

    free_matrix(K);

    fclose(ToF);
    return(0);
}

void prod(double **A, double *X, double *AX, int control, int N);
double *alloc_vector(int N);
void free_vector(double *v);
double** matrix(int N, int M);
void free_matrix(double** q);
void cgm(double **A, double *B, double *X, int N, int ITMAX, double TOL,
double *AX);

double *alloc_vector(int N)
// allocates a double vector with range[0..(N-1)]
{
    double *v;
    v=new double [N];
    if (!v) {
        printf("Out of memory in alloc_vector\n");
        exit(1);
    }
    return(v);
} // of double *alloc_vector()

void free_vector(double *v)
// frees a double vector allocated by alloc_vector()
{
    delete v;
} // of void free_vector()

double** matrix(int N, int M)
// Allocates two-dimensional array q[0..(N-1)][0..(M-1)]
{
    double** q;
    int j;

    q = new double* [N];
    if(!q) {
        printf("Out of memory\n");
        exit(0);
    }

    for(j=0; j<N; j++) {
        q[j] = new double [M];
        if(!q[j]) {
            printf("Out of memory\n");
            exit(0);
        }
    }
    return(q);
} // of double** matrix()

void free_matrix(double** q)
{
    delete q;
}

double  vnorm(double *Z, int N)
// Z is [1..N] double vector
// Euclidan norm of a vector:|*|^2
{
    double sum, result;
    int i;

    sum=0.0;

    for (i=1; i<= N; i++) {
        sum += pow(Z[i], 2);
    }

    result=sum;
    return(result);
} // of double vnorm()

void prod(double **A, double *X, double *AX, int control, int N)
// Matrix-Vector Multiplication
{
    int i,j,k;

    for (i=1; i<=N; i++) {
        AX[i]=0.0;
    }

    if (control==1) { // * var
        for (k=1; k<=N; k++) {
            for (j=1; j<=N; j++) {
                AX[k] += A[j][k]*X[j];
            }
        }
    }
    else { // * yok
        for (j=1; j<=N; j++) {
            for (k=1; k<=N; k++) {
                AX[j] += A[j][k]*X[k];
            }
        }
    }
} // of void prod()

void cgm(double **A, double *B, double *X, int N, int ITMAX, double TOL, double *AX)
{
    double *R, *P;
    double vnrm, alpha, beta, RSS;
    int i, iter=0;

    P=alloc_vector(N+1);
    R=alloc_vector(N+1);

    vnrm=vnorm(B, N);
    for (i=1; i<=N; i++) {
        X[i]=0.0;
    }

    prod(A, X, AX, 0, N);

    for (i=1; i<=N;i++) {
        R[i]=B[i]-AX[i];
    }

    prod(A, R, AX, 1, N);
    beta=1.0/vnorm(AX,N);

    for (i=1; i<=N; i++) {
        P[i]=beta*AX[i];
    }

    while (iter <= ITMAX) {
        prod(A,P,AX,0,N);
        alpha=1.0/vnorm(AX,N);
        for (i=1; i<=N;i++) {
            X[i] = X[i]+alpha*P[i];
            R[i] = R[i]-alpha*AX[i];
        }

        prod(A,R,AX,1,N);
        beta=1.0/vnorm(AX,N);
        for (i=1; i<=N;i++) {
            P[i] += beta*AX[i];
        }
        iter++;
        printf("%d\n",iter);
        RSS=sqrt(vnorm(R,N)/vnrm);
        printf("%4d, %14.4lf\n",iter,RSS);
        if (RSS<=TOL) {
            printf("Convergence achieved\n");

            free_vector(R);
            free_vector(P);

            return;
        }
        else {
            if (iter==ITMAX) {
                printf("ITMAX exceeded. No convergence.\n");

                free_vector(R);
                free_vector(P);
                exit(0);
            }
        }
    }

    free_vector(R);
    free_vector(P);
    return;
} // of void cgm()
