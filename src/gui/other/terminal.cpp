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
	font_ = GetGameClass()->GetFontResource()->Get ( settings::GetThemePath() + "/fonts/VeraMono.ttf" );
	
	renderedText_.SetFont ( *font_ );
	renderedText_.SetSize ( 14 );
	template_ = renderedText_;
	template_.SetText ( L"O" );
	
	this->SetSize( sf::Vector2f ( settings::GetWidth() * 0.8f, settings::GetHeight() * 0.2f ) );
	initialized_ = false;
	
	historyPos_ = 0;
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
			this->SetCursor ( this->GetText().length() );
			this->Run();
			this->AddText( L" ~ $ " );
			
			historyPos_ = history_.size();
			break;
			
		case sf::Key::Up:
			if ( !history_.empty() ) {
				if ( --historyPos_ < 0 )
					historyPos_ = 0;
				this->OverwriteLastCommandWith ( (sf::Unicode::Text) history_[historyPos_] );
			}
			break;
			
		case sf::Key::Down:
			if ( !history_.empty() ) {
				if ( ++historyPos_ > history_.size()-1 ) {
					historyPos_ = history_.size();
					this->OverwriteLastCommandWith( L"" );
				} else {
					this->OverwriteLastCommandWith( (sf::Unicode::Text) history_[historyPos_] );
				}
			}
			break;
				
	}
}


void CTerminal::Run()
{
	std::string str = this->GetRenderedText()->GetText();
	
	try {
		int prompt = str.find_last_of ( "$" ) + 1;
		str = str.substr ( prompt );
	} catch ( std::out_of_range e ) {
		GetGameClass()->Error ( (std::string)"out_of_range in " + e.what(), __PRETTY_FUNCTION__, __FILE__, __LINE__ );
		return;
	}
	
	/* Save to command history */
	std::string command = str;
	std::string::size_type space = str.find_first_not_of(' ');
	
	/* Delete all spaces before command... */
	if ( space != std::string::npos )
		command = str.substr ( space );

	/* and now after */
	space = command.find_last_of(' ');
	if ( space != std::string::npos )
		command = command.substr ( 0, space );
	
	if ( !command.empty() && command != " " )
		history_.push_back ( command );
	
	/* Add a line break after command */
	this->AddText ( L"\n" );
	
	script::LuaValueList ret;
	try {
		ret = script::GetLua()->doString ( "return " + str );
	} catch ( script::LuaSyntaxError e ) {
		this->AddText ( L"Syntax Error!\n       " );
		this->AddText ( (sf::Unicode::Text)( e.what() ) );
		this->AddText ( L"\n" );
		return;
	} catch ( script::LuaRunTimeError e ) {
		this->AddText ( L"      " );
		this->AddText ( (sf::Unicode::Text)( e.what() ) );
		this->AddText ( L"\n" );
		return;
	}
	
	for ( int i = 0; i < ret.size(); ++i ) {
		switch ( ret[i].type() ) {
			case LUA_TNIL:
				this->AddText ( L"Command not found!\n" );
				break;
			
			case LUA_TBOOLEAN:
				if ( ret[i].asBoolean() )
					this->AddText ( L"True" );
				else
					this->AddText ( L"False" );
				this->AddText ( L"\n" );
				break;
				
			case LUA_TNUMBER:
				this->AddText ( (sf::Unicode::Text)( util::lCast<std::string>(ret[i].asNumber())) );
				this->AddText ( L"\n" );
				break;
				
			case LUA_TSTRING:
				this->AddText ( (sf::Unicode::Text)(ret[i].asString()) );
				this->AddText ( L"\n" );
				break;
		}
	}
}


void CTerminal::SetCursor ( int x )
{
	gui::CTextArea::SetCursor ( x );
	std::string str = this->GetRenderedText()->GetText();
	std::string::size_type last = str.find_last_of ( "$" )+2;
	
	if ( x <= last )
		x = last ;
	if ( !str.empty() && last == str.length() ) {
		this->AddText( L" " );
	}
	
	gui::CTextArea::SetCursor ( x );
}


void CTerminal::OverwriteLastCommandWith ( std::wstring str )
{
	std::string text = this->GetRenderedText()->GetText();
	std::string::size_type last = text.find_last_of ( "$" )+2;
	
	this->DelText ( last, text.length() );
	this->SetCursor ( last );
	this->AddText ( str );
}



} /* namespace gui */
