
void defBinningScheme()
{
    TFile*binningSchemes = new TFile("testDYBinning.root","recreate");
    const int nBinsMass = 37;
    const int nBinsRecoEra = 3;
    const int nBinsTrueEra = 1;

    double binsMass[nBinsMass+1] = {50,55,60,64,68,72,76,81,86,91,96,101,106,110,115,120,126,133,141,150,160,171,185,200,220,243,273,320,380,440,510,600,700,830,1000,1500,2000,3000};
    double binsRecoEra[nBinsRecoEra+1] = {0,1,2,3};
    double binsTrueEra[nBinsTrueEra+1] = {0,1};

    TUnfoldBinning*recoBinning = new TUnfoldBinning("recoBinning");
    TUnfoldBinning*recoDist = recoBinning->AddBinning("recoDist");    
    recoDist->AddAxis("mass",nBinsMass,binsMass,true,true);
    recoDist->AddAxis("era",nBinsRecoEra,binsRecoEra,false,false);

    recoBinning->PrintStream(cout);

    TUnfoldBinning*trueBinning = new TUnfoldBinning("trueBinning");
    TUnfoldBinning*trueDist = trueBinning->AddBinning("trueDist");    
    trueDist->AddAxis("mass",nBinsMass,binsMass,true,true);
    trueDist->AddAxis("era",nBinsTrueEra,binsTrueEra,false,false);

    trueBinning->PrintStream(cout);

    recoBinning->Write();
    trueBinning->Write();
}
