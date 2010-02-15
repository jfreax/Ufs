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
	cursorPos_   = 0;
	background_  = NULL;
	
	backgroundColor_ = sf::Color ( 10, 10, 10, 200);
	
}


void CTextArea::Initialize()
{
	initialized_ = true;
	cursor_ = sf::Shape::Rectangle ( 1, 2, template_.GetRect().GetWidth(), template_.GetRect().GetHeight()+2, sf::Color ( 255, 255, 255, 160 ) );
}


void CTextArea::Render()
{
	sf::RenderWindow* app = GetGameClass()->GetApp();

	/* Draw at first the background */
	app->Draw ( *background_ );
	
	/* Then the cursor */
	if ( cursorTime_.GetElapsedTime() > .4f ) {
		app->Draw ( cursor_ );
		
		if ( cursorTime_.GetElapsedTime() > 1.f )
			cursorTime_.Reset();
	}
	
	/* And now the rest */
	gui::CText::Render();
}


bool CTextArea::Update ( bool doIt )
{
	gui::CText::Update ( doIt );
	
	if ( doIt ) {
		/* update the graphic position */	
		cursor_.SetPosition ( sf::Vector2f ( this->GetRenderedText()->GetCharacterPos(cursorPos_).x + this->GetPositionInScreen().x,
						     this->GetRenderedText()->GetCharacterPos(cursorPos_).y + this->GetPositionInScreen().y ) );
						     
		this->CalcBackground();
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
		case sf::Key::Delete:
			this->DelText ( cursorPos_, cursorPos_+1 );
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
	SetCursor ( cursorPos_ + x );
}


void CTextArea::CalcBackground()
{
	static sf::Vector2f oldSize;
	
	if ( oldSize.x == this->GetDimension().GetWidth() && oldSize.y == this->GetDimension().GetHeight() ) {
		background_->SetPosition ( this->GetPositionInScreen() );
		return;
	}
	
	oldSize.x = this->GetDimension().GetWidth();
	oldSize.y = this->GetDimension().GetHeight();
	
	/* altes Shape löschen */
	delete background_;
	background_ = new sf::Shape();
	
	/* Variablen deklarieren */
	float Angle;
	sf::Vector2f Center;
	float formRound = 10;
	
	/* obere linke Ecke */
	if ( this->GetPositionInScreen().y < 2 ) {
		background_->AddPoint ( 0, 0, backgroundColor_ );
	} else {
		Center = sf::Vector2f ( formRound, formRound );
		for ( int i = 40; i < 60; ++i ) {
			Angle = i * 2 * 3.141592654f / 80;
			sf::Vector2f Offset ( cos ( Angle ), sin ( Angle ) );
			background_->AddPoint ( Center + Offset * formRound, backgroundColor_ );	
		}
	}
	
	/* obere rechte Ecke */
	if ( this->GetPositionInScreen().y < 2 ) {
		background_->AddPoint ( this->GetDimension().GetWidth(), 0, backgroundColor_ );
	} else {
		Center = sf::Vector2f ( this->GetDimension().GetWidth() - formRound, formRound );
		for ( int i = 60; i < 80; ++i ) {
			Angle = i * 2 * 3.141592654f / 80;
			sf::Vector2f Offset ( cos ( Angle ), sin ( Angle ) );
			background_->AddPoint ( Center + Offset * formRound, backgroundColor_ );
		}
	}
	
	/* rechte untere Rundung */
	Center = sf::Vector2f ( this->GetDimension().GetWidth() - formRound, this->GetDimension().GetHeight() - formRound );
	
	for ( int i = 0; i < 20; ++i ) {
		Angle = i * 2 * 3.141592654f / 80;
		sf::Vector2f Offset ( cos ( Angle ), sin ( Angle ) );
		background_->AddPoint ( Center + Offset * formRound, backgroundColor_ );
	}
	
	/* Farbanpassungen */
	sf::Color lastColor = backgroundColor_;
	
	lastColor.a = 160;
	
	/* linke untere Rundung */
	Center = sf::Vector2f ( formRound, this->GetDimension().GetHeight() - formRound );
	
	for ( int i = 20; i < 40; ++i ) {
		Angle = i * 2 * 3.141592654f / 80;
		sf::Vector2f Offset ( cos ( Angle ), sin ( Angle ) );
		background_->AddPoint ( Center + Offset * formRound, lastColor );
	}
	
	background_->SetPosition ( this->GetPositionInScreen() );
}

	
} /* namespace gui */