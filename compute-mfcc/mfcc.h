#ifndef MFCC_H
#define MFCC_H

// -----------------------------------------------------------------------------
//  A simple MFCC extractor using C++ STL and C++11
// -----------------------------------------------------------------------------
//
//  Copyright (C) 2016 D S Pavan Kumar
//  dspavankumar [at] gmail [dot] com
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include<complex>
#include<vector>
#include<map>

typedef std::vector<double> v_d;
typedef std::complex<double> c_d;
typedef std::vector<v_d> m_d;
typedef std::vector<c_d> v_c_d;
typedef std::map<int,std::map<int,c_d> > twmap;

class MFCC {

private:
    const double PI = 4*atan(1.0);   // Pi = 3.14...
    int fs;
    twmap twiddle;
    size_t winLengthSamples, frameShiftSamples, numCepstra, numFFT, numFFTBins, numFilters;
    double preEmphCoef, lowFreq, highFreq;
    v_d frame, powerSpectralCoef, lmfbCoef, hamming, mfcc, prevsamples;
    m_d fbank, dct;

private:
    // Hertz to Mel conversion
    inline double hz2mel (double f) {
        return 2595*std::log10 (1+f/700);
    }

    // Mel to Hertz conversion
    inline double mel2hz (double m) {
        return 700*(std::pow(10,m/2595)-1);
    }

    // Twiddle factor computation
    void compTwiddle(void);

    // Cooley-Tukey DIT-FFT recursive function
    v_c_d fft(v_c_d x);

    //// Frame processing routines
    // Pre-emphasis and Hamming window
    void preEmphHam(void);

    // Power spectrum computation
    void computePowerSpec(void);

    // Applying log Mel filterbank (LMFB)
    void applyLMFB(void);

    // Computing discrete cosine transform
    void applyDct(void);

    // Initialisation routines
    // Pre-computing Hamming window and dct matrix
    void initHamDct(void);

    // Precompute filterbank
    void initFilterbank ();

    // Convert vector of double to string (for writing MFCC file output)
    std::string v_d_to_string (v_d vec);

public:
    // MFCC class constructor
    MFCC(int sampFreq=16000, int nCep=12, int winLength=25, int frameShift=10, int numFilt=40, double lf=50, double hf=6500);

    // Process each frame and extract MFCC
    std::string processFrame(int16_t* samples, size_t N);
    // svlpにより追加;
    std::vector<double> processFrameWithoutShift(std::vector<int16_t> &samples, size_t from);

    // Read input file stream, extract MFCCs and write to output file stream
    int process (std::ifstream &wavFp, std::ofstream &mfcFp);
};

#endif // MFCC_H
