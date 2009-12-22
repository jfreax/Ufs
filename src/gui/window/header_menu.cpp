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

#include "header_menu.hpp"
#include "../../settings/settings.hpp"
#include "../../engine/ui/button.hpp"


namespace gui
{


CHeaderWindow::CHeaderWindow ( void )
{
	/* Keine Titelleiste */
	this->SetTitlebar ( 0 );
	
	/* Temporäre Variablen */
	int width = settings::GetWidth();
	int height = settings::GetHeight();
	
	/* Position und Größe */
	this->SetPosition( sf::Vector2f ( width * 0.2f, 0.f ) );
	this->SetSize ( sf::Vector2f ( width * 0.15, 25.f ) );
	
	/* Nicht verschiebbar */
	this->SetMoveAble ( false );
	
        /* do not close */
// 	this->SetCloseAble ( false );

        /* Add buttons */
	CButton* button = new CButton;
	this->AddWidget ( button );
	
	button->SetDrawBackground ( false );
	button->SetSize ( this->GetSize() );
	button->SetText ( "TEST" );
}



} /* namespace gui */
