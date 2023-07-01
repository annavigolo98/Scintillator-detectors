//macro per sottrarre il backgound (funzione di Fermi)

#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

struct slimport_data_t {
	ULong64_t	timetag; //time stamp
	UShort_t	channel;
	UShort_t	energy; 
	};
	
void bkg()
{


//vettori dei dati di input per ch0 e ch1
vector<const char*> dati_in0({"ch0_0.root","ch0_1.root","ch0_2.root","ch0_3.root","ch0_4.root","ch0_5.root"});
vector<const char*>    dati_in1({"ch1_0.root","ch1_1.root","ch1_2.root","ch1_3.root","ch1_4.root","ch1_5.root"});


  
 
  
  

  
  
//picco a 1275 keV con ch0 


for(int a=0; a<dati_in0.size();a++)
    {
       TH1D *randomint = new TH1D("randomint","Counts under peak", 40, 0, 80000 );
       TCanvas* tc= new TCanvas("tc","histo",2400,800);
       
for(int u=0;u<100;u++)
{  

    vector <double> sxch0({1640.,1650.,1650.,1650.,1660.,1650.}); //ch0
    vector <double> dxch0({1760.,1760.,1760.,1760.,1765.,1760.});

    vector <double> sxch1({1730.,1735.,1730.,1735.,1730.,1740.});  //ch1
    vector <double> dxch1({1860.,1860.,1860.,1860.,1850.,1860.});

    double randsx=gRandom->Uniform(0.,50.);
    double randdx=gRandom->Uniform(0.,50.);
    
 
    
    
         
         
          sxch0[a]=sxch0[a]-randsx; //ch0
          dxch0[a]=dxch0[a]+randdx;
	
	// variables
	slimport_data_t indata1;
	slimport_data_t indata2;
	
	TFile *infile = new TFile(dati_in0[a]);  //ch0
	
	TTree *intree = (TTree*)infile->Get("Data"); 
	TBranch *inbranch = intree->GetBranch("Energy"); 
	TBranch *inbranch_time = intree->GetBranch("Timestamp"); 
	inbranch->SetAddress(&indata1.energy);
	inbranch_time->SetAddress(&indata2.timetag);
	
	TH1D *h_spectrum = new TH1D("h_spectrum","Total spectrum", 16383, -0.5, 16382.5 );
	
		
	// histogram filling
	for (int i=0; i<inbranch->GetEntries(); i++) {
		inbranch->GetEntry(i);
		h_spectrum->Fill(indata1.energy); 
		
	//	inbranch_time->GetEntry(i);	
	}	
	
	//itervalli a sx e dx del picco in cui vado a vedere il bkg
	int sx_min=sxch0[a]-50.; //ch0
	int sx_max=sxch0[a];
	
	int dx_min=dxch0[a]; //ch0
	int dx_max=dxch0[a]+50;
	
	//fit guassiano del picco, tra sx_max e dx_min	
	TF1 *g   = new TF1("g1","gaus",sx_max,dx_min);
	h_spectrum->Fit(g,"R");
	
	
	//double mean_gauss=1.70667e+03;
	double mean_gauss = g->GetParameter(1);
	cout<<"mean  "<< mean_gauss << endl;
	
	double sx=0;
	double mean_sx=0;


//primo for per la parte a sinistra del picco	
	for(int i=sx_min; i<sx_max;i++){
	sx+=h_spectrum->GetBinContent(i);
	mean_sx= sx/(sx_max-sx_min);
	}
	
	double dx=0;
	double mean_dx=0;

	
//primo for per la parte a destra del picco	
	for(int i=dx_min; i<dx_max;i++){
	dx+=h_spectrum->GetBinContent(i);
	mean_dx= dx/(dx_max-dx_min);
	}	
	
	
	TF1* fermi_func = new TF1("step_sx","([0]/(1+e^(x-[1])))+[2]",sx_max,dx_min);
	fermi_func-> SetParameter(0,mean_sx-mean_dx);
	fermi_func-> SetParameter(1,mean_gauss);
	fermi_func-> SetParameter(2,mean_dx);
	
	
	double bkg=(fermi_func->Integral(sx_max,dx_min));
	double peakcounts = h_spectrum->Integral(sx_max,dx_min);
	
	
	//attività del Co oggi bq
	double activity_co = 93.56*1.e3;
	//attività del Na oggi bq 
	double activity_na = 23447.;
	
	int n=intree->GetEntries();
       intree->GetEntry(n-1);
        double t0=indata2.timetag;
	double aqtime=t0*1e-12;

	

	//Alternativa per calcolare i conteggi di background
	//cout <<"Background counts  " << retta->Integral(sx_max,dx_min) << endl;	


         randomint->Fill(peakcounts-bkg);
	
	
	
	
	
	
	
	
	} //fine for u
	
	randomint->SetTitle(Form("Time spectrum 1275 keV Na_%d",a));
 	randomint->GetXaxis()->SetTitle("Net counts under the peak");
 	randomint->GetYaxis()->SetTitle("counts");
	randomint->Draw();
	tc->SaveAs(Form("Integral_spectrumch0%d",a)); //ch0
	
	}//fine for a	
	
	
	
	
	
	
 	
}
