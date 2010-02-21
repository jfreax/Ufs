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
#include "../../gui/window/tooltip.hpp"

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
	
	/* Standard properties */
	isMouseHere_ = wasMouseHere_ = unHover_ = false;
	name_ = "";
	
	/* Initialize tooltip window */
	toolTip_ = NULL;
	showTooltip_ = 0;
	hasTooltip_ = false;
	
	/* Widget should be draw */
	this->SetShow ( true );
		
	/* Draw a background */
	this->SetDrawBackground ( true );
		
	/* Positions */
	this->SetPosition ( sf::Vector2f ( 0, 0 ) );
	this->SetTextPosition ( sf::Vector2f ( 0, 0 ) );
	angle_ = 0;
	
	/* Standard font */
	this->SetFont ( "default" );
}


CWidget::~CWidget()
{
	delete toolTip_;
	
}



bool CWidget::Update ( bool doIt )
{
	if ( !doIt )
		update_ = true;

	/* Kein update ausführen! */
	if ( !motherWin_ || !doIt || !update_ )
		return false;
	
	/* Update wird jetzt ausgeführt, erstmal wieder ausschalten */
	update_ = false;	

	/* Position anpassen, inkl. ggf Minusbereiche */
	position_.x = fakePosition_.x < 0 ? fakePosition_.x + motherWin_->GetSize().x : fakePosition_.x;
	position_.y = fakePosition_.y + motherWin_->GetTitlebarDimension().GetHeight() < 0 ? fakePosition_.y + motherWin_->GetSize().y : fakePosition_.y;


	/* Und nun die Position anhand der Drehung des Fensters berechnen */
	if ( motherWin_->GetAngle() ) {
		double angle_ = motherWin_->GetAngle();
	
		position_.y = position_.x * sin_d ( -angle_ ) + position_.y * cos_d ( -angle_ );
		position_.x *= cos_d ( angle_ );

		background_.SetRotation( angle_ );
	}


	/* Hintergrundbild, oder... */
	if ( background_.GetSize().x != 1.f ) {
// 		background_.Resize ( curSize_ );
		background_.SetCenter ( background_.GetSize() * 0.5f );
		
		/* Bei der Positionsberechnung die Skalierung beachten (weil Center ist local!) */
		sf::Vector2f offset ( background_.GetCenter().x * background_.GetScale().x, background_.GetCenter().y * background_.GetScale().y );

		/* Neue Postion setzen */
		background_.SetPosition ( position_ + motherWin_->GetPosition() + offset );
	}
	/* Hintergrundsprite */
	else {
// 		form_ = sf::Shape::Rectangle ( position_, position_ + curSize_, backgroundColor_, border_, borderColor_ );
	}

	/* Set text position centered on button */
	text_.SetPosition ( position_ + motherWin_->GetPosition() + textPos_ );
	
	/* Change tooltip behavior */
	if ( hasTooltip_ )
		toolTip_->Update();
	

	return true;
}


void CWidget::Render()
{
	if ( hasTooltip_ && showTooltip_ > 0 ) {
		toolTip_->ChangeTransparency ( showTooltip_*8 );
		toolTip_->Render();
	}
}


bool CWidget::onHoverMouse()
{
	unHover_ = false;
	
	if ( hasTooltip_ && showTooltip_ < 31 ) {
		++showTooltip_;
	}
	
}


bool CWidget::onUnHoverMouse()
{
	if ( hasTooltip_ ) {
		lastMouseUnHover_.Reset();
		unHover_ = true;
	}
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


sf::Vector2f CWidget::GetPositionInScreen()
{
	return sf::Vector2f ( motherWin_->GetPosition().x + position_.x, motherWin_->GetPosition().y + position_.y );
}



sf::Rect<float> CWidget::GetDimension()
{
	return sf::Rect<float> ( fakePosition_.x, fakePosition_.y, fakePosition_.x + curSize_.x, fakePosition_.y + curSize_.y );
}


sf::Rect< float > CWidget::GetDimensionInScreen()
{
	if ( motherWin_ == NULL ) {
		return sf::Rect< float > ( 0, 0, 0, 0 );
	}
	
	sf::Vector2f motherPosition = motherWin_->GetPosition();
	return sf::Rect< float > ( motherPosition.x + position_.x, motherPosition.y + position_.y, motherPosition.x + position_.x + curSize_.x, motherPosition.y + position_.y + curSize_.y );
}


void CWidget::SetSize ( sf::Vector2f size )
{
	/* Change position of widget */
	curSize_ = size;
	
	/* Update text position */
	this->AdjustTextPosition();
	
	this->Update();
}


void CWidget::AdjustSize ( unsigned int border )
{
// 	if ( static_cast <std::string> (text_.GetText()).empty() )
// 		this->SetSize ( motherWin_->GetSize() );
// 	else
		this->SetSize ( sf::Vector2f ( text_.GetRect().GetWidth() + textPos_.x + border, text_.GetRect().GetHeight() + textPos_.y + border ) );
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


void CWidget::SetBackground ( sf::Sprite* background )
{
	background_.Clear();
	background_.Add ( background );
	this->Update();
}


void CWidget::SetBackground ( sf::Image* background )
{
	if ( background == NULL )
		return;
	
	background_.Clear();
	background_.Add ( background )->Resize( background->GetWidth(), background->GetHeight() );
}


void CWidget::SetBackground ( CGraphic* background )
{
	background_ = *background;
}



CGraphic* CWidget::GetBackground()
{
	return &background_;
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
	
	this->AdjustSize();
	this->Update();
}


void CWidget::SetText ( HEADERNAME header, std::string text )
{
	text_.SetText( l ( header, text ) );
	
	/* Update text position */
	this->AdjustTextPosition();
	
	this->AdjustSize();
	this->Update();
}


sf::String* CWidget::GetText()
{
	return &text_;
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


void CWidget::Calc()
{
	/* Funktionsanrufe tätigen */
// 	this->Call();
	
	/* Ggf. "Update"s durchführen */
	this->Update ( true );
	
	/* Hover Effekt? Unhover? */
	if ( !isMouseHere_ && wasMouseHere_ )
	{
		this->onUnHoverMouse();
		wasMouseHere_ = false;
	}
	
	isMouseHere_ = false;
	
	/* Fade out tooltip */
	if ( hasTooltip_ && showTooltip_ > 0 && unHover_ ) {
		showTooltip_ -= lastMouseUnHover_.GetElapsedTime();
	}
}


void CWidget::AdjustTextPosition()
{
	if ( curSize_.x > text_.GetRect().GetWidth() && curSize_.y > text_.GetRect().GetHeight() )
		this->SetTextPosition ( sf::Vector2f ( ( curSize_.x - text_.GetRect().GetWidth() ) * 0.5f, ( curSize_.y - text_.GetRect().GetHeight() ) * 0.5f ) );
}


CWindow* CWidget::ActivateTooltip ( std::string text )
{
	toolTip_ = new CTooltip ( this, text );
	hasTooltip_ = true;
}


CWindow* CWidget::ActivateTooltip ( HEADERNAME header, std::string var )
{
	this->ActivateTooltip ( l ( header, var ) );
}


} // namespace gui
