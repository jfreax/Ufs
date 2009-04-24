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
#include "util.hpp"


CInput::CInput ( std::string file )
{
	availableKeys["a"] = sf::Key::A;
	availableKeys["b"] = sf::Key::B;
	availableKeys["c"] = sf::Key::C;
	availableKeys["d"] = sf::Key::D;
	availableKeys["e"] = sf::Key::E;
	availableKeys["f"] = sf::Key::F;
	availableKeys["g"] = sf::Key::G;
	availableKeys["h"] = sf::Key::H;
	availableKeys["i"] = sf::Key::I;
	availableKeys["j"] = sf::Key::J;
	availableKeys["k"] = sf::Key::K;
	availableKeys["l"] = sf::Key::L;
	availableKeys["m"] = sf::Key::M;
	availableKeys["n"] = sf::Key::N;
	availableKeys["o"] = sf::Key::O;
	availableKeys["p"] = sf::Key::P;
	availableKeys["q"] = sf::Key::Q;
	availableKeys["r"] = sf::Key::R;
	availableKeys["s"] = sf::Key::S;
	availableKeys["t"] = sf::Key::T;
	availableKeys["u"] = sf::Key::U;
	availableKeys["v"] = sf::Key::V;
	availableKeys["w"] = sf::Key::W;
	availableKeys["x"] = sf::Key::X;
	availableKeys["y"] = sf::Key::Y;
	availableKeys["z"] = sf::Key::Z;
	availableKeys["num0"] = sf::Key::Num0;
	availableKeys["num1"] = sf::Key::Num1;
	availableKeys["num2"] = sf::Key::Num2;
	availableKeys["num3"] = sf::Key::Num3;
	availableKeys["num4"] = sf::Key::Num4;
	availableKeys["num5"] = sf::Key::Num5;
	availableKeys["num6"] = sf::Key::Num6;
	availableKeys["num7"] = sf::Key::Num7;
	availableKeys["num8"] = sf::Key::Num8;
	availableKeys["num9"] = sf::Key::Num9;
	availableKeys["escape"] = sf::Key::Escape;
	availableKeys["lcontrol"] = sf::Key::LControl;
	availableKeys["lshift"] = sf::Key::LShift;
	availableKeys["lalt"] = sf::Key::LAlt;
	availableKeys["lsystem"] = sf::Key::LSystem;
	availableKeys["rcontrol"] = sf::Key::RControl;
	availableKeys["rshift"] = sf::Key::RShift;
	availableKeys["ralt"] = sf::Key::RAlt;
	availableKeys["rsystem"] = sf::Key::RSystem;
	availableKeys["menu"] = sf::Key::Menu;
	availableKeys["lbracket"] = sf::Key::LBracket;
	availableKeys["rbracket"] = sf::Key::RBracket;
	availableKeys["semicolon"] = sf::Key::SemiColon;
	availableKeys["comma"] = sf::Key::Comma;
	availableKeys["period"] = sf::Key::Period;
	availableKeys["quote"] = sf::Key::Quote;
	availableKeys["slash"] = sf::Key::Slash;
	availableKeys["backSlash"] = sf::Key::BackSlash;
	availableKeys["tilde"] = sf::Key::Tilde;
	availableKeys["equal"] = sf::Key::Equal;
	availableKeys["dash"] = sf::Key::Dash;
	availableKeys["space"] = sf::Key::Space;
	availableKeys["return"] = sf::Key::Return;
	availableKeys["back"] = sf::Key::Back;
	availableKeys["tab"] = sf::Key::Tab;
	availableKeys["pageup"] = sf::Key::PageUp;
	availableKeys["pagedown"] = sf::Key::PageDown;
	availableKeys["end"] = sf::Key::End;
	availableKeys["home"] = sf::Key::Home;
	availableKeys["insert"] = sf::Key::Insert;
	availableKeys["delete"] = sf::Key::Delete;
	availableKeys["add"] = sf::Key::Add;
	availableKeys["subtract"] = sf::Key::Subtract;
	availableKeys["multiply"] = sf::Key::Multiply;
	availableKeys["divide"] = sf::Key::Divide;
	availableKeys["left"] = sf::Key::Left;
	availableKeys["right"] = sf::Key::Right;
	availableKeys["up"] = sf::Key::Up;
	availableKeys["down"] = sf::Key::Down;
	availableKeys["numpad0"] = sf::Key::Numpad0;
	availableKeys["numpad1"] = sf::Key::Numpad1;
	availableKeys["numpad2"] = sf::Key::Numpad2;
	availableKeys["numpad3"] = sf::Key::Numpad3;
	availableKeys["numpad4"] = sf::Key::Numpad4;
	availableKeys["numpad5"] = sf::Key::Numpad5;
	availableKeys["numpad6"] = sf::Key::Numpad6;
	availableKeys["numpad7"] = sf::Key::Numpad7;
	availableKeys["numpad8"] = sf::Key::Numpad8;
	availableKeys["numpad9"] = sf::Key::Numpad9;
	availableKeys["f1"] = sf::Key::F1;
	availableKeys["f2"] = sf::Key::F2;
	availableKeys["f3"] = sf::Key::F3;
	availableKeys["f4"] = sf::Key::F4;
	availableKeys["f5"] = sf::Key::F5;
	availableKeys["f6"] = sf::Key::F6;
	availableKeys["f7"] = sf::Key::F7;
	availableKeys["f8"] = sf::Key::F8;
	availableKeys["f9"] = sf::Key::F9;
	availableKeys["f10"] = sf::Key::F10;
	availableKeys["f11"] = sf::Key::F11;
	availableKeys["f12"] = sf::Key::F12;
	availableKeys["f13"] = sf::Key::F13;
	availableKeys["f14"] = sf::Key::F14;
	availableKeys["f15"] = sf::Key::F15;
	availableKeys["pause"] = sf::Key::Pause;

	this->LoadKeys ( file );
}


