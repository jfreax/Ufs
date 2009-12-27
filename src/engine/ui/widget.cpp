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

#include <list>

#include "../../game.hpp"
#include "../../action.hpp"

#include "window.hpp"
#include "widget.hpp"

#define sin_d(x)  (sin((x)*M_PI/180))
#define cos_d(x)  (cos((x)*M_PI/180))


namespace gui
{


CWidget::CWidget ( )
{
	static unsigned int globalId = 0;
	id_ = ++globalId;

	CTheme* theme = GetGameClass()->GetGuiManager()->GetTheme();

	motherWin_ = NULL;
	
	/* Standardeinstellungen */
	isMouseHere_ = wasMouseHere_ = false;
	name_ = "";
	
	/* Widget anzeigen */
	this->SetShow ( true );
		
	/* Widgethintergrund zeichnen */
	this->SetDrawBackground ( true );
		
	/* TextPosition */
	this->SetTextPosition ( sf::Vector2f ( 0, 0 ) );
	angle_ = 0;
	
	/* Standard font */
	this->SetFont ( "default" );
}


bool CWidget::Update ( bool doIt )
{
	if ( !doIt )
	{
		update_ = true;
	}

	/* Kein update ausführen! */
	if ( !motherWin_ || !doIt || !update_ )
	{
		return false;
	}
	
	/* Update wird jetzt ausgeführt, erstmal wieder ausschalten */
	update_ = false;
	

	/* Position anpassen, inkl. ggf Minusbereiche */
	position_.x = fakePosition_.x < 0 ? fakePosition_.x + motherWin_->GetSize().x : fakePosition_.x;
	position_.y = fakePosition_.y + motherWin_->GetTitlebarDimension().GetHeight() < 0 ? fakePosition_.y + motherWin_->GetSize().y : fakePosition_.y;


	/* Und nun die Position anhand der Drehung des Fensters berechnen */
	if ( motherWin_->GetAngle() )
	{
		double angle_ = motherWin_->GetAngle();
	
		position_.y = position_.x * sin_d ( -angle_ ) + position_.y * cos_d ( -angle_ );
		position_.x *= cos_d ( angle_ );

		background_.SetRotation( angle_ );
	}


	/* Hintergrundbild, oder... */
	if ( background_.GetSize().x != 1.f )
	{
		background_.Resize ( curSize_ );
		background_.SetCenter ( background_.GetSize() * 0.5f );
		
		/* Bei der Positionsberechnung die Skalierung beachten (weil Center ist local!) */
		sf::Vector2f offset ( background_.GetCenter().x * background_.GetScale().x, background_.GetCenter().y * background_.GetScale().y );

		/* Neue Postion setzen */
		background_.SetPosition ( position_ + motherWin_->GetPosition() + offset );
	}
	/* Hintergrundsprite */
	else
	{
		form_ = sf::Shape::Rectangle ( position_, position_ + curSize_, backgroundColor_, border_, borderColor_ );
	}

	/* Textposition mittig auf den Button setzen */
	text_.SetPosition ( position_ + motherWin_->GetPosition() + textPos_ );
	
		

	return true;
}


void CWidget::Calc()
{
	/* Funktionsanrufe tätigen */
	this->Call();
	
	/* Ggf. "Update"s durchführen */
	this->Update ( true );

	/* Hover Effekt? Unhover? */
	if ( !isMouseHere_ && wasMouseHere_ )
	{
		this->onUnHoverMouse();
		wasMouseHere_ = false;
	}

	isMouseHere_ = false;
}


void CWidget::SetMotherWin ( gui::CWindow* win )
{
	motherWin_ = win;
}


void CWidget::SetPosition ( sf::Vector2f position )
{
	fakePosition_.x = position.x != -1 ? position.x : fakePosition_.x;
	fakePosition_.y = position.y != -1 ? position.y : fakePosition_.y;
	this->Update();
}


void CWidget::MovePosition ( LAYOUT direction, unsigned int distance )
{
	if ( direction == HORIZONTAL ) {
		SetPosition ( sf::Vector2f ( GetPosition().x + distance, -1 ) );
	} else {
		SetPosition ( sf::Vector2f ( -1, GetPosition().y + distance ) );
	}
}


void CWidget::MovePosition ( LAYOUT direction, POSITION to )
{
	if ( direction == HORIZONTAL ) {
		if ( to == RIGHT )
			SetPosition ( sf::Vector2f ( - this->GetDimension().GetWidth() - motherWin_->GetLayoutBorder(), -1 ) );
		else if ( to == CENTER )
			SetPosition ( sf::Vector2f ( ( motherWin_->GetSize().x + this->GetDimension().GetWidth() ) * 0.5f, -1 ) );
		else
			SetPosition ( sf::Vector2f ( motherWin_->GetLayoutBorder(), -1 ) );
	} else {
		if ( to == TOP )
			SetPosition ( sf::Vector2f ( -1, - this->GetDimension().GetHeight() - motherWin_->GetLayoutBorder() ) );
		else if ( to == CENTER )
			SetPosition ( sf::Vector2f ( -1, ( motherWin_->GetSize().y + this->GetDimension().GetHeight() ) * 0.5f ) );
		else
			SetPosition ( sf::Vector2f ( -1, motherWin_->GetLayoutBorder() ) );
	}
}


sf::Vector2f CWidget::GetPosition()
{
	return fakePosition_;
}


sf::Rect<float> CWidget::GetDimension()
{
	return sf::Rect<float> ( fakePosition_.x, fakePosition_.y, fakePosition_.x + curSize_.x, fakePosition_.y + curSize_.y );
}


void CWidget::SetSize ( sf::Vector2f size )
{
	/* Change position of widget */
	curSize_ = size;
	
	/* Update text position */
	this->AdjustTextPosition();
	
	this->Update();
}


void CWidget::AdjustSize()
{
	this->SetSize ( sf::Vector2f ( text_.GetRect().GetWidth() + textPos_.x, text_.GetRect().GetHeight() + textPos_.y ) );
}


void CWidget::SetName ( std::string name )
{
	name_ = name;
	text_.SetText ( name );

	this->Update();
}


void CWidget::SetFontSize ( int size )
{
	text_.SetSize ( size );
}


void CWidget::SetDrawBackground ( bool ison )
{
	drawBackground_ = ison;
}


void CWidget::SetBackground ( sf::Sprite background )
{
	background_ = background;
	this->Update();
}


sf::Sprite* CWidget::GetBackground()
{
	return &background_;
}


void CWidget::SetBackground ( sf::Image* background )
{
	/* Image setzen */
	background_.SetImage ( *background );
	
	/* Größe vom Sprite anpassen */
	background_.Resize( background->GetWidth(), background->GetHeight() );
}


void CWidget::SetBackgroundColor ( sf::Color color )
{
	backgroundColor_ = color;
}


sf::Color CWidget::GetBackgroundColor()
{
	return backgroundColor_;
}


CWindow* CWidget::GetMotherWin()
{
	return motherWin_;
}


sf::Rect< float > CWidget::GetDimensionInScreen()
{
	sf::Vector2f motherPosition = motherWin_->GetPosition();
	return sf::Rect< float > ( motherPosition.x + position_.x, motherPosition.y + position_.y, motherPosition.x + position_.x + curSize_.x, motherPosition.y + position_.y + curSize_.y );
}


void CWidget::SetShow ( bool ison )
{
	show_ = ison;
}


bool CWidget::GetShow()
{
	return show_;
}


std::string CWidget::GetName()
{
	return name_;
}


void CWidget::SetText ( std::string text )
{
	text_.SetText( text );
	
	/* Update text position */
	this->AdjustTextPosition();

	this->Update();
}


sf::String CWidget::GetText()
{
	return text_;
}


void CWidget::SetTextPosition ( sf::Vector2f pos )
{
	textPos_ = pos;
	this->Update();
}


sf::Vector2f CWidget::GetTextPosition()
{
	return textPos_;
}


void CWidget::SetTextSize ( int size )
{
	text_.SetSize( size );
}


void CWidget::SetFont ( std::string fontname )
{
	sf::Font* font = GetGameClass()->GetFontResource()->Get( settings::GetThemePath() + "/fonts/" + fontname + ".ttf" );

	if ( font )
	{
		text_.SetFont( *font );
	}
}


/* ------- PRIVATE METHODS ------- */


void CWidget::AdjustTextPosition()
{
	if ( curSize_.x > text_.GetRect().GetWidth() && curSize_.y > text_.GetRect().GetHeight() )
		this->SetTextPosition ( sf::Vector2f ( ( curSize_.x - text_.GetRect().GetWidth() ) * 0.5f, ( curSize_.y - text_.GetRect().GetHeight() ) * 0.5f ) );
}


} // namespace gui
