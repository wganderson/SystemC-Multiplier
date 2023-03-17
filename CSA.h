
//simple CSA example using systemC

#ifndef CSA_H    
#define CSA_H

#include <systemc.h> 
                                
SC_MODULE(CSA) {
	sc_in<bool> clk;
	sc_in<bool> A, B, Ci, Si;
	sc_out<bool> Co, So;

	void CSA_method();
 
	SC_CTOR(CSA) {
		SC_METHOD(CSA_method);
		dont_initialize();
		sensitive << clk.pos();
	}
};
#endif 
