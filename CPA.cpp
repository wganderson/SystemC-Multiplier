#include "CPA.h"

void CPA :: CPA_method(){
	bool Aval = A.read();
	bool Bval = B.read();
	bool Cval = Ci.read();

	S.write((Aval != Bval) != Cval);
	Co.write((Aval && Bval) || (Cval && Aval) || (Cval && Bval));
}
