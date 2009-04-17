#include <iostream>

#include "game.hpp"

int main( int argc, char **argv )
{
  CGame game( argc, argv );

  if ( game.Initialize() )
    game.Start();
  else
    return 1;

  return 0;
}
