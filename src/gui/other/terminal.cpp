/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) <year>  <name of author>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/


#include "../../game.hpp"
#include "../../script/script.hpp"
#include "../../settings/settings.hpp"
#include "terminal.hpp"


namespace gui
{
	

CTerminal::CTerminal()
{
	this->SetSize( sf::Vector2f ( settings::GetWidth() * 0.8f, settings::GetHeight() * 0.2f ) );
	initialized_ = false;
}


void CTerminal::Initialize()
{
	initialized_ = true;
	
	this->AddText ( L" ~ $ " );
}


bool CTerminal::Update ( bool doIt )
{
	gui::CTextArea::Update ( doIt );
	
	if ( doIt ) {
		if ( !initialized_ )
			this->Initialize();

	}
	
	return true;
}

	
void CTerminal::PressedKey ( sf::Key::Code code )
{
	gui::CTextArea::PressedKey ( code );
		
	switch ( code ) {
		case sf::Key::Return:
			this->Run();
			this->AddText( L" ~ $ " );
			break;
				
	}
}


void CTerminal::Run()
{
	this->AddText ( L"\n" );
	std::string str = this->GetRenderedText()->GetText();
	
	try {
		str = str.substr( str.find_last_of ( "~ $ " )+1 );
	} catch ( std::out_of_range e ) {
		GetGameClass()->Error ( (std::string)"out_of_range in " + e.what(), __PRETTY_FUNCTION__, __FILE__, __LINE__ );
		return;
	}
	
	Diluculum::LuaValueList ret;
	try {
		ret = script::GetLua()->doString ( "return " + str );
	} catch ( Diluculum::LuaSyntaxError e ) {
		this->AddText ( L"Syntax Error!\n       " );
		this->AddText ( (sf::Unicode::Text)( e.what() ) );
		this->AddText ( L"\n" );
		return;
	} catch ( Diluculum::LuaRunTimeError e ) {
		this->AddText ( L"RunTimeError!\n       " );
		this->AddText ( (sf::Unicode::Text)( e.what() ) );
		this->AddText ( L"\n" );
		return;
	}
	
	for ( int i = 0; i < ret.size(); ++i ) {
		switch ( ret[i].type() ) {
			case 1:
				if ( ret[i].asBoolean() )
					this->AddText ( L"True" );
				else
					this->AddText ( L"False" );
				this->AddText ( L"\n" );
				break;
			case 3:
				this->AddText ( (sf::Unicode::Text)( util::lCast<std::string>(ret[i].asNumber())) );
				this->AddText ( L"\n" );
				break;
			case 4:
				this->AddText ( (sf::Unicode::Text)(ret[i].asString()) );
				this->AddText ( L"\n" );
				break;
		}
	}
}


} /* namespace gui */
