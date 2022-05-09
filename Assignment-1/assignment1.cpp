// Author - Kartikeya Gupta
// Roll No - 190420
// CC Username - gkartik

#include <iostream>
#include <bits/stdc++.h>
#include "AWGN.h"
using namespace std;

vector< pair<int, int>> satellite_prn_Ref = {
 make_pair(2,6),
 make_pair(3,7),
 make_pair(4,8),
 make_pair(5,9),
 make_pair(1,9),
 make_pair(2,10),
 make_pair(1,8),
 make_pair(2,9),
 make_pair(3,10),
 make_pair(2,3),
 make_pair(3,4),
 make_pair(5,6),
 make_pair(6,7),
 make_pair(7,8),
 make_pair(8,9),
 make_pair(9,10),
 make_pair(1,4),
 make_pair(2,5),
 make_pair(3,6),
 make_pair(4,7),
 make_pair(5,8),
 make_pair(6,9),
 make_pair(1,3),
 make_pair(4,6),
 make_pair(5,7),
 make_pair(6,8),
 make_pair(7,9),
 make_pair(8,10),
 make_pair(1,6),
 make_pair(2,7),
 make_pair(3,8),
 make_pair(4,9)
};


int generatePRNChip(vector<int>& shift_register_1,vector<int>& shift_register_2, int prn_no){

    // first, storing the output for P1, which later becomes 
    // the first index of the shift register of P1 
    int a1 = (shift_register_1[2])^(shift_register_1[9]);
    int a10 = shift_register_1[9];

    // shifting the register for P1
    shift_register_1.insert(shift_register_1.begin(),a1);
    shift_register_1.erase(shift_register_1.begin() + 10);

    // now, storing the output of P2 and shifting the 
    // shift register for P2
    int a2 = shift_register_2[1]^shift_register_2[2]^shift_register_2[5]^shift_register_2[7]^shift_register_2[8]^shift_register_2[9];
    shift_register_1.insert(shift_register_2.begin(),a2);
    shift_register_1.erase(shift_register_2.begin() + 10);

     // taking xor of eliminated index of P1 with satellite
    // specific space vehicle(SV) numbers
    return a10^(shift_register_2[satellite_prn_Ref[prn_no - 1].first]^shift_register_2[satellite_prn_Ref[prn_no - 1].second]);
}



// Directly call the solve() function for generating the
// PRN chip for any provided satellite and sample frequency
void calculatePRN(int pr_n, int s_freq, vector<int>& prn_chip){
    freopen( "q1.txt", "w", stdout );
    // Taking input for satellite PRN and 
    // sampling frequncy
     if(pr_n==-1){
        printf("Enter satellite PRN number: ");
        scanf("%d", &pr_n);  
     }
     if(s_freq==-1){
        printf("Enter sampling frequency: ");
        scanf("%d", &s_freq);
     } 


    // Declaring and intializing the shift registers for
    // the two polynomials P1 and P2
    vector<int> shift_register_1(10), shift_register_2(10);
    for(int i=0;i<10;i++){
        shift_register_1[i] = 1;
        shift_register_2[i] = 1;
    }

    for(int i=0;i<1023;i++){
        prn_chip[i] = generatePRNChip(shift_register_1,shift_register_2,pr_n);
        cout<<prn_chip[i];
    }
    return;
}




// This function generates the circular autocorrelation matrix with
// an additional additive white gaussian noise(AWGN).
// The required parameters are the satellite PRN number and the 
// delay given in the replica PNR code.
// Programmed for Q2
void autoCorrelation(int prn_n, int delay){
    // generating the PNR code and then
    // converting prn code for correlation calculations
    // if pnr_chip = 0 then set it to -1

    // Intializing the PRN code
    vector<int> prn_chip(1023);
    calculatePRN(prn_n, 200,prn_chip);
    freopen( "q4.txt", "w", stdout );
    for(int i=0;i<1023;i++){
        if(prn_chip[i]==0){
            prn_chip[i] = -1;
        }
    }

    int correlation_value[1023];
    for(int i=0;i<1023;i++){
        int prn_index = delay;
        int n = i;
        int sum = 0;
        for(int j=0;j<1023;j++){
            sum+=prn_chip[prn_index]*prn_chip[n];
            prn_index = (prn_index+1)%1023;
            n = (n+1)%1023;
        }
        correlation_value[i] = sum;
        cout<<correlation_value[i]<<endl;
    } 

    return;

}

