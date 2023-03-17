# SystemC-Multiplier
## CPA ##
A CPA is simply a full adder, each input and output was assigned to a sc_int and sc_out port accordingly. As for the functionality, if one analyzes the many block diagrams that can be found on the internet, one would find that each output; S and C can be boiled down to a logical expression. This is convenient because C++ allows us to write expressions representing this digital logic. The sum and carry-out logic expression can be written in C++ as follows:
![image](https://user-images.githubusercontent.com/114177995/226051008-ef1f36b1-3f7c-4892-9a91-9db2b993581e.png)

## CSA ##
Using the definition of a CSA seen below, only a few modifications to the CPA code were made to implement this functionality. Additional input was added and the A and B signals were anded together. The logical expression remains the same but with the sum and A&B signals replacing Bval and Aval accordingly:
![image](https://user-images.githubusercontent.com/114177995/226051247-06f6fa86-b186-4b66-a73b-f92d296bdba6.png)

## Definition of CPA/CSA: ##

![image](https://user-images.githubusercontent.com/114177995/226050422-949242a4-4fbf-402c-b0d8-eefec101cd3e.png)


## Connection ##

There were dozens of signals that needed to be binded to dozens of modules. This was done using arrays of signals and vectors of modules. Using a for loop and some logic to bind signals in a unique way depending on their position in the lattice, the binding was done. I used a separate loop for the CSA and the CPA. I did all of this connection inside of sc_main.cpp.


## Synchronization ##

each stage of the calculation, the order of the processes being run needs to be properly controlled. Each row of CSA blocks needs to complete before the next row starts calculating. Also, each CPA needs to run before the following block.

My solution consisted of 8 separate clock signals, each delayed 1ns from the previous one. Each row of CSAs was assigned its own clock which they were positive edge sensitive to. Each CPA was also assigned its own clock, but they were negative edge sensitive.
