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

#include "collision.hpp"

#include <iostream>
#include <SFML/Graphics.hpp>

bool isCollision ( sf::Rect<float> rect1, sf::Rect<float> rect2 )
{
	/*R1P1
	  #################
	  #               #
	  #               #
	  #         R2P1 #
	  #          #################
	  #          #++++#          #
	  #          #++++#          #
	  #################          #
	             #    R1P2       #
	             #               #
	             #               #
	             #################
	                            R2P2
	*/
	if ( rect2.Left <= rect1.Right )
		if ( rect2.Right >= rect1.Left )
			if ( rect2.Bottom >= rect1.Top )
				if ( rect2.Top <= rect1.Bottom )
				{
					return true;
				}

	return false;
}
