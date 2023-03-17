#include "CSA.h"

void CSA :: CSA_method(){
	bool ABval = A.read() && B.read();
	bool Sval = Si.read();
	bool Cval = Ci.read();

	So.write((ABval != Sval) != Cval);
	Co.write((ABval && Sval) || (Cval && ABval) || (Cval && Sval));
}
