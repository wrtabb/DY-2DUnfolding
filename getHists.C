TString loc_2016preAPV = "histograms_2016preAPV.root";
TString loc_2016postAPV = "histograms_2016postAPV.root";
TString loc_2018 = "histograms_2018.root";

void getHists()
{
    TH1::SetDefaultSumw2();
    TH2::SetDefaultSumw2();

    gStyle->SetOptStat(0);
    gStyle->SetPalette(1);
    TFile*outputFile = new TFile("unfold_histograms.root","recreate");
    TFile*binningSchemes = new TFile("testDYBinning.root");
    TUnfoldBinning*recoBinning,*trueBinning; 
    outputFile->cd();
    binningSchemes->GetObject("recoBinning",recoBinning);
    binningSchemes->GetObject("trueBinning",trueBinning);
    delete binningSchemes;
    recoBinning->Write();
    trueBinning->Write();

    if(recoBinning) recoBinning->PrintStream(cout);
    else cout << "could not read 'reco' binning" << endl;
    if(trueBinning) trueBinning->PrintStream(cout);
    else cout << "could not read 'true' binning" << endl;

    const TUnfoldBinning*recoDist = recoBinning->FindNode("recoDist");
    const TUnfoldBinning*trueDist = trueBinning->FindNode("trueDist");
    
    TString data_name = "data";
    TString reco_name = "reco_mass";
    TString true_name = "gen_mass";
    TString back_name = "backgrounds";
    TString matrix_name = "matrix";

    TFile*load2016preAPV = new TFile(loc_2016preAPV);
    TH1D*hDataSignal_2016preAPV = (TH1D*)load2016preAPV->Get(data_name);
    TH1D*hMCRecoSignal_2016preAPV = (TH1D*)load2016preAPV->Get(reco_name);
    TH1D*hMCRecoBack_2016preAPV = (TH1D*)load2016preAPV->Get(back_name);
    TH1D*hMCTruth_2016preAPV = (TH1D*)load2016preAPV->Get(true_name);
    TH2D*hMatrix_2016preAPV = (TH2D*)load2016preAPV->Get(matrix_name);
    hDataSignal_2016preAPV->SetName("data_2016preAPV");
    hMCRecoSignal_2016preAPV->SetName("reco_2016preAPV");
    hMCRecoBack_2016preAPV->SetName("back_2016preAPV");
    hMCTruth_2016preAPV->SetName("true_2016preAPV");
    hMatrix_2016preAPV->SetName("matrix_2016preAPV");

    TFile*load2016postAPV = new TFile(loc_2016postAPV);
    TH1D*hDataSignal_2016postAPV = (TH1D*)load2016postAPV->Get(data_name);
    TH1D*hMCRecoSignal_2016postAPV = (TH1D*)load2016postAPV->Get(reco_name);
    TH1D*hMCRecoBack_2016postAPV = (TH1D*)load2016postAPV->Get(back_name);
    TH1D*hMCTruth_2016postAPV = (TH1D*)load2016postAPV->Get(true_name);
    TH2D*hMatrix_2016postAPV = (TH2D*)load2016postAPV->Get(matrix_name);
    hDataSignal_2016postAPV->SetName("data_2016postAPV");
    hMCRecoSignal_2016postAPV->SetName("reco_2016postAPV");
    hMCRecoBack_2016postAPV->SetName("back_2016postAPV");
    hMCTruth_2016postAPV->SetName("true_2016postAPV");
    hMatrix_2016postAPV->SetName("matrix_2016postAPV");

    TFile*load2018 = new TFile(loc_2018);
    TH1D*hDataSignal_2018 = (TH1D*)load2018->Get(data_name);
    TH1D*hMCRecoSignal_2018 = (TH1D*)load2018->Get(reco_name);
    TH1D*hMCRecoBack_2018 = (TH1D*)load2018->Get(back_name);
    TH1D*hMCTruth_2018 = (TH1D*)load2018->Get(true_name);
    TH2D*hMatrix_2018 = (TH2D*)load2018->Get(matrix_name);
    hDataSignal_2018->SetName("data_2018");
    hMCRecoSignal_2018->SetName("reco_2018");
    hMCRecoBack_2018->SetName("back_2018");
    hMCTruth_2018->SetName("true_2018");
    hMatrix_2018->SetName("matrix_2018");
    
    TH1*histData = recoBinning->CreateHistogram("data");
    TH1*histMCReco = recoBinning->CreateHistogram("reco");
    TH1*histMCBack = recoBinning->CreateHistogram("back");
    TH1*histMCTrue = trueBinning->CreateHistogram("true");
    TH1*histMCTrue16 = trueBinning->CreateHistogram("true16");
    TH1*histMCTrue18 = trueBinning->CreateHistogram("true18");
    TH2*histMatrix=TUnfoldBinning::CreateHistogramOfMigrations(recoBinning,trueBinning,"hMatrix");

    int nBinsMass = hMCRecoSignal_2016preAPV->GetNbinsX();
    int nBinsRecoEra = 3;
    int nBinsTrueEra = 1;
    double recoMassContent,trueMassContent,backMassContent,dataMassContent,matrixMassContent;
    double recoMassError,trueMassError,backMassError,dataMassError,matrixMassError;
    double eraBin,massBin;
    double eraBinTrue = 0;
    int iTrue,iRec;
    for(int i=0;i<=nBinsMass+1;i++){
        massBin = hMCTruth_2016preAPV->GetBinCenter(i);
        iTrue = trueDist->GetGlobalBinNumber(massBin,eraBinTrue);
        for(int j=1;j<=nBinsRecoEra;j++){
            if(j==1){ 
                trueMassContent = hMCTruth_2016preAPV->GetBinContent(i);
                trueMassError = hMCTruth_2016preAPV->GetBinError(i);
                recoMassContent = hMCRecoSignal_2016preAPV->GetBinContent(i);
                recoMassError = hMCRecoSignal_2016preAPV->GetBinError(i);
                backMassContent = hMCRecoBack_2016preAPV->GetBinContent(i);
                backMassError = hMCRecoBack_2016preAPV->GetBinError(i);
                dataMassContent = hDataSignal_2016preAPV->GetBinContent(i);
                dataMassError = hDataSignal_2016preAPV->GetBinError(i);

                histMCTrue->SetBinContent(iTrue,trueMassContent);
                histMCTrue->SetBinError(iTrue,trueMassError);
            }
            else if(j==2){
                trueMassContent = hMCTruth_2016postAPV->GetBinContent(i);
                trueMassError = hMCTruth_2016postAPV->GetBinError(i);
                recoMassContent = hMCRecoSignal_2016postAPV->GetBinContent(i);
                recoMassError = hMCRecoSignal_2016postAPV->GetBinError(i);
                backMassContent = hMCRecoBack_2016postAPV->GetBinContent(i);
                backMassError = hMCRecoBack_2016postAPV->GetBinError(i);
                dataMassContent = hDataSignal_2016postAPV->GetBinContent(i);
                dataMassError = hDataSignal_2016postAPV->GetBinError(i);

                histMCTrue16->SetBinContent(iTrue,trueMassContent);
                histMCTrue16->SetBinError(iTrue,trueMassError);
            }
            else if(j==3){
                trueMassContent = hMCTruth_2018->GetBinContent(i);
                trueMassError = hMCTruth_2018->GetBinError(i);
                recoMassContent = hMCRecoSignal_2018->GetBinContent(i);
                recoMassError = hMCRecoSignal_2018->GetBinError(i);
                backMassContent = hMCRecoBack_2018->GetBinContent(i);
                backMassError = hMCRecoBack_2018->GetBinError(i);
                dataMassContent = hDataSignal_2018->GetBinContent(i);
                dataMassError = hDataSignal_2018->GetBinError(i);

                histMCTrue18->SetBinContent(iTrue,trueMassContent);
                histMCTrue18->SetBinError(iTrue,trueMassError);
            }
            eraBin = j-1;
            iRec = recoDist->GetGlobalBinNumber(massBin,eraBin);
            histMCReco->SetBinContent(iRec,recoMassContent);
            histMCReco->SetBinError(iRec,recoMassError);
            histMCBack->SetBinContent(iRec,backMassContent);
            histMCBack->SetBinError(iRec,backMassError);
            histData->SetBinContent(iRec,dataMassContent);
            histData->SetBinError(iRec,dataMassError);
        }
    }
    histMCTrue->Add(histMCTrue16);
    histMCTrue->Add(histMCTrue18);

    double massBinTrue,massBinReco;
    for(int i=0;i<=nBinsMass+1;i++){
        massBinTrue = hMCTruth_2016preAPV->GetBinCenter(i);
        iTrue = trueDist->GetGlobalBinNumber(massBinTrue,eraBinTrue);
        for(int j=0;j<=nBinsMass+1;j++){
            massBinReco = hMCRecoSignal_2016preAPV->GetBinCenter(j);
            for(int k=1;k<=3;k++){
                if(k==1){
                    matrixMassContent = hMatrix_2016preAPV->GetBinContent(j,i);
                    matrixMassError = hMatrix_2016preAPV->GetBinError(j,i);
                }
                else if(k==2){
                    matrixMassContent = hMatrix_2016postAPV->GetBinContent(j,i);
                    matrixMassError = hMatrix_2016postAPV->GetBinError(j,i);
                }
                else if(k==3){
                    matrixMassContent = hMatrix_2018->GetBinContent(j,i);
                    matrixMassError = hMatrix_2018->GetBinError(j,i);
                }
                eraBin = k-1;
                iRec = recoDist->GetGlobalBinNumber(massBinReco,eraBin);
                histMatrix->SetBinContent(iRec,iTrue,matrixMassContent);
                histMatrix->SetBinError(iRec,iTrue,matrixMassError);
            }
        }
    }

    
    TH1D*hMC = (TH1D*)histMCReco->Clone("hMC");
    hMC->Add(histMCBack);
    double ratioRange = 0.2;
    double upperBound = 1.0+ratioRange;
    double lowerBound = 1.0-ratioRange;

    TH1D*hRatio = (TH1D*)hMC->Clone("ratio");
    hRatio->Divide(histData);
    hRatio->SetMarkerStyle(20);
    hRatio->SetMarkerColor(kBlack);
    hRatio->SetMinimum(lowerBound);
    hRatio->SetMaximum(upperBound);

    TCanvas*c1=new TCanvas("c1","",0,0,1400,1000);
    c1->SetGrid();
    c1->SetLogy();
    histMCBack->SetFillColor(kRed+2);
    histMCReco->SetFillColor(kOrange-1);
    histData->SetMarkerStyle(20);
    histData->SetMarkerColor(kBlack);
    THStack*hStack = new THStack("stack_plot","");
    hStack->Add(histMCBack);
    hStack->Add(histMCReco);

    TLegend*legend = new TLegend(0.78,0.74,0.9,0.90);
    legend->SetTextSize(0.02);
    legend->AddEntry(histData,"Data");
    legend->AddEntry(histMCReco,"DY#rightarrow#mu#mu");
    legend->AddEntry(histMCBack,"backgrounds");

    const float padmargins = 0.03;
    const float yAxisMinimum = 1;
    const float yAxisMaximum = 1e7;
    double ratioSplit = 0.20;
    TPad*pad1 = new TPad("","",0,ratioSplit,1.0,1.0);
    pad1->SetLogy();
    pad1->SetBottomMargin(padmargins);
    pad1->SetGrid();
    pad1->SetTicks(1,1);
    pad1->Draw();
    pad1->cd();

    hStack->Draw("hist");
    hStack->GetXaxis()->SetLabelSize(0);
    hStack->GetXaxis()->SetTitleSize(0);
    hStack->SetMinimum(1);
    histData->Draw("pe,same");
    legend->Draw("same");

    int nBins = histMCReco->GetNbinsX();
    double x1 = histMCReco->GetBinLowEdge(1);
    double x2 = histMCReco->GetBinLowEdge(nBins);
    x2 += histMCReco->GetBinWidth(nBins);

    TLine*line = new TLine(x1,1,x2,1);
    line->SetLineColor(kRed);

    c1->cd();
    TPad*pad2 = new TPad("","",0,0.05,1,ratioSplit);
    pad2->SetTopMargin(padmargins);
    pad2->SetBottomMargin(0.2);
    pad2->SetGrid();
    pad2->SetTicks(1,1);
    pad2->Draw();
    pad2->cd();
    hRatio->SetTitle("");
    hRatio->GetYaxis()->SetLabelSize(0.06);
    hRatio->GetYaxis()->SetTitleSize(0.09);
    hRatio->GetYaxis()->SetTitleOffset(0.3);
    hRatio->GetYaxis()->SetTitle("MC/data");
    hRatio->GetXaxis()->SetLabelSize(0.1);
    hRatio->GetXaxis()->SetTitleSize(0.11);
    hRatio->GetXaxis()->SetTitleOffset(0.8);
    hRatio->GetXaxis()->SetTitle("recoBinning");
    hRatio->GetXaxis()->SetNoExponent();
    hRatio->GetXaxis()->SetMoreLogLabels();
    hRatio->Draw("pe");
    line->Draw("same");

    c1->SaveAs("reco_test_2D.png");   

    TCanvas*c2=new TCanvas("c2","",0,0,1000,1000);
    c2->SetGrid();
    c2->SetLogy();
    histMCTrue->SetMarkerColor(kRed);
    histMCTrue->SetMarkerColor(kBlue);
    histMCTrue->SetMarkerStyle(20);
    histMCTrue->SetMinimum(1);
    histMCTrue->Draw("pe,same");
    c2->SaveAs("true_test_2D.png");   

    TCanvas*c3=new TCanvas("c3","",0,0,1400,1000);
    c3->SetGrid();
    c3->SetLogz();
    histMatrix->Draw("colz");
    c3->SaveAs("matrix_test_2D.png");   

    TCanvas*c4 = new TCanvas("c4","",0,0,1400,1000);
    c4->SetGrid();
    c4->SetLogy();
    TH1D*projX = (TH1D*)histMatrix->ProjectionX("projX");
    TH1D*hReco = (TH1D*)histMCReco->Clone("hReco");
    hReco->SetFillColor(kWhite);
    projX->SetMarkerStyle(20);
    projX->SetMarkerColor(kBlue);
    hReco->SetLineColor(kBlue);
    hReco->Draw("hist,same");
    projX->Draw("pe,same");
    c4->SaveAs("reco_projection_test_2D.png");

    TCanvas*c5 = new TCanvas("c5","",0,0,1000,1000);
    c5->SetGrid();
    c5->SetLogy();
    TH1D*projY = (TH1D*)histMatrix->ProjectionY("projY");
    TH1D*hTrue = (TH1D*)histMCTrue->Clone("hTrue");
    projY->SetMarkerStyle(20);
    projY->SetMarkerColor(kRed);
    hTrue->SetLineColor(kRed);
    hTrue->Draw("hist,same");
    projY->Draw("pe,same"); 
    c5->SaveAs("true_projection_test_2D.png");

    outputFile->cd();
    histMCReco->Write();
    histMCTrue->Write();
    histData->Write();
    histMatrix->Write();
    histMCBack->Write();
    outputFile->Close();
}
