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
#include "../../gui/button/header.hpp"
#include "../../gui/other/spacer.hpp"


namespace gui
{


CHeaderWindow::CHeaderWindow()
{
	gui::CWindow::CWindow();
	
	/* No titlebar */
	this->SetTitlebar ( 0 );
	
	/* Temp. vars */
	int width = settings::GetWidth();
	int height = settings::GetHeight();
	
	this->SetPosition( sf::Vector2f ( width * 0.2f, 0.f ) );
	this->SetMoveAble ( false );
	this->SetCloseAble ( false );
	this->SetLayout ( HORIZONTAL, 5 );

        /* Add buttons */
	this->AddWidget ( new CSpacer ( VERTICAL, 3 ) );
	this->AddWidget ( new CSpacer ( HORIZONTAL, 3 ) );
	this->AddWidget ( new CHeaderMainButton );
	this->AddWidget ( new CHeaderStatsButton );
	this->AddWidget ( new CHeaderStatsButton );
	
	this->UpdateWidgets();
	
	/* Set size (depend on buttons) */
	this->SetSize ( sf::Vector2f ( GetWidget ( -1 )->GetPosition().x + GetWidget ( -1 )->GetDimension().GetWidth() + 3, 28 ) );
}



} /* namespace gui */
