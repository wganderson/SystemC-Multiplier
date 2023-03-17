// Multiplier
//////////////////////////////////
//Testbench driver 

#include <systemc.h>
#include "CPA.h"
#include "CSA.h"
#include <iostream>
#include <string>
#include <sstream>

void make_MULT();

int sc_main(int argc, char* argv[]){
	make_MULT();
	return 0;                         
}

void make_MULT(){
	sc_signal<bool> ground;	

	sc_signal<bool> At[8], Bt[8], Cit[8], Cot[8];

	sc_vector<CPA> CPAlist;
	CPAlist.init(8);
	
	sc_signal<bool> Cost[8][8], Sost[8][8]; 
	
	sc_uint<8> Ain, Bin;
	sc_signal<sc_uint<16> > result;
	sc_signal<bool> resbool[16];
	sc_signal<bool> overflow;
	
	sc_clock clk0("clk0", 20, SC_NS, 0.5, 0, SC_NS);   //Create a clock signal
	sc_clock clk1("clk1", 20, SC_NS, 0.5, 1, SC_NS);
	sc_clock clk2("clk2", 20, SC_NS, 0.5, 2, SC_NS);
	sc_clock clk3("clk3", 20, SC_NS, 0.5, 3, SC_NS);
	sc_clock clk4("clk4", 20, SC_NS, 0.5, 4, SC_NS);
	sc_clock clk5("clk5", 20, SC_NS, 0.5, 5, SC_NS);
	sc_clock clk6("clk6", 20, SC_NS, 0.5, 6, SC_NS);
	sc_clock clk7("clk7", 20, SC_NS, 0.5, 7, SC_NS);

	sc_vector<sc_vector<CSA> > CSAlist;
	CSAlist.init(8);
	for(int i = 0; i < 8; i++){
		CSAlist[i].init(8);
	}

	for(int i = 0; i < 8; i++){//bind signals for CPA components
		CPAlist[i].S(resbool[8 + i]); 
		if(i == 0){	
			CPAlist[i].B(Cost[7][0]);
			CPAlist[i].A(Sost[7][1]);
			CPAlist[i].Ci(ground);
			CPAlist[i].Co(Cot[i]);
		}
		else if(i == 7){
			CPAlist[i].B(ground);
			CPAlist[i].A(Cost[7][7]);
			CPAlist[i].Ci(Cot[i - 1]);
			CPAlist[i].Co(overflow);
		}
		else{
			CPAlist[i].B(Cost[7][i]);
			CPAlist[i].A(Sost[7][i + 1]);
			CPAlist[i].Ci(Cot[i - 1]);
			CPAlist[i].Co(Cot[i]);
		}
		switch(i){//assign a clock for each final calculation
			case 0:
				CPAlist[i].clk(clk0);			
				break;
			case 1:
				CPAlist[i].clk(clk1);
				break;
			case 2:
				CPAlist[i].clk(clk2);
				break;
			case 3:
				CPAlist[i].clk(clk3);
				break;
			case 4:
				CPAlist[i].clk(clk4);
				break;
			case 5:
				CPAlist[i].clk(clk5);
				break;
			case 6:
				CPAlist[i].clk(clk6);
				break;
			case 7:
				CPAlist[i].clk(clk7);
				break;			
			}
	}
	int j = 0;

	for(int i = 0; i < 8; i++){//bind signals for CSA components
		for(int j = 0; j < 8; j++){
			CSAlist[i][j].A(At[j]);
			CSAlist[i][j].B(Bt[i]);
			CSAlist[i][j].Co(Cost[i][j]);
			if(i == 0){//first row
				CSAlist[i][j].Ci(ground);
				CSAlist[i][j].Si(ground);
				if(j == 0){//first column
					CSAlist[i][j].So(resbool[i]);
				}
				else{
					CSAlist[i][j].So(Sost[i][j]);
				}
			}
			else if(j == 7){//last column
				CSAlist[i][j].Ci(Cost[i - 1][j]);
				CSAlist[i][j].Si(ground);
				CSAlist[i][j].So(Sost[i][j]);
			}
			else if(j == 0){//first column
				CSAlist[i][j].Ci(Cost[i - 1][j]);
				CSAlist[i][j].Si(Sost[i - 1][j + 1]);
				CSAlist[i][j].So(resbool[i]);
			}
			else{//everything else
				CSAlist[i][j].Ci(Cost[i - 1][j]);
				CSAlist[i][j].Si(Sost[i - 1][j + 1]);
				CSAlist[i][j].So(Sost[i][j]);
			}
			switch(i){//assign a clock for each row for sequential operation
				case 0:
					CSAlist[i][j].clk(clk0);			
					break;
				case 1:
					CSAlist[i][j].clk(clk1);
					break;
				case 2:
					CSAlist[i][j].clk(clk2);
					break;
				case 3:
					CSAlist[i][j].clk(clk3);
					break;
				case 4:
					CSAlist[i][j].clk(clk4);
					break;
				case 5:
					CSAlist[i][j].clk(clk5);
					break;
				case 6:
					CSAlist[i][j].clk(clk6);
					break;
				case 7:
					CSAlist[i][j].clk(clk7);
					break;			
			}
		}
	}

	sc_trace_file *tf;
	tf = sc_create_vcd_trace_file("MULT_trace_file");	
	tf->set_time_unit(1, SC_NS);
	
	//add trace signals
	for(int i = 0; i < 16; i++){
		sc_trace(tf, resbool[i], "Result");
	}
	sc_trace(tf, Ain, "A");
	sc_trace(tf, Bin, "B");
	//sc_trace(tf, overflow, "overflow");
	//initialize	
	ground.write(0);

	//cout << "0 * 0" << endl;
	Ain = 0;
	Bin = 0;
	
	for(int i = 0; i < 8; i++){
		At[i].write(Ain[i]);
		Bt[i].write(Bin[i]);
	}
	
	sc_start(20, SC_NS);

	//multiply all numbers
	for(int j = 0; j < 256; j++){
		for(int k = 0; k < 256; k++){
			Ain = j;
			Bin = k;
			for(int i = 0; i < 8; i++){
				At[i].write(Ain[i]);
				Bt[i].write(Bin[i]);
			}
			sc_start(20, SC_NS);
		}
	}
	
	Ain = 0;
	Bin = 0;
	
	for(int i = 0; i < 8; i++){
		At[i].write(Ain[i]);
		Bt[i].write(Bin[i]);
	}
	
	sc_start(20, SC_NS);
}