// This function generates the cross-correlation matrix
// The required parameters are the satellite PRN numbers and the 
// delays given in the PNR codes.
// Programmed for Q3
void crossCorrelation(int prn_n1,int prn_n2,int delay1,int delay2){
    // generating the PNR code and then
    // converting prn code for correlation calculations
    // if pnr_chip = 0 then set it to -1
    vector<int> prn_chip1(1023);
    calculatePRN(prn_n1, 200, prn_chip1);

    for(int i=0;i<1023;i++){
        if(prn_chip1[i]==0){
            prn_chip1[i] = -1;
        }
    }

    vector<int> prn_chip2(1023);
    calculatePRN(prn_n2, 200, prn_chip2);

    // The below few loops have been implemented to 
    // account for a heap corruption due to consecutive calling of
    // the calculatePRN() function
    int min = INT_MAX;
    for(int i=0;i<1023;i++){
        if(prn_chip2[i]<min){
            min = prn_chip2[i];
        }
    }
    for(int i=0;i<1023;i++){
        prn_chip2[i] = prn_chip2[i] - min;
        //cout<<prn_chip2[i];
        if(prn_chip2[i]==0){
            prn_chip2[i] = -1;
        }
    }
    
    freopen( "q3.txt", "w", stdout );
    int correlation_value[1023];
    for(int i=0;i<1023;i++){
        int prn_index2 = delay2;
        int n = i + delay1;
        int sum = 0;
        for(int j=0;j<1023;j++){
            sum+= (prn_chip2[prn_index2]*prn_chip1[n]);
            prn_index2 = (prn_index2 + 1)%1023;
            n = (n+1)%1023;
        }
        correlation_value[i] = sum*(-1);
        cout<<correlation_value[i]<<endl;
    } 

    return;

}



void noisyAutoCorrelation(int prn_n, int delay){
    // generating the PNR code and then
    // converting prn code for correlation calculations
    // if pnr_chip = 0 then set it to -1

    // Intializing the PRN code
    vector<int> prn_chip(1023);
    calculatePRN(prn_n, 200,prn_chip);
    freopen( "q4.txt", "w", stdout );
    for(int i=0;i<1023;i++){
        if(prn_chip[i]==0){
            prn_chip[i] = -1;
        }
    }

    long samples = 1023;
    AWGN g(0.0,4.0,samples);
    double * awgn = g.generateNoiseSamples();

    int correlation_value[1023];
    for(int i=0;i<1023;i++){
        int prn_index = delay;
        int n = i;
        int sum = 0;
        for(int j=0;j<1023;j++){
            sum+=prn_chip[prn_index]*prn_chip[n];
            prn_index = (prn_index+1)%1023;
            n = (n+1)%1023;
        }
        awgn[i] += sum;
        cout<<awgn[i]<<endl;
    } 

    return;

}

int main()
{    
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    // Run this for Q1
    vector<int> prn_chip(1023);
    calculatePRN(5,200,prn_chip);

    // Run this for Q2
    // autoCorrelation(8,200);

    // Run this for Q3
    // crossCorrelation(8,16,0,200);

    // Run this for Q4
    noisyAutoCorrelation(8,200);
	//cerr << "Time : " << 1000 * ((double)clock()) / (double)CLOCKS_PER_SEC << "ms\n";
	return 0;
}  
