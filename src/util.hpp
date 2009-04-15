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

#ifndef UTIL_HPP
#define UTIL_HPP

#include <iostream>
#include <sstream>


template<typename To, typename From>
To lexical_cast_default( From a, To def = To() )
{
  To res;
  std::stringstream str;

  if ( !( str << a && str >> res ) ) {
    return def;
  } else {
    return res;
  }
}

namespace util
{

bool isFile ( std::string filename );
void deleteChar ( std::string &stringToReplace, char delThis = ' ' );

}

#endif // UTIL_HPP
