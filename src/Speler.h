#ifndef SPELER_H
#define SPELER_H

#include <string>

class Speler
{
  public:
  Speler();
  Speler(std::string spelersnaam) : naam(spelersnaam) {}
  virtual ~Speler();
  
  unsigned int id;
  std::string naam;
};

#endif // SPELER_H

