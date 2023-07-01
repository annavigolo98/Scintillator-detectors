struct slimport_data_t {
	ULong64_t	timetag; //time stamp
	UInt_t		baseline;
	UShort_t	qshort; //integration with shorter time
	UShort_t	qlong; //integration with longer time
	UShort_t	pur;
	UShort_t	samples[4096];
	UShort_t        Energy;
	ULong64_t       Timestamp;
};

//subtracts the background histogram 
//acq_ch0   : timetag/l:baseline/i:qshort/s:qlong/s:pur/s: l=ulong i=uint s=ushort


void readchannel()
{

 
 //data input for ch0 and ch1
  
  vector<const char*> dati_in1({"CH1_Run72.root","CH1_Run73.root","CH1_Run74.root","CH1_Run75.root","CH1_Run76.root","CH1_Run77.root"});
  
  //integral thresholds
  
  
  
  vector<double> sogliesx_ch1 = {1900,1920,1900,1920,1900,1900};
  vector<double> sogliedx_ch1 = {2060,2040,2060,2040,2060,2060};
  
  
  //TCanvas 
  
  
  
  TCanvas* tc= new TCanvas("tc","histo",2400,800);
  tc->Divide(3,2); 
 
for(int a=0; a<dati_in1.size();a++)
 {
    
    
     
  slimport_data_t indata1;
  slimport_data_t indata2;
  
   
 TFile *infile1 = new TFile(dati_in1[a]); //channel 1 LaBr det
 TTree *intree1 = (TTree*)infile1->Get(Form("Data"));
 TBranch *energybranch1 = intree1->GetBranch(Form("Energy"));
 TBranch *timebranch1= intree1->GetBranch(Form("Timestamp"));
 
 energybranch1->SetAddress(&indata1.Energy);
 timebranch1->SetAddress(&indata1.Timestamp);
  
  
 int n1=intree1->GetEntries();
 
 
 TFile *infile2 = new TFile("CH1_Runbkg.root");  //Run background CH1 of 24/11/22
 TTree *intree2 = (TTree*)infile2->Get(Form("Data"));
 TBranch *energybranch2 = intree2->GetBranch(Form("Energy"));
 TBranch *timebranch2= intree2->GetBranch(Form("Timestamp"));
 
 energybranch2->SetAddress(&indata2.Energy);
 timebranch2->SetAddress(&indata2.Timestamp);
  
  
 int n2=intree2->GetEntries();
 
 
 
 //energy histogram 
 
 TH1D *h_spectrum= new TH1D("h_spectrum","decayspectrum",16383, -0.5, 16382.5);
 //TH1D *h_spectrum= new TH1D("h_spectrum","decayspectrum",100,6500 ,7500 );
 for(int i=0;i<n1;i++)
 { 
   energybranch1->GetEntry(i);
   h_spectrum->Fill(indata1.Energy);
   
  };
     
h_spectrum->SetTitle(Form("Energy spectrum _%d",a));
h_spectrum->GetXaxis()->SetTitle("Energy (ch)");
h_spectrum->GetYaxis()->SetTitle("counts");
h_spectrum->SetLineColor(2);     

 

 //last timestamp of signal and background for weights of the  background spectrum 
 
 


//last timestamp spectrum
timebranch1->GetEntry(n1-1);
double t_last_spectrum=indata1.Timestamp;  //in secondi
//cout<<"Last timestamp spectrum "<<endl;
//cout<<t_last_spectrum<<endl; 


timebranch2->GetEntry(n2-1);
double t_last_bkg=indata2.Timestamp; 

//cout<<"Last timestamp bkg"<<endl;
//cout<<t_last_bkg<<endl;


//weigth
double w=t_last_spectrum/t_last_bkg;
//cout<<w<<endl;



//  background histogram




 TH1D *h_spectrum_bkg= new TH1D("h_spectrum_bkg","decayspectrum_bkg",16383, -0.5, 16382.5);
 //TH1D *h_spectrum_bkg= new TH1D("h_spectrum_bkg","decayspectrum",100,6500 ,7500 );
 for(int i=0;i<n2;i++)
 { 
   energybranch2->GetEntry(i);
   h_spectrum_bkg->Fill(indata2.Energy,w);
   
   
  };



     
h_spectrum_bkg->SetTitle(Form("Background spectrum _%d",a));
h_spectrum_bkg->GetXaxis()->SetTitle("Energy (ch)");
h_spectrum_bkg->GetYaxis()->SetTitle("counts");
h_spectrum_bkg->SetLineColor(2);   
h_spectrum_bkg->SetLineColor(3);






//  background subtraction end plote 


tc->cd(a+1);

h_spectrum->Draw();



h_spectrum_bkg->Draw("same");




//bkg subtraction
TH1*hfin = (TH1*)(h_spectrum->Clone("h_fin"));
hfin->Add(h_spectrum_bkg, -1.0);


hfin->SetTitle(Form("Energy total spectrum_%d",a));
hfin->GetXaxis()->SetTitle("Energy (ch)");
hfin->GetYaxis()->SetTitle("counts");
hfin->SetLineColor(2);   
hfin->SetYTitle("Counts");
hfin->SetLineColor(4);



hfin->Draw("same"); 

//integral thresholds for the net area calculation

double x_in= hfin->GetXaxis()->FindBin(sogliesx_ch1[a]);
double x_fin= hfin->GetXaxis()->FindBin(sogliedx_ch1[a]);

cout<<"estremi  "<<x_in<<"  "<<x_fin<<endl;

double net_area= hfin->Integral(x_in,x_fin);
double net_area_err=sqrt(net_area);

//double net_area2=hfin->Integral(1920,2060);
//double net_area2_err=sqrt(net_area2);

cout<<"Net area under the peak  "<<a<<endl;
cout<<net_area<<" pm "<<net_area_err<<endl;

//cout<<"Net area under the peak 2"<<endl;
//cout<<net_area2<<" pm "<<net_area2_err<<endl;

}  //for a 















  };
