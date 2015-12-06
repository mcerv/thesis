#include "progbar.h"

ProgBar::ProgBar ()
{
}

ProgBar::ProgBar (string message)
{
  cout<<" "<<message<<endl;
}

void ProgBar::show(int32_t nevent, int32_t numEvents)
{

  int32_t progress = nevent + 1;
  //if (progress % 500 && nevent != endEvent) return;

  int32_t progressProc = (progress * 100) / numEvents; // Now as an integer %

  cout << "\r Progress: [";
  for (unsigned int i = 1; i <= 50; i++)
  {

    if (progressProc >= i * 2) cout << "=";
    else cout << " ";
  }
  //cout << "] " << std::setw(4) << progress << "%" << flush;
  cout << "] " << std::setw(5) << progress << "/" << numEvents << flush;
  if (progress == numEvents) cout << endl;
}
