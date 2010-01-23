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
#include "../../settings/settings.hpp"

#include "text.hpp"
#include <stdexcept>

namespace gui
{
	
	
CText::CText()
{
	font_ = GetGameClass()->GetFontResource()->Get ( settings::GetThemePath() + "/fonts/VeraMono.ttf" );
	
	renderedText_.SetFont ( *font_ );
	renderedText_.SetSize ( 14 );
	
	template_ = renderedText_;
	template_.SetText ( L"O" );
	
	scrollbar_ = 0;
}


void CText::Render()
{
	gui::CWidget::Render();
	sf::RenderWindow* app = GetGameClass()->GetApp();
	
	app->Draw ( renderedText_ );
}


bool CText::Update ( bool doIt )
{
	gui::CWidget::Update ( doIt );
    
	if ( doIt ) {
		renderedText_.SetPosition ( this->GetPositionInScreen() );
       	}
       	
       	return true;
}


bool CText::AddText ( std::wstring str, int position )
{
	bool ret = false;
	
	std::wstring text = this->GetText();
	int posOfLastCharInLine = 0;
	
	/* Calc the length of current line */
	if ( position != -1 || position != text.length() ) {
		std::wstring strBeforePos ( text );
		std::wstring strAfterPos ( L"" );
		
		try {
			strBeforePos = text.substr ( 0, position );
			strAfterPos = text.substr ( position );
		} catch ( std::out_of_range e ) {
			GetGameClass()->Error( (std::string)"out_of_range in " + e.what(), __PRETTY_FUNCTION__, __FILE__, __LINE__ );
			return false;
		}
		
		renderedText_.SetText ( strBeforePos + str + strAfterPos );
		
		std::wstring::size_type lastNewline = text.find_last_of ( '\n' );
		if ( lastNewline == std::string::npos )
			posOfLastCharInLine = renderedText_.GetCharacterPos( lastNewline ).x;
		else
			posOfLastCharInLine = renderedText_.GetCharacterPos( -1 ).x;
		
	} else {
		renderedText_.SetText ( text + str );
		posOfLastCharInLine = renderedText_.GetCharacterPos(-1).x;
	}
	text = this->GetText();
	
	/* Text if too long */
	if ( posOfLastCharInLine+10 > this->GetDimension().GetWidth() + scrollbar_ ) {
		std::wstring::size_type lastSpace = text.find_last_of (' ');
		
		if ( lastSpace == std::string::npos || text.length() - lastSpace > 10 ) {
			lastSpace = text.length();
		}
		
		static std::wstring strUptoSpace;
		static std::wstring strFromSpace;
		try {
			strUptoSpace = text.substr ( 0, lastSpace );
			strFromSpace = text.substr ( lastSpace );
		} catch ( std::out_of_range e ) {
			GetGameClass()->Error ( (std::string)"out_of_range in " + e.what(), __PRETTY_FUNCTION__, __FILE__, __LINE__ );
			return false;
		}
		
		renderedText_.SetText ( strUptoSpace + L"\n" + strFromSpace );
		
		ret = true;
	}
	text = this->GetText();
	
	/* Text is too height */
	if ( renderedText_.GetCharacterPos(-1).y + template_.GetRect().GetHeight() + 5 > this->GetDimension().GetHeight() ) {
		
		if ( scrollbar_ ) {
			
		} else { /* delete first line */
			std::wstring::size_type firstNewline = text.find_first_of ('\n')+1;
			
			try {
				renderedText_.SetText ( text.substr ( firstNewline ) );
			} catch ( std::out_of_range e ) {
				GetGameClass()->Error ( (std::string)"out_of_range in " + e.what(), __PRETTY_FUNCTION__, __FILE__, __LINE__ );
				return false;
			}
		}
	}
	
	return ret;
}


std::wstring CText::GetText()
{
	return renderedText_.GetText();
}


sf::String* CText::GetRenderedText()
{
	return &renderedText_;
}



bool CText::DelText ( int from, int to )
{
	std::wstring str;
	std::wstring strEnd;
	
	try {
		str = this->GetText().substr ( 0, from );
		strEnd = this->GetText().substr ( to );
	} catch ( std::out_of_range e ) {
		GetGameClass()->Error( (std::string)"out_of_range in " + e.what(), __PRETTY_FUNCTION__, __FILE__, __LINE__ );
		return false;
	}
	

	renderedText_.SetText ( str + strEnd );
	
	return true;
}


void CText::SetScollbar ( int scrollbar )
{
	scrollbar_ = scrollbar;
}





} /* namespace gui */
