
//simple CPA example using systemC

#ifndef CPA_H    
#define CPA_H

#include <systemc.h> 
                                
SC_MODULE(CPA) {
	sc_in<bool> clk;
	sc_in<bool> A, B, Ci;
	sc_out<bool> Co, S;

	void CPA_method();
 
	SC_CTOR(CPA) {
		SC_METHOD(CPA_method);
		dont_initialize();
		sensitive << clk.neg() << Ci;
	}
};
#endif 
