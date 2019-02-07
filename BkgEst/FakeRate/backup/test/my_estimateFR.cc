#include <TStyle.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1D.h>
#include <TGraphAsymmErrors.h>
#include <TMultiGraph.h>
#include <TLegend.h>
#include <TCanvas.h>
#include <TROOT.h>
#include <THStack.h>
#include <TMath.h>
#include <TText.h>
#include <TPad.h>
#include <TPaveText.h>
#include <TLorentzVector.h>
#include <TStopwatch.h>
#include <TColor.h>
#include <TLatex.h>
#include <TEfficiency.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include "../../interface/tdrstyle.C"
#include "../../interface/CMS_lumi.C"
using namespace std;

void setDataHist(TH1D* hist);
void setMCHist(TH1D* hist, const int& color);
void setFRHist(TH1D* hist);
//TH1D* FRByTemplate(TH1D** numerator, TH1D** denominator);
//TH1D* FRBytRatio(TH1D** numerator, TH1D** denominator);
TH1D* FRByTemplate(vector< TH1D* > numerator, vector< TH1D* > denominator, vector< TString > types);
TH1D* FRBytRatio(vector< TH1D* > numerator, vector< TH1D* > denominator, vector< TString > types);

void my_estimateFR()
{
	//const int ptbinnum_endcap = 9;
	//double ptbin_endcap[ptbinnum_endcap+1] = {47,52,60,70,80,90,100,150,200,500};
	//const int ptbinnum = 17;
	//double ptbin[ptbinnum+1] = {47,52,60,70,80,90,100,120,140,160,180,200,250,300,350,400,450,500};
	const int ptbinnum_endcap = 8;
	double ptbin_endcap[ptbinnum_endcap+1] = {52,60,70,80,90,100,150,200,500};
	const int ptbinnum = 16;
	double ptbin[ptbinnum+1] = {52,60,70,80,90,100,120,140,160,180,200,250,300,350,400,450,500};
	const int etabinnum = 2;
	double etabin[etabinnum+1] = {0,1.2,2.4};

	//Set types
	vector< TString > types;
	types.push_back("Data");
	types.push_back("QCD");
	types.push_back("WJets");
	types.push_back("DY");
	types.push_back("ttbar");
	types.push_back("tW");
	types.push_back("WW");
	types.push_back("WZ");
	types.push_back("ZZ");

	//////////////////////////
	// -- Get histograms -- //
	//////////////////////////
	vector<TH1D*> denominator_pt_fit_barrel;
	vector<TH1D*> denominator_pt_xsec_barrel;
	vector<TH1D*> denominator_pt_fit_endcap;
	vector<TH1D*> denominator_pt_xsec_endcap;
	vector<TH1D*> denominator_barrel;
	vector<TH1D*> denominator_endcap;
	vector<TH1D*> numerator_pt_barrel;
	vector<TH1D*> numerator_pt_endcap;

	vector< pair< vector<TH1D*>, TString > > VnT;
	VnT.push_back(make_pair( denominator_pt_fit_barrel, "denominator_pt_barrel" ));
	VnT.push_back(make_pair( denominator_pt_xsec_barrel, "denominator_pt_barrel" ));
	VnT.push_back(make_pair( denominator_pt_fit_endcap, "denominator_pt_endcap" ));
	VnT.push_back(make_pair( denominator_pt_xsec_endcap, "denominator_pt_endcap" ));
	VnT.push_back(make_pair( denominator_barrel, "denominator_barrel" ));
	VnT.push_back(make_pair( denominator_endcap, "denominator_endcap" ));
	VnT.push_back(make_pair( numerator_pt_barrel, "numerator_pt_barrel" ));
	VnT.push_back(make_pair( numerator_pt_endcap, "numerator_pt_endcap" ));

	TFile f_input("bin/histograms/Re_hist_test.root", "read");
	TH1D *h_med;
	
	for(int i=0; i<types.size(); i++)
	{
		h_med = (TH1D*)f_input.Get("denominator_pt_barrel_"+types[i]);
		denominator_pt_fit_barrel.push_back(h_med); h_med->Reset();

		h_med = (TH1D*)f_input.Get("denominator_pt_barrel_"+types[i]);
		denominator_pt_xsec_barrel.push_back(h_med); h_med->Reset();

		h_med = (TH1D*)f_input.Get("denominator_pt_endcap_"+types[i]);
		denominator_pt_fit_endcap.push_back(h_med); h_med->Reset();
		
		h_med = (TH1D*)f_input.Get("denominator_pt_endcap_"+types[i]);
		denominator_pt_xsec_endcap.push_back(h_med); h_med->Reset();
		
		h_med = (TH1D*)f_input.Get("denominator_barrel_"+types[i]);
		denominator_barrel.push_back(h_med); h_med->Reset();
		
		h_med = (TH1D*)f_input.Get("denominator_endcap_"+types[i]);
		denominator_endcap.push_back(h_med); h_med->Reset();
		
		h_med = (TH1D*)f_input.Get("numerator_pt_barrel_"+types[i]);
		numerator_pt_barrel.push_back(h_med); h_med->Reset();
		
		h_med = (TH1D*)f_input.Get("numerator_pt_endcap_"+types[i]);
		numerator_pt_endcap.push_back(h_med); h_med->Reset();
	}

	//Set histograms
	for(int i=0;i<types.size();i++)
	{
		if( type[i] == "Data" )
		{
			setDataHist( denominator_pt_fit_barrel[i] );
			setDataHist( denominator_pt_xsec_barrel[i] );
			setDataHist( denominator_pt_fit_endcap[i] );
			setDataHist( denominator_pt_xsec_endcap[i] );
			setDataHist( denominator_barrel[i] );
			setDataHist( denominator_endcap[i] );
			setDataHist( numerator_pt_barrel[i] );
			setDataHist( numerator_pt_endcap[i] );
		}
		else
		{
			setMCHist( denominator_pt_fit_barrel[i], i );
			setMCHist( denominator_pt_xsec_barrel[i], i );
			setMCHist( denominator_pt_fit_endcap[i], i );
			setMCHist( denominator_pt_xsec_endcap[i], i );
			setMCHist( denominator_barrel[i], i );
			setMCHist( denominator_endcap[i], i );
			setMCHist( numerator_pt_barrel[i], i );
			setMCHist( numerator_pt_endcap[i], i );
		}
	}

	/////////////////////////////////
	// -- Measure the Fake Rate -- //
	/////////////////////////////////
	//Proportion in Data. Order is same with types. 
	vector<Double_t> values_barrel;
	values_barrel.push_back(2.3647e+07);
	values_barrel.push_back(1.2249e+07);
	values_barrel.push_back(4.6877e+06);
	values_barrel.push_back(1.4304e+06);
	values_barrel.push_back(6.7331e+04);
	values_barrel.push_back(9.3232e+04);
	values_barrel.push_back(3.4202e+04);
	values_barrel.push_back(1.1022e+04);

	vector<Double_t> values_endcap;
	values_endcap.push_back(1.2133e+07);
	values_endcap.push_back(9.6993e+06);
	values_endcap.push_back(3.6501e+06);
	values_endcap.push_back(5.8866e+05);
	values_endcap.push_back(2.6427e+04);
	values_endcap.push_back(6.6023e+04);
	values_endcap.push_back(2.3330e+04);
	values_endcap.push_back(7.6996e+03);

	for(int i=0; i<types.size(); i++)
	{
		if( types[i] == "Data" ) continue;

		Double_t norm_fit_barrel = 0, norm_fit_endcap = 0;

		norm_fit_barrel = values_barrel[i]/(denominator_barrel[i]->Integral());
		norm_fit_endcap = values_endcap[i]/(denominator_endcap[i]->Integral());

		denominator_barrel[i]->Scale(norm_fit_barrel);
		denominator_pt_fit_barrel[i]->Scale(norm_fit_barrel);

		denominator_endcap[i]->Scale(norm_fit_endcap);
		denominator_pt_fit_endcap[i]->Scale(norm_fit_endcap);
	}

	TH1D* FR_template_barrel = (TH1D*)FRByTemplate(numerator_pt_barrel, denominator_pt_fit_barrel, types);
	TH1D* FR_template_endcap = (TH1D*)FRByTemplate(numerator_pt_endcap, denominator_pt_fit_endcap, types);

	TH1D* FR_xsec_barrel = (TH1D*)FRBytRatio(numerator_pt_barrel, denominator_pt_xsec_barrel, types);
	TH1D* FR_xsec_endcap = (TH1D*)FRBytRatio(numerator_pt_endcap, denominator_pt_xsec_endcap, types);

	////////////////////
	// -- Set plot -- //
	////////////////////
	int W = 1200;
	int H = 1200;

	int H_ref = 1200;
	int W_ref = 1200;

	// references for Top, Bottom, Left, Right
	float T = 0.08*H_ref;
	float B = 0.12*H_ref;
	float L = 0.12*W_ref;
	float R = 0.04*W_ref;

	lumi_13TeV = "35.9 fb^{-1}";
	writeExtraText = true;
	extraText = "Preliminary";
	drawLogo = false;
	tdrGrid(true);
	lumiTextSize = 0.5;
	cmsTextSize = 0.75;

	TH1D* ptFrame = new TH1D("ptFrame","",8,47,500);
	ptFrame->SetStats(kFALSE);
	ptFrame->GetXaxis()->SetTitle("p_{T}[GeV]");
	ptFrame->GetYaxis()->SetTitle("Fake Rate");

	ptFrame->SetMinimum(0);
	ptFrame->SetMaximum(1.0); 
	ptFrame->GetXaxis()->SetTitleOffset(1);
	ptFrame->GetYaxis()->SetTitleOffset(1.1);
	ptFrame->GetXaxis()->SetTitleSize(0.05);
	ptFrame->GetYaxis()->SetTitleSize(0.05);  
	ptFrame->GetXaxis()->SetLabelSize(0.035);
	ptFrame->GetYaxis()->SetLabelSize(0.035); 
	ptFrame->GetXaxis()->SetMoreLogLabels(); 

	TCanvas* canv = new TCanvas("canv","",1200,1200);
	canv->SetFillColor(0);
	canv->SetLeftMargin( L/W );
	canv->SetRightMargin( R/W );
	canv->SetTopMargin( T/H );
	canv->SetBottomMargin( B/H );

	FR_template_barrel->SetMarkerSize(3);
	FR_template_endcap->SetMarkerSize(3);
	FR_xsec_barrel->SetMarkerSize(3);
	FR_xsec_endcap->SetMarkerSize(3);

	FR_template_barrel->SetLineColor(1);
	FR_template_barrel->SetLineWidth(2);
	FR_template_barrel->SetMarkerStyle(20);
	FR_template_barrel->SetMarkerColor(1);

	FR_xsec_barrel->SetLineColor(2);
	FR_xsec_barrel->SetLineWidth(2);
	FR_xsec_barrel->SetMarkerStyle(21);
	FR_xsec_barrel->SetMarkerColor(2);

	FR_template_endcap->SetLineColor(1);
	FR_template_endcap->SetLineWidth(2);
	FR_template_endcap->SetMarkerStyle(20);
	FR_template_endcap->SetMarkerColor(1);

	FR_xsec_endcap->SetLineColor(2);
	FR_xsec_endcap->SetLineWidth(2);
	FR_xsec_endcap->SetMarkerStyle(21);
	FR_xsec_endcap->SetMarkerColor(2);

	canv->cd();
	canv->SetLogx();

	TLegend* legend2 = new TLegend(.45,.65,.75,.89);
	legend2->AddEntry(FR_template_barrel,"Template fitting");
	legend2->AddEntry(FR_xsec_barrel,"Ratio method");
	legend2->SetBorderSize(0);

	ptFrame->Draw();
	CMS_lumi(canv,4,11);
	canv->Update();
	canv->RedrawAxis();
	canv->GetFrame()->Draw();
	FR_template_barrel->Draw("EPSAME");
	FR_xsec_barrel->Draw("EPSAME");
	legend2->Draw("SAME");
	canv->Print("bin/print/FR_Barrel.pdf");

	canv->Clear();
	ptFrame->Draw();
	CMS_lumi(canv,4,11);
	canv->Update();
	canv->RedrawAxis();
	canv->GetFrame()->Draw();
	FR_template_endcap->Draw("EPSAME");
	FR_xsec_endcap->Draw("EPSAME");
	legend2->Draw("SAME");
	canv->Print("bin/print/FR_Endcap.pdf");

	TFile* g = new TFile("bin/result/fakerate.root","RECREATE");
	FR_template_barrel->Write();
	FR_template_endcap->Write();
	FR_xsec_barrel->Write();
	FR_xsec_endcap->Write();
	g->Close();

	cout<<"Template Barrel"<<endl;
	for(int i=1; i<ptbinnum+1; i++)
	{
		if(i!=1) cout<<",";
		cout<<FR_template_barrel->GetBinContent(i);
	}
	cout<<endl;
	cout<<"Template Endcap"<<endl;
	for(int i=1; i<ptbinnum_endcap+1; i++)
	{
		if(i!=1) cout<<",";
		cout<<FR_template_endcap->GetBinContent(i);
	}
	cout<<endl;
	cout<<"Ratio Barrel"<<endl;
	for(int i=1; i<ptbinnum+1; i++)
	{
		if(i!=1) cout<<",";
		cout<<FR_xsec_barrel->GetBinContent(i);
	}
	cout<<endl;
	cout<<"Ratio Endcap"<<endl;
	for(int i=1; i<ptbinnum_endcap+1; i++)
	{
		if(i!=1) cout<<",";
		cout<<FR_xsec_endcap->GetBinContent(i);
	}
	cout<<endl;
}

