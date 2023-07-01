

//arctangent fit of the yield profile for the AN2000 accelerator calibration


void grafico(const char *namefile) {

TCanvas *c1= new TCanvas("c1");
TGraphErrors *d= new TGraphErrors(namefile,"%lg %lg %lg %lg");







d->SetMarkerStyle(6);
d->SetMarkerSize(1);
d->SetMarkerColor(1);
d->SetLineColor(1);




c1->SetGrid();
   
   
TMultiGraph  *mg  = new TMultiGraph();
mg->Add(d);
//mg->Add(d2);
mg->SetTitle("Resonance 992keV");
mg->GetXaxis()->SetTitle("NMR (10kGauss)");
mg->GetYaxis()->SetTitle("Yield");
mg->Draw("AP");


   


//fit with arctangent function


double sx= 0.1615;
double dx= 0.1645;

//double a_par=0.001684209; //par0

//double b_par=14925; //par1

//double c_par=-0.16287; //par2

//double d_par=0.00716067;  //par3
 
//plot with fit parameters

double a_par=1.77180e-03; 

double b_par=2.30763e+04; 

double c_par=-1.62907e-01; 

double d_par= 7.33238e-03;



TF1* arctan_func = new TF1("arctan_func","[0]*TMath::ATan( [1]*(x+[2]) )  + [3]",sx,dx);
	arctan_func-> SetParameter(0,a_par);	
	arctan_func-> SetParameter(1,b_par);
	arctan_func-> SetParameter(2,c_par);
	arctan_func-> SetParameter(3,d_par);


//d->Fit(arctan_func,"R");
arctan_func->Draw("same");

// residuals of the fit

const int h=d->GetN();
double resy[h];
double x[h];
double erry[h];

// f->Eval(x_val[i])

for (int i=0; i<d->GetN();i++)
{
   double yval=d->GetY()[i];
   double xval=d->GetX()[i];
   double yvalerr=d->GetEY()[i];
   //cout<<"X, Y, errY values "<<xval<<"   "<<yval<<"  "<<yvalerr<<endl;
   x[i]=xval;
   resy[i]=yval-arctan_func->Eval(xval);
   erry[i]=yvalerr;
   }
   
   
TCanvas *c2=new TCanvas();
c2->SetGrid();
TGraphErrors* res = new TGraphErrors(h,x,resy,0,erry);
res->GetXaxis()->SetTitle("NMR (10kGauss)");
res->GetYaxis()->SetTitle("Yield");
res->SetTitle("Residuals resonance at 992 keV");

res -> SetMarkerStyle(20);
res -> SetMarkerColor(1);
res -> SetLineColor(1);

res->Draw("APE");
TF1* retta_res = new TF1("retta_res","[0]",sx,dx);
retta_res->Draw("same");   


} 
