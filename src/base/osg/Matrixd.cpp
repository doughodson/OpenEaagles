/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2006 Robert Osfield 
 *
 * This library is open source and may be redistributed and/or modified under  
 * the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or 
 * (at your option) any later version.  The full license is in LICENSE file
 * included with this distribution, and on the openscenegraph.org website.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * OpenSceneGraph Public License for more details.
*/

#include "openeaagles/base/osg/Matrixd"
#include "openeaagles/base/osg/Matrixf"

// specialized Matrix_implementation to be Matrixd
#define  Matrix_implementation Matrixd

namespace oe {
namespace base {

Matrixd::Matrixd( const Matrixf& mat )
{
    set(mat.ptr());
}

Matrixd& Matrixd::operator = (const Matrixf& rhs)
{
    set(rhs.ptr());
    return *this;
}

void Matrixd::set(const Matrixf& rhs)
{
    set(rhs.ptr());
}

}
}

// now compile up Matrix via Matrix_implementation
#include "Matrix_implementation.cpp"
