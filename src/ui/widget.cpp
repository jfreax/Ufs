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

#include "../game.hpp"
#include "../action.hpp"

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
	
	/* Widget anzeigen */
	this->SetShow ( true );
		
	/* Widgethintergrund zeichnen */
	this->SetDrawBackground ( true );
		
	/* TextPosition */
	this->SetTextPosition ( this->GetPosition() );

	angle_ = 0;
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
	else
	{
		background_.SetRotation( 0 );
	}


	/* Hintergrundbild, oder... */
	if ( background_.GetSize().x != 1.f )
	{
		background_.SetPosition ( position_ + motherWin_->GetPosition() + background_.GetCenter() );
		background_.Resize ( curSize_ );
	}
	/* Hintergrundsprite */
	else
	{
		form_ = sf::Shape::Rectangle ( position_, position_ + curSize_, backgroundColor_, border_, borderColor_ );
	}

	/* Textposition mittig auf den Button setzen */
	sf::Vector2f textPos ( ( curSize_.x - text_.GetRect().GetWidth() ) * 0.5f, ( curSize_.y - text_.GetRect().GetHeight() ) * 0.5f );
	text_.SetPosition ( position_ + motherWin_->GetPosition() + textPos );
	
		

	return true;
}


void CWidget::Calc ( void )
{
	/* Funktionsanrufe tätigen */
	this->Call();
	
	/* Ggf. "Update"s durchführen */
	this->Update( true );

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
	fakePosition_ = position;
	this->Update();
}


sf::Vector2f CWidget::GetPosition ( )
{
	return position_;
}


void CWidget::SetSize ( sf::Vector2f size )
{
	curSize_ = size;
	this->Update();
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
	background_.SetCenter ( background_.GetSize() * 0.5f );
}


sf::Sprite* CWidget::GetBackground ( void )
{
	return &background_;
}


void CWidget::SetBackground ( sf::Image* background )
{
	background_.SetImage ( *background );
}


void CWidget::SetBackgroundColor ( sf::Color color )
{
	backgroundColor_ = color;
}


sf::Color CWidget::GetBackgroundColor ( void )
{
	return backgroundColor_;
}


CWindow* CWidget::GetMotherWin ( void )
{
	return motherWin_;
}


sf::Rect< float > CWidget::GetDimensionInScreen ( void )
{
	sf::Vector2f motherPosition = motherWin_->GetPosition();
	return sf::Rect< float > ( motherPosition.x + position_.x, motherPosition.y + position_.y, motherPosition.x + position_.x + curSize_.x, motherPosition.y + position_.y + curSize_.y );
}


void CWidget::SetShow ( bool ison )
{
	show_ = ison;
}


bool CWidget::GetShow ( void )
{
	return show_;
}


std::string CWidget::GetName ( void )
{
	return name_;
}


void CWidget::SetText ( std::string text )
{
	text_.SetText( text );
	this->Update();
}


std::string CWidget::GetText ( void )
{
	return text_.GetText();
}


void CWidget::SetTextPosition ( sf::Vector2f pos )
{
	text_.SetPosition ( pos );
	this->Update();
}


} // namespace gui
