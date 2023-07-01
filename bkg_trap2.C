//macro per sottrarre il backgound (metodo trapezio)


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



vector <double> sxch0({1640.,1650.,1650.,1650.,1660.,1650.}); //ch0
vector <double> dxch0({1760.,1760.,1760.,1760.,1765.,1760.});

vector <double> sxch1({1730.,1735.,1730.,1735.,1730.,1740.});  //ch1
vector <double> dxch1({1860.,1860.,1860.,1860.,1850.,1860.});
  
  TCanvas* tc= new TCanvas("tc","histo",2400,800);
  //tc->Divide(3,2); 
  
    for(int a=0; a<dati_in0.size();a++)
    {
	
	// variables
	slimport_data_t indata1;
	slimport_data_t indata2;
	
	TFile *infile = new TFile(dati_in0[a]);   //ch0
	
	TTree *intree = (TTree*)infile->Get("Data"); 
	TBranch *inbranch = intree->GetBranch("Energy"); 
	TBranch *inbranch_time = intree->GetBranch("Timestamp"); 
	inbranch->SetAddress(&indata1.energy);
	inbranch_time->SetAddress(&indata2.timetag);
	TH1D *h_spectrum = new TH1D("","Total spectrum", 16383, -0.5, 16382.5 );
	h_spectrum->SetTitle(Form("Picco 1275 keV Na_%d",a));
 	h_spectrum->GetXaxis()->SetTitle("Energy (ch)");
 	h_spectrum->GetYaxis()->SetTitle("counts");
	
	
		
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
	
	
	TF1* retta = new TF1("retta","((x-[0])/[1])*([2])+[3]",sx_max,dx_min);
	retta-> SetParameter(0,sx_max);
	retta-> SetParameter(1,dx_min-sx_max);
	retta-> SetParameter(2,mean_dx-mean_sx);
	retta-> SetParameter(3,mean_sx); 


	double bkg=((mean_sx+mean_dx)*(dx_min-sx_max))/2;
	double peakcounts = h_spectrum->Integral(sx_max,dx_min);
	
	
	//attività del Co oggi
	double activity_co = 93.56*1.e3;  //bq
	
	//attività del Na oggi bq 
	double activity_na = 23447.;  //bq
	
	int n=intree->GetEntries();
       intree->GetEntry(n-1);
        double t0=indata2.timetag;
	double aqtime=t0*1e-12;

	cout<<"mean parte sx  "<< mean_sx <<endl;
	cout<<"mean parte dx  "<< mean_dx <<endl;

	cout<<"Peak counts  "<<peakcounts << "  +/-  " << sqrt(peakcounts) << endl;
	cout<<"Background counts  "<<bkg << "  +/-  " << sqrt(bkg) << endl;   

	//Alternativa per calcolare i conteggi di background
	//cout <<"Background counts  " << retta->Integral(sx_max,dx_min) << endl;	



	cout<<"Net counts  " << (peakcounts-bkg) <<"  +/-  " << sqrt(peakcounts+bkg)<<endl;
	
	//efficienza per il sodio
	cout<<"Efficienza "<<(peakcounts-bkg)/(activity_na*0.904*aqtime)<<endl;
	
	//tc->cd(a+1);
	double min=1620; //ch0
	double max=1780;
	
	h_spectrum->GetXaxis()->SetRangeUser(min, max);
	h_spectrum->Draw();
	retta ->Draw("same");
	
	tc->SaveAs( Form("Gauss1275trap%d.pdf",a)   );
	
	
 	
}

}
