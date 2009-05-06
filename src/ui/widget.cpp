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
	this->SetTextPosition ( sf::Vector2f ( 0, 0 ) );
	angle_ = 0;
}


bool CWidget::Update ( bool doIt )
{
	if ( !doIt )
	{
		update_ = true;
	}

	/* Kein update ausf�hren! */
	if ( !motherWin_ || !doIt || !update_ )
	{
		return false;
	}
	
	/* Update wird jetzt ausgef�hrt, erstmal wieder ausschalten */
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


void CWidget::Calc ( void )
{
	/* Funktionsanrufe t�tigen */
	this->Call();
	
	/* Ggf. "Update"s durchf�hren */
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


sf::Vector2f CWidget::GetPosition ( void )
{
	return position_;
}


sf::Rect<float> CWidget::GetDimension ( void )
{
	sf::Vector2f pos;
	sf::Vector2f posEnd;
	
	/* Die gr��en vom Hintergrundbild nehmen */
	if ( drawBackground_ && background_.GetSize().x != 1.f )
	{
		pos = background_.TransformToGlobal( sf::Vector2f ( 0, 0 ) );
		posEnd = background_.TransformToGlobal( curSize_ );
	}
	else
	/* Oder wenn keins vorhanden ist, dann vom Hintergrundshape */
	{
		pos = form_.TransformToGlobal( sf::Vector2f ( 0, 0 ) );
		posEnd = form_.TransformToGlobal( curSize_ );
	}


	return sf::Rect<float> ( pos.x, pos.y, posEnd.x, posEnd.y );
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
	this->Update();
}


sf::Sprite* CWidget::GetBackground ( void )
{
	return &background_;
}


void CWidget::SetBackground ( sf::Image* background )
{
	/* Image setzen */
	background_.SetImage ( *background );
	
	/* Gr��e vom Sprite anpassen */
	background_.Resize( background->GetWidth(), background->GetHeight() );
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


sf::String CWidget::GetText ( void )
{
	return text_;
}


void CWidget::SetTextPosition ( sf::Vector2f pos )
{
	textPos_ = pos;
	this->Update();
}


sf::Vector2f CWidget::GetTextPosition ( void )
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


} // namespace gui
