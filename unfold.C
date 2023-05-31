
void unfold()
{
    TH1::SetDefaultSumw2();
    TH2::SetDefaultSumw2();
    TFile*inputFile = new TFile("unfold_histograms.root");
    TH1D*hReco = (TH1D*)inputFile->Get("reco");
    TH1D*hTrue = (TH1D*)inputFile->Get("true");
    TH1D*hBack = (TH1D*)inputFile->Get("back");
    TH1D*hData = (TH1D*)inputFile->Get("data");
    TH2D*hMatrix = (TH2D*)inputFile->Get("hMatrix");
    TUnfoldBinning*recoBinning = (TUnfoldBinning*)inputFile->Get("recoBinning");
    TUnfoldBinning*trueBinning = (TUnfoldBinning*)inputFile->Get("trueBinning");

    TFile*outputFile = new TFile("unfold_results.root","recreate");
    recoBinning->Write();
    trueBinning->Write();
    hReco->Write();
    hTrue->Write();
    hBack->Write();
    hData->Write();
    hMatrix->Write();

    TUnfold::ERegMode regMode = TUnfold::kRegModeCurvature;
    TUnfold::EConstraint constraintMode = TUnfold::kEConstraintNone;
    TUnfoldDensity::EDensityMode densityFlags = TUnfoldDensity::kDensityModeBinWidth;
    TUnfold::EHistMap outputMap = TUnfold::kHistMapOutputVert;
    TUnfoldDensity unfold(hMatrix,outputMap,regMode,constraintMode,densityFlags);
    unfold.SetInput(hReco);//the measured distribution
    double backScale = 1.0;
    double backScaleError = 0.0;//scale error for background
//    unfold.SubtractBackground(hBack,"background",backScale,backScaleError);
    double tau = 0;
    unfold.DoUnfold(tau,hReco);
    TH1*hUnfolded = unfold.GetOutput("hUnfolded");
    TH2*hEmatStat=unfold.GetEmatrixInput("hEmatrixInput");
    TH2*hEmatTotal=unfold.GetEmatrixTotal("hEmatrixTotal");
    TH1F*hUnfoldedE = (TH1F*)hUnfolded->Clone("hUnfoldedTUnfold");
    int nMassBins = hTrue->GetNbinsX();
    for(int i=1;i<=nMassBins;i++){
        double binError = TMath::Sqrt(hEmatTotal->GetBinContent(i+1,i+1));
        hUnfoldedE->SetBinError(i+1,binError);
    }

    hUnfolded = hUnfoldedE;
    unfold.GetEmatrix(hEmatStat);

    TCanvas*c1 = new TCanvas("c1","",0,0,1000,1000);
    c1->SetGrid();
    c1->SetLogy();
    hTrue->SetMinimum(1);
    hTrue->SetFillColor(kRed+2);
    hUnfolded->SetMarkerStyle(20);
    hUnfolded->SetMarkerColor(kBlack);
    hTrue->Draw("hist");
    hUnfolded->Draw("pe,same");
    c1->SaveAs("unfold_test_closure_2D.png");
    outputFile->cd();
    hUnfolded->Write();
    outputFile->Close();
}
