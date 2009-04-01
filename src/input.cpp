/*
   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "settings.hpp"
#include "parser.hpp"
#include "game.hpp"
#include "input.hpp"


Input::Input( std::string file )
{
  availableKeys["A"] = sf::Key::A;
  availableKeys["B"] = sf::Key::B;
  availableKeys["C"] = sf::Key::C;
  availableKeys["D"] = sf::Key::D;
  availableKeys["E"] = sf::Key::E;
  availableKeys["F"] = sf::Key::F;
  availableKeys["G"] = sf::Key::G;
  availableKeys["H"] = sf::Key::H;
  availableKeys["I"] = sf::Key::I;
  availableKeys["J"] = sf::Key::J;
  availableKeys["K"] = sf::Key::K;
  availableKeys["L"] = sf::Key::L;
  availableKeys["M"] = sf::Key::M;
  availableKeys["N"] = sf::Key::N;
  availableKeys["O"] = sf::Key::O;
  availableKeys["P"] = sf::Key::P;
  availableKeys["Q"] = sf::Key::Q;
  availableKeys["R"] = sf::Key::R;
  availableKeys["S"] = sf::Key::S;
  availableKeys["T"] = sf::Key::T;
  availableKeys["U"] = sf::Key::U;
  availableKeys["V"] = sf::Key::V;
  availableKeys["W"] = sf::Key::W;
  availableKeys["X"] = sf::Key::X;
  availableKeys["Y"] = sf::Key::Y;
  availableKeys["Z"] = sf::Key::Z;
  availableKeys["Num0"] = sf::Key::Num0;
  availableKeys["Num1"] = sf::Key::Num1;
  availableKeys["Num2"] = sf::Key::Num2;
  availableKeys["Num3"] = sf::Key::Num3;
  availableKeys["Num4"] = sf::Key::Num4;
  availableKeys["Num5"] = sf::Key::Num5;
  availableKeys["Num6"] = sf::Key::Num6;
  availableKeys["Num7"] = sf::Key::Num7;
  availableKeys["Num8"] = sf::Key::Num8;
  availableKeys["Num9"] = sf::Key::Num9;
  availableKeys["Escape"] = sf::Key::Escape;
  availableKeys["LControl"] = sf::Key::LControl;
  availableKeys["LShift"] = sf::Key::LShift;
  availableKeys["LAlt"] = sf::Key::LAlt;
  availableKeys["LSystem"] = sf::Key::LSystem;
  availableKeys["RControl"] = sf::Key::RControl;
  availableKeys["RShift"] = sf::Key::RShift;
  availableKeys["RAlt"] = sf::Key::RAlt;
  availableKeys["RSystem"] = sf::Key::RSystem;
  availableKeys["Menu"] = sf::Key::Menu;
  availableKeys["LBracket"] = sf::Key::LBracket;
  availableKeys["RBracket"] = sf::Key::RBracket;
  availableKeys["SemiColon"] = sf::Key::SemiColon;
  availableKeys["Comma"] = sf::Key::Comma;
  availableKeys["Period"] = sf::Key::Period;
  availableKeys["Quote"] = sf::Key::Quote;
  availableKeys["Slash"] = sf::Key::Slash;
  availableKeys["BackSlash"] = sf::Key::BackSlash;
  availableKeys["Tilde"] = sf::Key::Tilde;
  availableKeys["Equal"] = sf::Key::Equal;
  availableKeys["Dash"] = sf::Key::Dash;
  availableKeys["Space"] = sf::Key::Space;
  availableKeys["Return"] = sf::Key::Return;
  availableKeys["Back"] = sf::Key::Back;
  availableKeys["Tab"] = sf::Key::Tab;
  availableKeys["PageUp"] = sf::Key::PageUp;
  availableKeys["PageDown"] = sf::Key::PageDown;
  availableKeys["End"] = sf::Key::End;
  availableKeys["Home"] = sf::Key::Home;
  availableKeys["Insert"] = sf::Key::Insert;
  availableKeys["Delete"] = sf::Key::Delete;
  availableKeys["Add"] = sf::Key::Add;
  availableKeys["Subtract"] = sf::Key::Subtract;
  availableKeys["Multiply"] = sf::Key::Multiply;
  availableKeys["Divide"] = sf::Key::Divide;
  availableKeys["Left"] = sf::Key::Left;
  availableKeys["Right"] = sf::Key::Right;
  availableKeys["Up"] = sf::Key::Up;
  availableKeys["Down"] = sf::Key::Down;
  availableKeys["Numpad0"] = sf::Key::Numpad0;
  availableKeys["Numpad1"] = sf::Key::Numpad1;
  availableKeys["Numpad2"] = sf::Key::Numpad2;
  availableKeys["Numpad3"] = sf::Key::Numpad3;
  availableKeys["Numpad4"] = sf::Key::Numpad4;
  availableKeys["Numpad5"] = sf::Key::Numpad5;
  availableKeys["Numpad6"] = sf::Key::Numpad6;
  availableKeys["Numpad7"] = sf::Key::Numpad7;
  availableKeys["Numpad8"] = sf::Key::Numpad8;
  availableKeys["Numpad9"] = sf::Key::Numpad9;
  availableKeys["F1"] = sf::Key::F1;
  availableKeys["F2"] = sf::Key::F2;
  availableKeys["F3"] = sf::Key::F3;
  availableKeys["F4"] = sf::Key::F4;
  availableKeys["F5"] = sf::Key::F5;
  availableKeys["F6"] = sf::Key::F6;
  availableKeys["F7"] = sf::Key::F7;
  availableKeys["F8"] = sf::Key::F8;
  availableKeys["F9"] = sf::Key::F9;
  availableKeys["F10"] = sf::Key::F10;
  availableKeys["F11"] = sf::Key::F11;
  availableKeys["F12"] = sf::Key::F12;
  availableKeys["F13"] = sf::Key::F13;
  availableKeys["F14"] = sf::Key::F14;
  availableKeys["F15"] = sf::Key::F15;
  availableKeys["Pause"] = sf::Key::Pause;

  this->loadKeys( file );
}


bool Input::loadKeys( std::string file )
{
  bool ret = keyfile.open( file );

  for ( int i = keyfile.countKeys( "GLOBAL" ) - 1; i >= 0 ; --i )
    globalKeys[keyfile.getKey( "GLOBAL", i )] = availableKeys[keyfile.getValue( "GLOBAL", i )];


  return ret;
}


bool Input::events( CGame* game )
{
  // Mouse
  game->getGuiManager()->proofMouseClick ( game->getApp()->GetInput() );


  // Tastatur
  while ( game->getApp()->GetEvent( event ) ) {
    // ## Fenster schließen ##
    if ( event.Type == sf::Event::Closed )
      game->stop();

    // ## Tastendruck - global ##
    if ( event.Type == sf::Event::KeyPressed ) {
      if ( event.Key.Code == globalKeys["quit"] ) {
        game->stop();
      }

    } // Tastendruck

//     // ## Mausklick ##
//     game->getGuiManager()->proofMouseClick ( event.Type, event.MouseButton.Button, Input.GetMouseX(), Input.GetMouseY() );
  }

  return false;
}

