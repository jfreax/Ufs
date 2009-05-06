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


#include "../../ui/button.hpp"

#ifndef CLOSE_HPP
#define CLOSE_HPP

namespace gui
{


/* CLOSE BUTTON 
   ------------ */
class CCloseButton : public CButton
{
	public:
		CCloseButton ( void );
		bool onLeftClick ( void );
	private:
};


/* MINIMIZE BUTTON 
   --------------- */
class CMinimizeButton : public CButton
{
	public:
		CMinimizeButton ( void );
		
		bool onLeftClick ( void );
		bool Call ( void );
		
	private:
		float originalSize_;
	
		bool doMinimize_;
		bool doMaximize_;
		
		double diffRotate_;
		double diff_;
};


/* HIDE BUTTON 
   ----------- */
class CHideButton : public CButton
{
	public:
		CHideButton ( CButton* minimizeButton );
		
		bool onLeftClick ( void );
		bool Call ( void );
	
	private:
		CButton* minimizeButton_;
		
		double diff_;
		double diffRotate_;
		
		bool finish_;
		
		bool doHide_;
		bool doShow_;
		
		sf::Vector2f originalSize_;
		sf::Vector2f originalPosition_;
		
		sf::Vector2f dockPosition_;
};


} /* namespace gui */

#endif // CLOSE_HPP
