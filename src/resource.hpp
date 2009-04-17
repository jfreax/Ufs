namespace sf
{
class Image;
}
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

#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include <iostream>
#include <typeinfo>
#include <map>

#include <SFML/Graphics.hpp>

#include "util.hpp"
#include "settings.hpp"

template < typename T >
class CResource
{
  public:
    T* Get ( std::string name_ ) {
      if ( !data [ name_ ] ) {
        return Push ( name_ );
      } else {
        return data [ name_ ];
      }
    }


    T* Push ( std::string filename_ ) {
      if ( typeid ( T ).name() == typeid ( sf::Image ).name() ) {

        sf::Image* img = new sf::Image;

        if ( !img->LoadFromFile ( settings::GetPath() + filename_ ) ) {
          // TODO Logfile
          std::cerr << "Error loading backgroundimage: " << filename_ << std::endl;
        }

        data [ filename_ ] = (T*)img;
        return (T*)img;
      }
    }

  private:
    std::map< std::string, T* > data;
};

#endif // RESOURCE_HPP
