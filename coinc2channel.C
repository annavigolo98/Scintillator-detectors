#include <iostream>
#include <cmath>
#include <vector>
/*#include <Riostream.h>
#include <stdlib.h>
#include <TROOT.h>
#include <TSystem.h>
#include "TNtuple.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TF1.h"
#include "TLegend.h"
#include "TArrow.h"
#include "TLatex.h"
#include <TMath.h>
#include "TH1F.h"
#include <fstream>
#include <string>
#include <vector>
*/



struct slimport_data_t {
	ULong64_t	timetag; //time stamp
	UInt_t		baseline;
	UShort_t	qshort; //integration with shorter time
	UShort_t	qlong; //integration with longer time
	UShort_t	pur;
	UShort_t	samples[4096];
	UShort_t        Energy;
	ULong64_t         Timestamp;
};

//acq_ch0   : timetag/l:baseline/i:qshort/s:qlong/s:pur/s: l=ulong i=uint s=ushort

void readchannel()
{

//Le sigma dei 511 sono quelle trovate nel file centroidi

//vettori dei dati di input per ch0 e ch1
  vector<const char*> dati_in0({"ch0_0.root","ch0_1.root","ch0_2.root","ch0_3.root","ch0_4.root","ch0_5.root"});
  vector<const char*> dati_in1({"ch1_0.root","ch1_1.root","ch1_2.root","ch1_3.root","ch1_4.root","ch1_5.root"});
  
//vettori dei dati di output istogrammi picchi 511 per ch0 e ch1, istogrammi delle coincidenze tra i due canali, istogramma delta tempi  
  vector<const char*> coinc_ch0({"coinc_ch0_0.root","coinc_ch0_1.root","coinc_ch0_2.root","coinc_ch0_3.root","coinc_ch0_4.root","coinc_ch0_5.root"});
  vector<const char*> coinc_ch1({"coinc_ch1_0.root","coinc_ch1_1.root","coinc_ch1_2.root","coinc_ch1_3.root","coinc_ch1_4.root","coinc_ch1_5.root"});
  
  vector<const char*> coinc({"coinc_0.root","coinc_1.root","coinc_2.root","coinc_3.root","coinc_4.root","coinc_5.root"});
  vector<const char*> deltatempi({"deltat0.root","deltat1.root","deltat2.root","deltat3.root","deltat4.root","deltat5.root"});
  
  
  
  
//vettore delle mean e delle sigma dei centroidi del picco a 511 del ch0 + vettori delle soglie in energia  
  vector<double> mean_0={510.63,510.7,510.4,510.2,510.4,510.1};
  vector<double> tresigma_0={3*11.36,3*11.6,3*11.9,3*12.0,3*13.2,3*14.1};
  
  vector<double> sogliapicco10;
  vector<double> sogliapicco20;
  
  for(int i=0; i<mean_0.size(); i++){
  
   sogliapicco10.push_back(mean_0[i] - tresigma_0[i]);
   sogliapicco20.push_back(mean_0[i] + tresigma_0[i]);
   }

//vettore delle mean e delle sigma dei centroidi del picco a 511 del ch0 + vettori delle soglie in energia  
  vector<double> mean_1={510.96,510.3,510.1,510.0,509.0,509.2};
  vector<double> tresigma_1={3*11.08,3*11.9,3*11.9,3*12.4,3*12.7,3*12.9};
  
  vector<double> sogliapicco11;
  vector<double> sogliapicco21;
  
  vector<double> tempiacq;
  
  for(int i=0; i<mean_1.size(); i++){
   sogliapicco11.push_back(mean_1[i]-tresigma_1[i]);
   sogliapicco21.push_back(mean_1[i]+tresigma_1[i]);
  }
  
   
                TCanvas* tc= new TCanvas("tc","histo",2400,800);
                //tc->Divide(3,2);  MULTIGRAPH  
   
   
    for(int a=0; a<dati_in0.size();a++)
    {
      
      //int  a=3;
         
 	 slimport_data_t indata1;
 	 slimport_data_t indata2;
 
 	 slimport_data_t indata3;
 	 slimport_data_t indata4;
  
 
 
 	TFile *infile1 = new TFile(dati_in0[a]);
 	TTree *intree1 = (TTree*)infile1->Get(Form("Data"));
 	
 
 
 
 
 	TFile *infile2 = new TFile(dati_in1[a]);
 	TTree *intree2 = (TTree*)infile2->Get(Form("Data"));
 
 	
 
 

  	//parametri calibrazione ch0 (valori da calibrazione con na e co, entrambi i detector a 13cm)
 	double m0=0.7494;
 	double q0=-3.7;
 
 
 	//parametri calibrazione ch1 
 	double m1=0.706;
 	double q1=5.4;

 
 	vector <double> piccoenergy0;
 	vector <double> piccoenergy1;
 	vector <double> time0;
 	vector <double> time1;
        
        
        
 	double esoglia=10.0; //keV
 	double tsoglia=3.*1e4; //ch
 
  
 	double sogliadeltat=2.*1e6;; //soglia deltat coincidenze spettri
 
 	TH1D *h_spectrum0= new TH1D("h_spectrum0","decayspectrum",16383,-0.5, 16382.5);
	TH1D *h_spectrum1= new TH1D("h_spectrum1","decayspectrum",16383,-0.5, 16382.5);
	
	
 	TH1D *coinc_spectrum= new TH1D("coinc_spectrum","Coincidences 511keV peak",30,500,800);  //limiti in channel 30 500 800
 	
 	//coinc_spectrum->SetTitle(Form("Coincidences 511 keV Na_%d",a)); //MULTIGRAPH
 	coinc_spectrum->SetTitle("Coincidences 511 keV Na");
 	coinc_spectrum->GetXaxis()->SetTitle("Energy (Kev)");
 	coinc_spectrum->GetYaxis()->SetTitle("counts");
 	
 	
 	TH1D *deltat= new TH1D("Time spectrum","decayspectrum",70,-4e4,4e4);
 	
 	//deltat->SetTitle(Form("Time spectrum 511 keV Na_%d",a)); MULTIGRAPH
 	deltat->SetTitle("Time spectrum 511 keV Na");
 	deltat->GetXaxis()->SetTitle("Time (ps)");
 	deltat->GetYaxis()->SetTitle("counts");
 	
 	
 	TH2D *matrixcoinc= new TH2D("Coincidences","coincidences Na22",100,500,800,100,500,800); //histo bidimens con coincidenze ch0 e ch1 limiti in channel 100 500 800
 	matrixcoinc->GetXaxis()->SetTitle("Energy CH0 (Kev)");
 	matrixcoinc->GetYaxis()->SetTitle("Energy CH1 (KeV)");
 	//matrixcoinc->SetTitle(Form("Coincidences 511 keV Na_%d",a)); //MULTIGRAPH
 	matrixcoinc->SetTitle("Coincidences 511 keV Na");
 	
 	intree1->SetBranchAddress("Timestamp",&indata2.Timestamp);
        intree1->SetBranchAddress("Energy",&indata1.Energy);
        
        
        int n=intree1->GetEntries();
        intree1->GetEntry(n-1);
        
        //cout<<indata2.Timestamp<<endl;
        double t0=indata2.Timestamp;
        
        
 		for(int i=0;i<intree1->GetEntries();i++)
  		{     
     			intree1->GetEntry(i);
     			
     
      
     			if((indata1.Energy*m0+q0)>sogliapicco10[a] && (indata1.Energy*m0+q0)<sogliapicco20[a])
     			{
      			 piccoenergy0.push_back(indata1.Energy);  //in canali
       		 time0.push_back(indata2.Timestamp);
       		 h_spectrum0->Fill(indata1.Energy);  //in canali
                       }
                }

                
                
                intree2->SetBranchAddress("Timestamp",&indata4.Timestamp);
                intree2->SetBranchAddress("Energy",&indata3.Energy);
               
                
                int n2=intree2->GetEntries();
                intree2->GetEntry(n2-1);
                //cout<<indata4.Timestamp<<endl;
                double t1=indata4.Timestamp;
                
                if(t0>=t1)
                { tempiacq.push_back(t0*1e-12);   }
                else
                tempiacq.push_back(t1*1e-12);
                
 		for(int i=0; i<intree2->GetEntries();i++)          
     		{	
     			intree2->GetEntry(i);
     			
     			
     			if((indata3.Energy*m1+q1)>sogliapicco11[a] && (indata3.Energy*m1+q1)<sogliapicco21[a])
     			{
       		piccoenergy1.push_back(indata3.Energy);   //in canali
       		time1.push_back(indata4.Timestamp);
       		h_spectrum1->Fill(indata3.Energy);   //in canali
       		
     
    		       }
     
     
     
    		}
   
   
  
      
   
  /* 		for(int i=0;i<time1.size();i++)
  		 {
  		   cout<<time0[i]<<"  "<<time1[i]<<endl;
  		 }
   */
   
                 
                 
                
                
                
                
               // coincidenze
               int start1=0;
               
                for(int i=0;i<piccoenergy0.size();i++)
   		{
   		    
     			for(int j=start1;j<piccoenergy1.size();j++)
     			{
       
          	 	   if( abs(time0[i]-time1[j])<tsoglia /*&& abs(piccoenergy0[i]+piccoenergy1[j]-1022.)<5. */)
                	    {  
             	 	     coinc_spectrum->Fill(piccoenergy0[i]);   //in canali
             	 	     matrixcoinc->Fill(piccoenergy0[i],piccoenergy1[j]);   //in canali
             	 	     start1=j;
             	 	     break;
             	 	    }
       
                      }
               
                  }
                 
                 
                 int start2=0;
                
                
                // istogramma tempi
   		for(int i=0;i<piccoenergy0.size();i++)
   		{
     			for(int j=start2;j<piccoenergy1.size();j++)
     			{
     
     		 	
     
     		 	    if(time0[i]-time1[j]<sogliadeltat)
     		 	    {
     		 	    deltat->Fill(time0[i]-time1[j]);
     		 	    start2=j;
     		 	    break;
     		 	    }
       
                      }
               
                 }
   
      
        
        //tc->cd(a+1); MULTIGRAPH
        
        
        double min=-2e4; 
	double max=4e4;
	
	deltat->GetXaxis()->SetRangeUser(min, max);
        deltat->Draw();
        
        //coinc_spectrum->GetXaxis()->SetRangeUser(500,800);
        
        //TAxis *axis=coinc_spectrum->GetXaxis(); //CH0
   	//axis->SetLimits(axis->GetXmin()*m0+q0,axis->GetXmax()*m0+q0);
   	
   
   	
   	
        //coinc_spectrum->Draw();
        
        
        //TAxis *axis=matrixcoinc->GetXaxis(); //CH0
   	//axis->SetLimits(axis->GetXmin()*m0+q0,axis->GetXmax()*m0+q0);
   	
   	//TAxis *axis2=matrixcoinc->GetYaxis(); //CH1
   	//axis2->SetLimits(axis2->GetXmin()*m1+q1,axis2->GetXmax()*m1+q1);
        
        //matrixcoinc->Draw("colz");
        
	
 	 
 
 
 
 
 
	}  //for a 
	
	
	 //for(int i=0;i<tempiacq.size();i++)
 	// {
 	  //  cout<<tempiacq[i]<<endl;
 	    //}  	

	
tc->SaveAs("tc2.png");	



}
