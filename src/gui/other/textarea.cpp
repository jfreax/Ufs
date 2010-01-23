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

#include "textarea.hpp"


namespace gui
{

	
CTextArea::CTextArea()
{
	initialized_ = false;
	cursorPos_ = 0;
}


void CTextArea::Initialize()
{
	initialized_ = true;
	
// 	this->AddText ( L" ~~ $ " );
	cursor_ = sf::Shape::Line ( 3, 4, 3, template_.GetRect().GetHeight(), 2.f, sf::Color::Black );
}


void CTextArea::Render()
{
	gui::CText::Render();
	sf::RenderWindow* app = GetGameClass()->GetApp();
	
	app->Draw ( cursor_ );
}


bool CTextArea::Update ( bool doIt )
{
	gui::CText::Update ( doIt );
	
	if ( doIt ) {
		if ( !initialized_ )
			this->Initialize();
		
		/* update the graphic position */	
		cursor_.SetPosition ( sf::Vector2f ( this->GetRenderedText()->GetCharacterPos(cursorPos_).x + this->GetPositionInScreen().x,
						     this->GetRenderedText()->GetCharacterPos(cursorPos_).y + this->GetPositionInScreen().y ) );
	}
	
	return true;
}


bool CTextArea::onLeftClick()
{
	gui::CWidget::onLeftClick();
	GetGameClass()->GetGuiManager()->FocusTextArea ( this );
	
	return true;
}


void CTextArea::PressedKey ( sf::Key::Code code )
{
	gui::CWidget::PressedKey ( code );
	
	switch ( code ) {
		case sf::Key::Back:
			if ( this->DelText ( cursorPos_-1, cursorPos_ ) )
				this->MoveCursor( -1 );
			break;
		case sf::Key::Left:
			this->MoveCursor ( -1 );
			break;
		case sf::Key::Right:
			this->MoveCursor ( 1 );
			break;
		
	}
}


bool CTextArea::AddText ( std::wstring str )
{
	if ( gui::CText::AddText ( str, cursorPos_ ) )
		this->MoveCursor( 1 );
	
	this->MoveCursor( str.length() );
}


void CTextArea::SetCursor ( int x )
{
	cursorPos_ = x;
	
	if ( cursorPos_ < 0 )
		cursorPos_ = 0;
	if ( cursorPos_ > this->GetText().length() )
		cursorPos_ = this->GetText().length();
	
	/* update the graphic position */	
	cursor_.SetPosition ( sf::Vector2f ( this->GetRenderedText()->GetCharacterPos(cursorPos_).x + this->GetPositionInScreen().x,
					     this->GetRenderedText()->GetCharacterPos(cursorPos_).y + this->GetPositionInScreen().y ) );
}


void CTextArea::MoveCursor ( int x )
{
	this->SetCursor ( cursorPos_ + x );
}



	
} /* namespace gui */