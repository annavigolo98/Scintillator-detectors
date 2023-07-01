//Subtracts the background using a Fermi function for a given channel of the digitizer (ch0 or ch1)


struct slimport_data_t {
	ULong64_t	timetag; //time stamp
	UShort_t	channel;
	UShort_t	energy; 
	};
	
void bkg()
{


//input data ch0 and ch1
vector<const char*> dati_in0({"ch0_0.root","ch0_1.root","ch0_2.root","ch0_3.root","ch0_4.root","ch0_5.root"});
vector<const char*>    dati_in1({"ch1_0.root","ch1_1.root","ch1_2.root","ch1_3.root","ch1_4.root","ch1_5.root"});


// values in channels for the peaks at 1275+511 keV of lantanium detector 
vector <double> sxch0({2320.,2300.,2300.,2300.,2300.,2300.}); //ch0
vector <double> dxch0({2450.,2450.,2450.,2400.,2400.,2400.});

vector <double> sxch1({2460.,2460.,2460.,2460.,2460.,2460.});  //ch1
vector <double> dxch1({2590.,2590.,2590.,2590.,2590.,2590.});
  
  TCanvas* tc= new TCanvas("tc","histo",2400,800);
   
  
    for(int a=0; a<dati_in0.size();a++)
    {
	
	// variables
	slimport_data_t indata1;
	slimport_data_t indata2;
	
	TFile *infile = new TFile(dati_in1[a]);  //ch1
	TTree *intree = (TTree*)infile->Get("Data"); 
	TBranch *inbranch = intree->GetBranch("Energy"); 
	TBranch *inbranch_time = intree->GetBranch("Timestamp"); 
	inbranch->SetAddress(&indata1.energy);
	inbranch_time->SetAddress(&indata2.timetag);
	
	TH1D *h_spectrum = new TH1D("","Total spectrum", 16383, -0.5, 16382.5 );
	h_spectrum->SetTitle(Form("Picco 511 keV Na_%d",a));
 	h_spectrum->GetXaxis()->SetTitle("Energy (ch)");
 	h_spectrum->GetYaxis()->SetTitle("counts");
		
	// histogram filling
	for (int i=0; i<inbranch->GetEntries(); i++) {
		inbranch->GetEntry(i);
		h_spectrum->Fill(indata1.energy); 
		
	//	inbranch_time->GetEntry(i);	
	}	
	
	//itervals  sx and dx of the peak to which subtract the background
	int sx_min=sxch1[a]-50.; //ch1
	int sx_max=sxch1[a];
	
	int dx_min=dxch1[a]; //ch1
	int dx_max=dxch1[a]+50;
	
	//gaussian fit between sx_max and dx_min
	TF1 *g   = new TF1("g1","gaus",sx_max,dx_min);
	h_spectrum->Fit(g,"R");
	
	
	//double mean_gauss=1.70667e+03;
	double mean_gauss = g->GetParameter(1);
	cout<<"mean  "<< mean_gauss << endl;
	
	double sx=0;
	double mean_sx=0;


//part at the left side of the peak
	for(int i=sx_min; i<sx_max;i++){
	sx+=h_spectrum->GetBinContent(i);
	mean_sx= sx/(sx_max-sx_min);
	}
	
	double dx=0;
	double mean_dx=0;

	
//part at the right side of the peak
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
	
	
	
	//activity Na today bq 
	double activity_na = 23447.;
	
	int n=intree->GetEntries();
       intree->GetEntry(n-1);
        double t0=indata2.timetag;
	double aqtime=t0*1e-12;

	cout<<"mean parte sx  "<< mean_sx <<endl;
	cout<<"mean parte dx  "<< mean_dx <<endl;

	cout<<"Peak counts  "<<peakcounts << "  +/-  " << sqrt(peakcounts) << endl;
	cout<<"Background counts  "<<bkg << "  +/-  " << sqrt(bkg) << endl;   

		



	cout<<"Net counts  " << (peakcounts-bkg) <<"  +/-  " << sqrt(peakcounts+bkg)<<endl;
	//efficiency 1275 and 511 keV  branching ratio: 0.904
	
	
	cout<<"Efficienza "<<(peakcounts-bkg)/(activity_na*aqtime*0.904)<<endl; 
	
	
	
	double min=2300; //ch1
	double max=2700;
	
	h_spectrum->GetXaxis()->SetRangeUser(min, max);
	
	
	h_spectrum->Draw();
	fermi_func->Draw("same");
	
	
	}
	
		
 	
}