bool CInput::LoadKeys ( std::string file )
{
	bool ret = keyfile.Open ( file, false, true );

	for ( int i = keyfile.CountKeys ( "global" ) - 1; i >= 0 ; --i )
	{
		KeyHolder keyHolder;
		keyHolder.strg = keyHolder.shift = keyHolder.alt = 0;

		std::string keysToPress = keyfile.GetValue ( "global", i );
		keysToPress = "+" + keysToPress;
		util::deleteChar ( keysToPress );


		for ( size_t found = 0; found != std::string::npos; found = keysToPress.find ( '+', found + 1 ) )
		{
			std::string keyTmp = keysToPress.substr ( found + 1, keysToPress.find ( '+', found + 1 ) - found - 1 );

			if ( keyTmp == "strg" )
				keyHolder.strg = true;
			else if ( keyTmp == "alt" )
				keyHolder.alt = true;
			else if ( keyTmp == "shift" )
				keyHolder.shift = true;
			else
				keyHolder.key = availableKeys[ keyTmp ];
		}

		globalKeys[keyfile.GetKey ( "global", i ) ] = keyHolder;
	}

	return ret;
}


bool CInput::TestPressedKeys ( std::string keyName_ )
{
	bool ret;
	std::string keyName = keyName_;

	for ( int i = 0; globalKeys[keyName].key; keyName = keyName_ + "-" + util::lCast< std::string > ( i ) )
	{
		ret = true;

		if ( event.Key.Code != globalKeys[keyName].key )
			ret = false;

		if ( event.Key.Control != globalKeys[keyName].strg )
			ret = false;

		if ( event.Key.Alt != globalKeys[keyName].alt )
			ret = false;

		if ( event.Key.Shift != globalKeys[keyName].shift )
			ret = false;

		if ( ret )
			return true;
		else
			++i;
	}

	return false;
}


bool CInput::Events ( void )
{
	// lokale Variablen
	sf::RenderWindow *app = GetGameClass()->GetApp();
	gui::CManager* guiManager = GetGameClass()->GetGuiManager();
	const sf::Input* input = &app->GetInput();



	// Mouse
	if ( input->IsMouseButtonDown ( sf::Mouse::Left ) )
	{
		guiManager->MouseClick ( input->GetMouseX(), input->GetMouseY(), sf::Mouse::Left );
	}
	else if ( input->IsMouseButtonDown ( sf::Mouse::Right ) )
	{
		guiManager->MouseClick ( input->GetMouseX(), input->GetMouseY(), sf::Mouse::Right );
	}
	else if ( input->IsMouseButtonDown ( sf::Mouse::Middle ) )
	{
		guiManager->MouseClick ( input->GetMouseX(), input->GetMouseY(), sf::Mouse::Middle );
	}
	else
	{
		guiManager->MouseHover ( input->GetMouseX(), input->GetMouseY() );
	}


	// Tastatur

	while ( app->GetEvent ( event ) )
	{
		// ## Spiel beenden (aka Schließen) ##
		if ( event.Type == sf::Event::Closed )
			GetGameClass()->Stop();

		// ## Tastendruck - global ##
		if ( event.Type == sf::Event::KeyPressed )
		{
			if ( TestPressedKeys ( "quit" ) )
				GetGameClass()->Stop();
			else if ( TestPressedKeys ( "fullscreen" ) )
				settings::ToggleFullscreen();
			else if ( TestPressedKeys ( "close" ) )
				guiManager->CloseWindow ( );

		} // Tastendruck
	}

	return false;
}

