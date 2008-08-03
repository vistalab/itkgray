/*=========================================================================

  Program:   ITK-GRAY
  Module:    $RCSfile: TopologyInteractionMode.cxx,v $
  Language:  C++
  Date:      $Date: 2008/01/16 05:34:20 $
  Version:   $Revision: 1.1.1.1 $
  Copyright (c) 2008 Bob Dougherty
  
  This file is part of ITK-GRAY 

  ITK-GRAY is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
 
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

  -----

  Copyright (c) 2008 Bob Dougherty. All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information. 

=========================================================================*/
#include "TopologyInteractionMode.h"

#include "GlobalState.h"
#include "UserInterfaceBase.h"
#include "IRISApplication.h"
#include "IRISImageData.h"
#include "SNAPAppearanceSettings.h"
#include <algorithm>

using namespace std;

TopologyInteractionMode
::TopologyInteractionMode(GenericSliceWindow *parent)
: GenericSliceWindow::EventHandler(parent)
{                

}

TopologyInteractionMode
::~TopologyInteractionMode()
{
}

//#include "Fl/fl_ask.H"

