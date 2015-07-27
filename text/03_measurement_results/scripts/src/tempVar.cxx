#include <string.h>
#include <sstream>
#include "drawfuns.h"
using namespace std;

// ------------------------------ MAIN PROGRAM ----------------------
int32_t main (void) {

  DrawFuns* dr = new DrawFuns();
  

  delete(dr);
  cout<<"All done."<<endl;
  return 0;
}//-------------------- end of main -----------------------------------------