void setDataHist(TH1D* hist)
{
	hist->SetLineWidth(2);
	hist->SetMarkerStyle(33);
	hist->SetMarkerSize(3);
	hist->SetStats(kFALSE);
	hist->Sumw2();
}

void setMCHist(TH1D* hist, const int& color)
{
	hist->SetFillColor(color+2);
	hist->SetStats(kFALSE);
	hist->Sumw2();
}

TH1D* FRByTemplate(vector< TH1D* > numerator, vector< TH1D* > denominator, vector< TString > types)
{
	//Set order
	int i_data = -9999, i_dy = -9999, i_qcd = -9999;
	for(int i=0; i<types.size(); i++)
	{
		if( types[i] == "Data" ) i_data = i;
		else if( types[i] == "DY" ) i_dy = i;
		else if( types[i] == "QCD" ) i_qcd = i;
	}

	TString name = ( ((TString)(denominator[i_qcd]->GetName())).Contains("barrel") ) ? "FR_template_barrel" : "FR_template_endcap";

	TH1D* num = numerator[i_qcd]->Clone(name);
	TH1D* den = numerator[i_dy]->Clone(name+"_");

	num->Multiply(numerator[i_data]);

	for(int i=0; i<types.size(); i++)
	{
		if( types[i] == "Data" || types[i] == "DY" ) continue;
		den->Add(numerator[i]);
	}

	den->Multiply(denominator[i_qcd]);

	num->Divide(den);

	delete den;
	return num;
}

TH1D* FRBytRatio(vector< TH1D* > numerator, vector< TH1D* > denominator, vector< TString > types)
{
	//Set order
	int i_data = -9999, i_dy = -9999, i_qcd = -9999;
	for(int i=0; i<types.size(); i++)
	{
		if( types[i] == "Data" ) i_data = i;
		else if( types[i] == "DY" ) i_dy = i;
		else if( types[i] == "QCD" ) i_qcd = i;
	}

	TString name = ( ((TString)(denominator[i_qcd]->GetName())).Contains("barrel") ) ? "FR_xsec_barrel" : "FR_xsec_endcap";

	TH1D* num = denominator[i_dy]->Clone(name);
	TH1D* den = numerator[i_dy]->Clone(name+"_");

	for(int i=0; i<types.size(); i++)
	{
		if( types[i] == "Data" || types[i] == "DY") continue;
		num->Add(denominator[i]);
		den->Add(numerator[i]);
	}

	num->Multiply(numerator[i_qcd]);
	num->Multiply(numerator[i_data]);

	den->Multiply(denominator[i_qcd]);
	den->Multiply(denominator[i_data]);

	num->Divide(den);

	delete den;
	return num;
}
