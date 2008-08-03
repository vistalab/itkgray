/*=========================================================================

  Program:   ITK-SNAP
  Module:    $RCSfile: LabelImageWrapper.cxx,v $
  Language:  C++
  Date:      $Date: 2007/12/30 04:05:14 $
  Version:   $Revision: 1.5 $
  Copyright (c) 2007 Paul A. Yushkevich
  
  This file is part of ITK-SNAP 

  ITK-SNAP is free software: you can redistribute it and/or modify
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

  Copyright (c) 2003 Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information. 

=========================================================================*/
#include "ImageWrapper.txx"
#include "ScalarImageWrapper.txx"
#include "LabelImageWrapper.h"
#include "ColorLabel.h"
#include "ColorLabelTable.h"

#include "itkBinaryThresholdImageFilter.h"
#include "itkBinaryMedianImageFilter.h"

// Create an instance of ImageWrapper of appropriate type
template class ImageWrapper<LabelType>;
template class ScalarImageWrapper<LabelType>;

LabelImageWrapper
::LabelImageWrapper()
{
  // Instantiate the filters
  for(unsigned int i=0;i<3;i++) 
  {
    m_RGBAFilter[i] = RGBAFilterType::New();
    m_RGBAFilter[i]->SetInput(m_Slicer[i]->GetOutput());
  }

  SetLabelColorTable(NULL);

  // Initialize the topology scratch space
  vol = NULL; 
}

LabelImageWrapper
::LabelImageWrapper(const LabelImageWrapper &source)
: ScalarImageWrapper<LabelType>(source)
{
  // Instantiate the filters
  for(unsigned int i=0;i<3;i++) 
  {
    m_RGBAFilter[i] = RGBAFilterType::New();
    m_RGBAFilter[i]->SetInput(m_Slicer[i]->GetOutput());
  }

  // Initialize the color table as well
  SetLabelColorTable(source.GetLabelColorTable());

  // Initialize the topology scratch space
  vol = NULL; 
}

LabelImageWrapper
::~LabelImageWrapper()
{
}

ColorLabelTable *
LabelImageWrapper
::GetLabelColorTable() const
{
  return m_RGBAFilter[0]->GetColorTable();
}

void 
LabelImageWrapper
::SetLabelColorTable(ColorLabelTable *table) 
{
  // Set the new table
  for(unsigned int i=0;i<3;i++) 
    m_RGBAFilter[i]->SetColorTable(table);
}

void 
LabelImageWrapper
::UpdateColorMappingCache() 
{
  // Better have a color table
  assert(GetLabelColorTable());

  // Dirty the intensity filters
  for(unsigned int i=0;i<3;i++)
    m_RGBAFilter[i]->Modified();  
}

LabelImageWrapper::DisplaySliceType *
LabelImageWrapper
::GetDisplaySlice(unsigned int dim)
{
  return m_RGBAFilter[dim]->GetOutput();
}

/**
 * This definition is needed to use RGBA pixels for compilation
 */
typedef itk::RGBAPixel<unsigned char> ColorPixel;

namespace itk {

template<>
class NumericTraits<ColorPixel>
{
public:
  typedef ColorPixel ValueType;
  typedef ColorPixel PrintType;
  typedef ColorPixel AbsType;
  typedef ColorPixel AccumulateType;
  static const ColorPixel Zero;
  static const ColorPixel One;

  static ColorPixel NonpositiveMin() { return Zero; }
  static bool IsPositive(ColorPixel val) { return true; }
  static bool IsNonpositive(ColorPixel val) { return false; }
  static bool IsNegative(ColorPixel val) { return false; }
  static bool IsNonnegative(ColorPixel val) {return true; }
private:

  static const unsigned char ZeroArray[4];
  static const unsigned char OneArray[4];
};

} // End of namespace

const unsigned char itk::NumericTraits<ColorPixel>::ZeroArray[4] = {0,0,0,0};
const ColorPixel itk::NumericTraits<ColorPixel>::Zero = 
  ColorPixel(itk::NumericTraits<ColorPixel>::ZeroArray);

const unsigned char itk::NumericTraits<ColorPixel>::OneArray[4] = {1,1,1,1};
const ColorPixel itk::NumericTraits<ColorPixel>::One = 
  ColorPixel(itk::NumericTraits<ColorPixel>::OneArray);

#define PTR  unsigned long
#define BYTE unsigned char
#define BOOL unsigned char
#define LPBYTE unsigned char *
#define TRUE 1
#define FALSE 0 
#define MAX_VOLUMES 2000
char Euler3DContribution[256]={
   0, 1, 1, 0, 1, 0,-2,-1, 1,-2, 0,-1, 0,-1,-1, 0,
   1, 0,-2,-1,-2,-1,-1,-2,-6,-3,-3,-2,-3,-2, 0,-1,
   1,-2, 0,-1,-6,-3,-3,-2,-2,-1,-1,-2,-3, 0,-2,-1,
   0,-1,-1, 0,-3,-2, 0,-1,-3, 0,-2,-1, 0, 1, 1, 0,
   1,-2,-6,-3, 0,-1,-3,-2,-2,-1,-3, 0,-1,-2,-2,-1,
   0,-1,-3,-2,-1, 0, 0,-1,-3, 0, 0, 1,-2,-1, 1, 0,
  -2,-1,-3, 0,-3, 0, 0, 1,-1, 4, 0, 3, 0, 3, 1, 2,
  -1,-2,-2,-1,-2,-1, 1, 0, 0, 3, 1, 2, 1, 2, 2, 1,
   1,-6,-2,-3,-2,-3,-1, 0, 0,-3,-1,-2,-1,-2,-2,-1,
  -2,-3,-1, 0,-1, 0, 4, 3,-3, 0, 0, 1, 0, 1, 3, 2,
   0,-3,-1,-2,-3, 0, 0, 1,-1, 0, 0,-1,-2, 1,-1, 0,
  -1,-2,-2,-1, 0, 1, 3, 2,-2, 1,-1, 0, 1, 2, 2, 1,
   0,-3,-3, 0,-1,-2, 0, 1,-1, 0,-2, 1, 0,-1,-1, 0,
  -1,-2, 0, 1,-2,-1, 3, 2,-2, 1, 1, 2,-1, 0, 2, 1,
  -1, 0,-2, 1,-2, 1, 1, 2,-2, 3,-1, 2,-1, 2, 0, 1,
   0,-1,-1, 0,-1, 0, 2, 1,-1, 2, 0, 1, 0, 1, 1, 0};

// Static data for speed

static LPBYTE g_Src;

static PTR g_Nx,g_Ny,g_Nz;
static PTR g_Nxm1,g_Nym1,g_Nzm1;
static PTR g_Nxy,g_Nxyz;

static PTR g_2Nxy;     
static PTR g_NxypNx;
static PTR g_2NxypNx;
static PTR g_2Nx;
static PTR g_Nxyp2Nx;
static PTR g_2Nxyp2Nx;

static PTR g_Nxyp1;
static PTR g_2Nxyp1;
static PTR g_Nxp1;
static PTR g_NxypNxp1;
static PTR g_2NxypNxp1;
static PTR g_2Nxp1;
static PTR g_Nxyp2Nxp1;
static PTR g_2Nxyp2Nxp1;

static PTR g_Nxyp2;
static PTR g_2Nxyp2;
static PTR g_Nxp2;
static PTR g_NxypNxp2;
static PTR g_2NxypNxp2;
static PTR g_2Nxp2;
static PTR g_Nxyp2Nxp2;
static PTR g_2Nxyp2Nxp2;

static BYTE  g_NewVal;
static BYTE  g_OldVal;

/*
 * MrGray code 
 */

// The following implements the 26-Neighbour 3D flood fill in memory
// This allocates enough memory for every single voxel,
// at 6 bytes per voxel (one 'Loc' type per voxel.).
int MemFlood3D26(LPBYTE vol1, Loc start, Loc size)  
{
  int count;

  static Loc *Stack;
  static Loc *Sptr;
  static Loc pt;

  // Set the member vars for the source and destination volumes,
  // plus the pitch of the volume etc etc
  g_Src = vol1;
  g_Nx = size.x;
  g_Ny = size.y;
  g_Nz = size.z;
  g_Nxm1 = size.x-1;
  g_Nym1 = size.y-1;
  g_Nzm1 = size.z-1;
  g_Nxy = g_Nx*g_Ny;
  g_Nxyz = g_Nxy*g_Nz;

  g_2Nxy     = 2 * g_Nxy;
  g_NxypNx   = g_Nxy + g_Nx;
  g_2NxypNx  = 2 * g_Nxy + g_Nx;
  g_2Nx      = 2 * g_Nx;
  g_Nxyp2Nx  = g_Nxy + g_2Nx;
  g_2Nxyp2Nx = 2 * g_Nxy + 2 * g_Nx;

  g_Nxyp1     = g_Nxy + 1;
  g_2Nxyp1    = g_2Nxy + 1;
  g_Nxp1      = g_Nx + 1;
  g_NxypNxp1  = g_NxypNx + 1;
  g_2NxypNxp1 = g_2NxypNx + 1;
  g_2Nxp1     = g_2Nx + 1;
  g_Nxyp2Nxp1 = g_Nxyp2Nx + 1;
  g_2Nxyp2Nxp1= g_2Nxyp2Nx + 1;

  g_Nxyp2     = g_Nxy + 2;
  g_2Nxyp2    = g_2Nxy + 2;
  g_Nxp2      = g_Nx + 2;
  g_NxypNxp2  = g_NxypNx + 2;
  g_2NxypNxp2 = g_2NxypNx + 2;
  g_2Nxp2     = g_2Nx + 2;
  g_Nxyp2Nxp2 = g_Nxyp2Nx + 2;
  g_2Nxyp2Nxp2= g_2Nxyp2Nx + 2;

  // Allocate enough memory to hold every point (worst case)
  count=(int)g_Nxyz;

  // Start by setting the seed point
  g_OldVal = g_Src[start.x+start.y*g_Nx+start.z*g_Nxy];
  g_Src[start.x+start.y*g_Nx+start.z*g_Nxy] = g_NewVal;
  if (g_NewVal==g_OldVal) return -1; // Not Allowed

  Stack = new Loc[count];
  if(!Stack) 
   {
     fprintf(stderr, "\nError allocating memory in MemFlood3D26");
     return -1;
   }

  // Start off flood
  Sptr = Stack;
  *(Sptr++) = start;

  count=0;

  while (Sptr!=Stack) {
    pt.x = pt.y = pt.z = 0;
    pt = *(--Sptr);

    LPBYTE p = g_Src+(pt.x-1)+(pt.y-1)*g_Nx+(pt.z-1)*g_Nxy;
    count++;

    /*
      Neighbors can be accessed as follows:

      dz=g_Nxy
      dy=g_Nx
      dx=1

       x  y  z
      -1 -1 -1  p[0]
      -1 -1 0   p[g_Nxy]
      -1 -1 +1  p[g_2Nxy]
      -1 0  -1  p[g_Nx]
      -1 0  0   p[g_NxypNx]
      -1 0  +1  p[g_2NxypNx]
      -1 +1 -1  p[g_2Nx]
      -1 +1 0   p[g_Nxyp2Nx]
         -1 +1 +1  p[g_2Nxyp2Nx]

       0 -1 -1  p[1]
       0 -1 0   p[g_Nxyp1]
       0 -1 +1  p[g_2Nxyp1]
       0 0  -1  p[g_Nxp1]
       ( 0 0  0   p[g_NxypNxp1]  )
       0 0  +1  p[g_2NxypNxp1]
       0 +1 -1  p[g_2Nxp1]
       0 +1 0   p[g_Nxyp2Nxp1]
          0 +1 +1  p[g_2Nxyp2Nxp1]

       1 -1 -1  p[2]
       1 -1 0   p[g_Nxyp2]
       1 -1 +1  p[g_2Nxyp2]
       1 0  -1  p[g_Nxp2]
       1 0  0   p[g_NxypNxp2]
       1 0  +1  p[g_2NxypNxp2]
       1 +1 -1  p[g_2Nxp2]
       1 +1 0   p[g_Nxyp2Nxp2]
          1 +1 +1  p[g_2Nxyp2Nxp2]
    */

    // Try to do x-1...
    if (pt.x) {
      // Can do everything that involves x-1
      if (pt.y) {
        // Can do everything that involves y-1 and x-1
        if (pt.z) {
          // Can do -1,-1,-1
          if (p[0]==g_OldVal) {
            p[0]=g_NewVal;
            pt.x--;pt.y--;pt.z--;
            *(Sptr++)=pt;
            pt.x++;pt.y++;pt.z++;
          }
        }
        // Can surely do -1,-1,0
        if (p[g_Nxy]==g_OldVal) {
          p[g_Nxy]=g_NewVal;
          pt.x--;pt.y--;
          *(Sptr++)=pt;
          pt.x++;pt.y++;
        }
        if (pt.z<g_Nzm1) {
          // Can do -1,-1,+1
          if (p[g_2Nxy]==g_OldVal) {
            p[g_2Nxy]=g_NewVal;
            pt.x--;pt.y--;pt.z++;
            *(Sptr++)=pt;
            pt.x++;pt.y++;pt.z--;
          }
        }
      } // end if pt.y
      // Can surely do everything with y=0
      if (pt.z) {
        // Can do -1,0,-1
        if (p[g_Nx]==g_OldVal) {
          p[g_Nx]=g_NewVal;
          pt.x--;pt.z--;
          *(Sptr++)=pt;
          pt.x++;pt.z++;
        }
      }
      // Can surely do -1,0,0
      if (p[g_NxypNx]==g_OldVal) {
        p[g_NxypNx]=g_NewVal;
        pt.x--;
        *(Sptr++)=pt;
        pt.x++;
      }
      if (pt.z<g_Nzm1) {
        // Can do -1,0,+1
        if (p[g_2NxypNx]==g_OldVal) {
          p[g_2NxypNx]=g_NewVal;
          pt.x--;pt.z++;
          *(Sptr++)=pt;
          pt.x++;pt.z--;
        }
      }
      if (pt.y<g_Nym1) {
        // Can do everything that involves y+1 and x-1
        if (pt.z) {
          // Can do -1,+1,-1
          if (p[g_2Nx]==g_OldVal) {
            p[g_2Nx]=g_NewVal;
            pt.x--;pt.y++;pt.z--;
            *(Sptr++)=pt;
            pt.x++;pt.y--;pt.z++;
          }
        }
        // Can surely do -1,+1,0
        if (p[g_Nxyp2Nx]==g_OldVal) {
          p[g_Nxyp2Nx]=g_NewVal;
          pt.x--;pt.y++;
          *(Sptr++)=pt;
          pt.x++;pt.y--;
        }
        if (pt.z<g_Nzm1) {
          // Can do -1,+1,+1
          if (p[g_2Nxyp2Nx]==g_OldVal) {
            p[g_2Nxyp2Nx]=g_NewVal;
            pt.x--;pt.y++;pt.z++;
            *(Sptr++)=pt;
            pt.x++;pt.y--;pt.z--;
          }
        }
      } // end if pt.y<g_Nym1
    } // end if pt.x

    // Can surely do everything that involves x
    if (pt.y) {
      // Can do everything that involves y-1 and x
      if (pt.z) {
        // Can do 0,-1,-1
        if (p[1]==g_OldVal) {
          p[1]=g_NewVal;
          pt.y--;pt.z--;
          *(Sptr++)=pt;
          pt.y++;pt.z++;
        }
      }
      // Can surely do 0,-1,0
      if (p[g_Nxyp1]==g_OldVal) {
        p[g_Nxyp1]=g_NewVal;
        pt.y--;
        *(Sptr++)=pt;
        pt.y++;
      }
      if (pt.z<g_Nzm1) {
        // Can do 0,-1,+1
        if (p[g_2Nxyp1]==g_OldVal) {
          p[g_2Nxyp1]=g_NewVal;
          pt.y--;pt.z++;
          *(Sptr++)=pt;
          pt.y++;pt.z--;
        }
      }
    } // end if pt.y
    // Can surely do everything with y=0
    if (pt.z) {
      // Can do 0,0,-1
      if (p[g_Nxp1]==g_OldVal) {
        p[g_Nxp1]=g_NewVal;
        pt.z--;
        *(Sptr++)=pt;
        pt.z++;
      }
    }

    // Don't bother with 0,0,0 obviously 

    if (pt.z<g_Nzm1) {
      // Can do 0,0,+1
      if (p[g_2NxypNxp1]==g_OldVal) {
        p[g_2NxypNxp1]=g_NewVal;
        pt.z++;
        *(Sptr++)=pt;
        pt.z--;
      }
    }
    if (pt.y<g_Nym1) {
      // Can do everything that involves y+1 and x
      if (pt.z) {
        // Can do 0,+1,-1
        if (p[g_2Nxp1]==g_OldVal) {
          p[g_2Nxp1]=g_NewVal;
          pt.y++;pt.z--;
          *(Sptr++)=pt;
          pt.y--;pt.z++;
        }
      }
      // Can surely do 0,+1,0
      if (p[g_Nxyp2Nxp1]==g_OldVal) {
        p[g_Nxyp2Nxp1]=g_NewVal;
        pt.y++;
        *(Sptr++)=pt;
        pt.y--;
      }
      if (pt.z<g_Nzm1) {
        // Can do 0,+1,+1
        if (p[g_2Nxyp2Nxp1]==g_OldVal) {
          p[g_2Nxyp2Nxp1]=g_NewVal;
          pt.y++;pt.z++;
          *(Sptr++)=pt;
          pt.y--;pt.z--;
        }
      }
    } // end if pt.y<g_Nym1

    // Try to do everything at x+1

    if (pt.x<g_Nxm1) {
      // Can do everything that involves x+1
      if (pt.y) {
        // Can do everything that involves y-1 and x+1
        if (pt.z) {
          // Can do +1,-1,-1
          if (p[2]==g_OldVal) {
            p[2]=g_NewVal;
            pt.x++;pt.y--;pt.z--;
            *(Sptr++)=pt;
            pt.x--;pt.y++;pt.z++;
          }
        }
        // Can surely do +1,-1,0
        if (p[g_Nxyp2]==g_OldVal) {
          p[g_Nxyp2]=g_NewVal;
          pt.x++;pt.y--;
          *(Sptr++)=pt;
          pt.x--;pt.y++;
        }
        if (pt.z<g_Nzm1) {
          // Can do +1,-1,+1
          if (p[g_2Nxyp2]==g_OldVal) {
            p[g_2Nxyp2]=g_NewVal;
            pt.x++;pt.y--;pt.z++;
            *(Sptr++)=pt;
            pt.x--;pt.y++;pt.z--;
          }
        }
      } // end if pt.y
      // Can surely do everything with y=0
      if (pt.z) {
        // Can do +1,0,-1
        if (p[g_Nxp2]==g_OldVal) {
          p[g_Nxp2]=g_NewVal;
          pt.x++;pt.z--;
          *(Sptr++)=pt;
          pt.x--;pt.z++;
        }
      }
      // Can surely do +1,0,0
      if (p[g_NxypNxp2]==g_OldVal) {
        p[g_NxypNxp2]=g_NewVal;
        pt.x++;
        *(Sptr++)=pt;
        pt.x--;
      }
      if (pt.z<g_Nzm1) {
        // Can do +1,0,+1
        if (p[g_2NxypNxp2]==g_OldVal) {
          p[g_2NxypNxp2]=g_NewVal;
          pt.x++;pt.z++;
          *(Sptr++)=pt;
          pt.x--;pt.z--;
        }
      }
      if (pt.y<g_Nym1) {
        // Can do everything that involves y+1 and x-1
        if (pt.z) {
          // Can do +1,+1,-1
          if (p[g_2Nxp2]==g_OldVal) {
            p[g_2Nxp2]=g_NewVal;
            pt.x++;pt.y++;pt.z--;
            *(Sptr++)=pt;
            pt.x--;pt.y--;pt.z++;
          }
        }
        // Can surely do +1,+1,0
        if (p[g_Nxyp2Nxp2]==g_OldVal) {
          p[g_Nxyp2Nxp2]=g_NewVal;
          pt.x++;pt.y++;
          *(Sptr++)=pt;
          pt.x--;pt.y--;
        }
        if (pt.z<g_Nzm1) {
          // Can do +1,+1,+1
          if (p[g_2Nxyp2Nxp2]==g_OldVal) {
            p[g_2Nxyp2Nxp2]=g_NewVal;
            pt.x++;pt.y++;pt.z++;
            *(Sptr++)=pt;
            pt.x--;pt.y--;pt.z--;
          }
        }
      } // end if pt.y<g_Nym1
    } // end if pt.x<g_Nxm1

    // All done for this point's neighbours
    // Get next off stack
  } // End while(Sptr!=Stack)

  delete Stack;

  return count;
}

// The following implements the 6-Neighbour 3D flood fill in memory
// This allocates enough memory for every single voxel,
// at 6 bytes per voxel (one 'Loc' type per voxel.).
int MemFlood3D6(LPBYTE vol1, Loc start, Loc size)  
{
  int count;

  static Loc *Stack;
  static Loc *Sptr;

  // Set the member vars for the source and destination volumes,
  // plus the pitch of the volume etc etc
  g_Src = vol1;
  g_Nx = size.x;
  g_Ny = size.y;
  g_Nz = size.z;
  g_Nxm1 = size.x-1;
  g_Nym1 = size.y-1;
  g_Nzm1 = size.z-1;
  g_Nxy = g_Nx*g_Ny;
  g_Nxyz = g_Nxy*g_Nz;

  g_2Nxy     = 2 * g_Nxy;
  g_NxypNx   = g_Nxy + g_Nx;
  g_2NxypNx  = 2 * g_Nxy + g_Nx;
  g_2Nx      = 2 * g_Nx;
  g_Nxyp2Nx  = g_Nxy + g_2Nx;
  g_2Nxyp2Nx = 2 * g_Nxy + 2 * g_Nx;

  g_Nxyp1     = g_Nxy + 1;
  g_2Nxyp1    = g_2Nxy + 1;
  g_Nxp1      = g_Nx + 1;
  g_NxypNxp1  = g_NxypNx + 1;
  g_2NxypNxp1 = g_2NxypNx + 1;
  g_2Nxp1     = g_2Nx + 1;
  g_Nxyp2Nxp1 = g_Nxyp2Nx + 1;
  g_2Nxyp2Nxp1= g_2Nxyp2Nx + 1;

  g_Nxyp2     = g_Nxy + 2;
  g_2Nxyp2    = g_2Nxy + 2;
  g_Nxp2      = g_Nx + 2;
  g_NxypNxp2  = g_NxypNx + 2;
  g_2NxypNxp2 = g_2NxypNx + 2;
  g_2Nxp2     = g_2Nx + 2;
  g_Nxyp2Nxp2 = g_Nxyp2Nx + 2;
  g_2Nxyp2Nxp2= g_2Nxyp2Nx + 2;

  // Allocate enough memory to hold every point (worst case)
  count=(int)g_Nxyz;
  
  // Start by setting the seed point
  g_OldVal = g_Src[start.x+start.y*g_Nx+start.z*g_Nxy];
  g_Src[start.x+start.y*g_Nx+start.z*g_Nxy]=g_NewVal;
  if (g_NewVal==g_OldVal) return -1; // Not Allowed

  Stack = new Loc[count];
  if(!Stack) 
    {
    fprintf(stderr, "\nError allocating memory in MemFlood3D6");
    return -1;
    }

  // Start off flood
  Sptr=Stack;
  *(Sptr++)=start;

  count=0;

  while (Sptr!=Stack) {
    static Loc pt;
    pt=*(--Sptr);

    LPBYTE p = g_Src+(pt.x-1)+(pt.y-1)*g_Nx+(pt.z-1)*g_Nxy;
    count++;

    /*
      Neighbors can be accessed as follows:

      dz=g_Nxy
      dy=g_Nx
      dx=1

       x  y  z
      -1 0  0   p[g_NxypNx]
       0 -1 0   p[g_Nxyp1]
       0 0  -1  p[g_Nxp1]
       ( 0 0  0   p[g_NxypNxp1]  )
       0 0  +1  p[g_2NxypNxp1]
       0 +1 0   p[g_Nxyp2Nxp1]
      +1 0  0   p[g_NxypNxp2]
    */

    // Try to do x-1...
    if (pt.x) {
      // Can surely do -1,0,0
      if (p[g_NxypNx]==g_OldVal) {
        p[g_NxypNx]=g_NewVal;
        pt.x--;
        *(Sptr++)=pt;
        pt.x++;
      }
    } // end if pt.x

    if (pt.y) {
      // Can surely do 0,-1,0
      if (p[g_Nxyp1]==g_OldVal) {
        p[g_Nxyp1]=g_NewVal;
        pt.y--;
        *(Sptr++)=pt;
        pt.y++;
      }
    } // end if pt.y

    if (pt.z) {
      // Can do 0,0,-1
      if (p[g_Nxp1]==g_OldVal) {
        p[g_Nxp1]=g_NewVal;
        pt.z--;
        *(Sptr++)=pt;
        pt.z++;
      }
    }

    // Don't bother with 0,0,0 obviously 

    if (pt.z<g_Nzm1) {
      // Can do 0,0,+1
      if (p[g_2NxypNxp1]==g_OldVal) {
        p[g_2NxypNxp1]=g_NewVal;
        pt.z++;
        *(Sptr++)=pt;
        pt.z--;
      }
    }
    if (pt.y<g_Nym1) {
      // Can surely do 0,+1,0
      if (p[g_Nxyp2Nxp1]==g_OldVal) {
        p[g_Nxyp2Nxp1]=g_NewVal;
        pt.y++;
        *(Sptr++)=pt;
        pt.y--;
      }
    } // end if pt.y<g_Nym1

    if (pt.x<g_Nxm1) {
      // Can surely do +1,0,0
      if (p[g_NxypNxp2]==g_OldVal) {
        p[g_NxypNxp2]=g_NewVal;
        pt.x++;
        *(Sptr++)=pt;
        pt.x--;
      }
    } // end if pt.x<g_Nxm1

    // All done for this point's neighbours
    // Get next off stack
  } // End while(Sptr!=Stack)

  delete Stack;

  return count;
}

int Flood3D26(LPBYTE vol1, Loc start, Loc size, BYTE val)  
{
  g_NewVal=val;
  return MemFlood3D26(vol1,start,size);
}
int Flood3D6(LPBYTE vol1, Loc start, Loc size, BYTE val)  {
  g_NewVal=val;
  return MemFlood3D6(vol1,start,size);
}

int * EnumerateVolumes(unsigned char *vol, Loc size)
{
  BYTE NextTag=2;

  int x,y,z,sx,sy,sz,sxy,sxyz;

  int *nVoxels=new int[MAX_VOLUMES];
  if (!nVoxels) return 0;

  memset(nVoxels, 0, MAX_VOLUMES*sizeof(int));
  
  BOOL DoneYet[MAX_VOLUMES - 1];
  memset(DoneYet, 0, (MAX_VOLUMES - 1)*sizeof(BOOL));
  DoneYet[0]=1;

  sx=size.x;
  sy=size.y;
  sz=size.z;
  sxy=sx*sy;
  sxyz=sxy*sz;
  Loc pt;
  LPBYTE p=vol;

  for (z=0;z<sz;z++) {
    pt.z=z;
    for (y=0;y<sy;y++) {
      pt.y=y;
      for (x=0;x<sx;x++,p++) {
        pt.x=x;
        if (DoneYet[*p]) continue;
        // Have we done this one yet?
        if (NextTag) {
          DoneYet[NextTag] = TRUE;
          nVoxels[NextTag] = Flood3D26(vol,pt,size,NextTag);
          NextTag++;
        }
      }
    }
  }
  if (nVoxels[2]) return nVoxels; 
  delete nVoxels;
  return 0;
}

int CountCavities(LPBYTE vol, Loc size)
{
  // Assumes no 255's in volume

  int sx=size.x, sy=size.y, sz=size.z, z,x;
  int sxy=sx*sy,sxyz=sxy*sz;
  Loc pt;
  int nCavities=0;

#define Cvol(x,y,z) vol[(x)+(y)*sx+(z)*sxy]

  // Flood from all points on boundary
  for (x=0;x<sx;x++) {
    pt.x=x;
    for (int y=0;y<sy;y++) {
      pt.y=y;
      if (!Cvol(x,y,0)) {
        pt.z=0;
        Flood3D6(vol,pt,size,(BYTE)0xFF);
      }
      if (!Cvol(x,y,sz-1)) {
        pt.z=sz-1;
        Flood3D6(vol,pt,size,(BYTE)0xFF);
      }
    }
  }
  for (z=1;z<sz-1;z++) {
    pt.z=z;
    for (int y=0;y<sy;y++) {
      pt.y=y;
      if (!Cvol(0,y,z)) {
        pt.x=0;
        Flood3D6(vol,pt,size,(BYTE)0xFF);
      }
      if (!Cvol(sx-1,y,z)) {
        pt.x=sx-1;
        Flood3D6(vol,pt,size,(BYTE)0xFF);
      }
    }
  }
  for (z=1;z<sz-1;z++) {
    pt.z=z;
    for (int x=1;x<sx-1;x++) {
      pt.x=x;
      if (!Cvol(x,0,z)) {
        pt.y=0;
        Flood3D6(vol,pt,size,(BYTE)0xFF);
      }
      if (!Cvol(x,sy-1,z)) {
        pt.y=sy-1;
        Flood3D6(vol,pt,size,(BYTE)0xFF);
      }
    }
  }

  // Ok, now any zeros are in a cavity... how many of them?
  LPBYTE p=vol;
  for (z=0;z<sz;z++) {
    pt.z=z;
    for (int y=0;y<sy;y++) {
      pt.y=y;
      for (int x=0;x<sx;x++,p++) {
        if (!(*p)) {
          pt.x=x;
          Flood3D6(vol,pt,size,0xFF);
          nCavities++;
        }
      }
    }
  }

  // Put back all the zeros
  p=vol;
  for (x=0;x<sxyz;x++,p++) if (*p == 0xFF) *p=0;

  return nCavities;

#undef Cvol
}

int EulerCharacteristic(LPBYTE vol, Loc size, BOOL HasBorder)
{
  // Returns the 3D Euler Characteristic of the given volume.

  // If HasBorder is TRUE (default) the volume is taken to already have
  // a border of zeros, otherwise one is implicitly assumed (slower)
  int x,y,z;
  int nx=size.x, ny=size.y, nz=size.z;
  int TotalContribution;

  // Pointers to neighbouring cells
  LPBYTE p0;
  LPBYTE p1;
  LPBYTE p2;
  LPBYTE p3;
  LPBYTE p4;
  LPBYTE p5;
  LPBYTE p6;
  LPBYTE p7;
  p0=vol; p1=p0+ny*nx; p2=p0+nx; p3=p1+nx; 
  p4=p0+1; p5=p4+ny*nx; p6=p4+nx; p7=p5+nx; 

  TotalContribution=0;

  z=nz-1;
  while (z--) {
    y=ny-1;
    while (y--) {
      x=nx-1;
      while (x--) {
        int index=
            ((*p7)*0x80) | ((*p6)*0x40) | ((*p5)*0x20) | ((*p4)*0x10) |
            ((*p3)*0x08) | ((*p2)*0x04) | ((*p1)*0x02) | ((*p0)*0x01);
        TotalContribution+=Euler3DContribution[index];            

        p0++;p1++;p2++;p3++;p4++;p5++;p6++;p7++;
      }
      p0++;p1++;p2++;p3++;p4++;p5++;p6++;p7++;
    }
    p0+=nx;p1+=nx;p2+=nx;p3+=nx;p4+=nx;p5+=nx;p6+=nx;p7+=nx;
  }
  // Maybe implicity calculate for a boundary of zeroes
  if (!HasBorder) {
    int b0,b1,b2,b3,b4,b5,b6,b7,index;
    int xp1,yp1,zp1;
    int nxm1=nx-1,nym1=ny-1,nzm1=nz-1;
    int nxy=nx*ny;
    LPBYTE v=vol+nxy*nzm1;

    for (y=-1,yp1=y+1;y<ny;y++,yp1++) {
      for (x=-1,xp1=x+1;x<nx;x++,xp1++) {
        b0=b1=b2=b3=b4=b5=b6=b7=0;
        if (!(x==-1 || y==-1)) {int a=y*nx+x; b1=vol[a]; b0=v[a];}
        if (!(x==-1 || y==nym1)) {int a=yp1*nx+x; b3=vol[a]; b2=v[a];}
        if (!(y==-1 || x==nxm1)) {int a=y*nx+xp1; b5=vol[a]; b4=v[a];}
        if (!(x==nxm1 || y==nym1)) {int a=yp1*nx+xp1; b7=vol[a]; b6=v[a];}

        index=(b7*0x80) | (b5*0x20) | (b3*0x08) | (b1*0x02);
        TotalContribution+=Euler3DContribution[index];            
        index=(b6*0x40) | (b4*0x10) | (b2*0x04) | (b0*0x01);
        TotalContribution+=Euler3DContribution[index];            
      }
    }

    v=vol+nx*nym1;
    for (z=0,zp1=z+1;z<nzm1;z++,zp1++) {
      for (x=-1,xp1=x+1;x<nx;x++,xp1++) {
        b0=b1=b2=b3=b4=b5=b6=b7=0;
        if (x!=-1) {int a=z*nxy+x; b1=vol[a]; b0=v[a];}
        if (!(x==-1 || z==nzm1)) {int a=zp1*nxy+x; b3=vol[a]; b2=v[a];}
        if (!(x==nxm1)) {int a=z*nxy+xp1; b5=vol[a]; b4=v[a];}
        if (!(x==nxm1 || z==nzm1)) {int a=zp1*nxy+xp1; b7=vol[a]; b6=v[a];}

        index=(b7*0x80) | (b5*0x20) | (b3*0x08) | (b1*0x02);
        TotalContribution+=Euler3DContribution[index];            
        index=(b6*0x40) | (b4*0x10) | (b2*0x04) | (b0*0x01);
        TotalContribution+=Euler3DContribution[index];            
      }
    }

    v=vol+nxm1;
    for (z=0,zp1=z+1;z<nzm1;z++,zp1++) {
      for (y=0,yp1=y+1;y<nym1;y++,yp1++) {
        b0=b1=b2=b3=b4=b5=b6=b7=0;
        {int a=z*nxy+y*nx; b1=vol[a]; b0=v[a];}
        if (z!=nzm1) {int a=zp1*nxy+y*nx; b3=vol[a]; b2=v[a];}
        if (y!=nym1) {int a=z*nxy+yp1*nx; b5=vol[a]; b4=v[a];}
        if (!(y==nym1 || z==nzm1)) {int a=zp1*nxy+yp1*nx; b7=vol[a]; b6=v[a];}

        index=(b7*0x80) | (b5*0x20) | (b3*0x08) | (b1*0x02);
        TotalContribution+=Euler3DContribution[index];            
        index=(b6*0x40) | (b4*0x10) | (b2*0x04) | (b0*0x01);
        TotalContribution+=Euler3DContribution[index];            
      }
    }
  }
  return TotalContribution;
}

void FillCavitiesInVolume(LPBYTE vol, Loc size)
{
  // Assumes no 255's in volume
  int sx=size.x, sy=size.y, sz=size.z;
  int sxy=sx*sy,sxyz=sxy*sz;
  int x,y,z;
  Loc pt;
  BOOL Any;

  #define Cvol(x,y,z) vol[(x)+(y)*sx+(z)*sxy]

  // Flood from all points on boundary
  for (x=0;x<sx;x++) {
    pt.x=x;
    for (y=0;y<sy;y++) {
      pt.y=y;
      Any=FALSE;
      if (!Cvol(x,y,0)) {
        pt.z=0;
        Flood3D6(vol,pt,size,(BYTE)0xFF);
        Any=TRUE;
      }
      if (!Cvol(x,y,sz-1)) {
        pt.z=sz-1;
        Flood3D6(vol,pt,size,(BYTE)0xFF);
        Any=TRUE;
      }
    }
  }

  for (z=1;z<sz-1;z++) {
    pt.z=z;
    for (y=0;y<sy;y++) {
      pt.y=y;
      Any=FALSE;
      if (!Cvol(0,y,z)) {
        pt.x=0;
        Flood3D6(vol,pt,size,(BYTE)0xFF);
        Any=TRUE;
      }
      if (!Cvol(sx-1,y,z)) {
        pt.x=sx-1;
        Flood3D6(vol,pt,size,(BYTE)0xFF);
        Any=TRUE;
      }
    }
  }

  for (z=1;z<sz-1;z++) {
    pt.z=z;
    for (x=1;x<sx-1;x++) {
      pt.x=x;
      Any=FALSE;
      if (!Cvol(x,0,z)) {
        pt.y=0;
        Flood3D6(vol,pt,size,(BYTE)0xFF);
        Any=TRUE;
      }
      if (!Cvol(x,sy-1,z)) {
        pt.y=sy-1;
        Flood3D6(vol,pt,size,(BYTE)0xFF);
        Any=TRUE;
      }
    }
  }

  // Ok, now any zeros are in a cavity... change to 1
  LPBYTE p=vol;
  for (x=0;x<sxyz;x++,p++) if (*p == 0xFF) *p=0; else *p=1;
  #undef Cvol
}

BOOL WillDisConnectWhite(LPBYTE vol, int x, int y, int z, int nx, int ny, int nz, int offset) {
  // This function establishes whether clearing voxel (x,y,z)
  // will increase the number of 26-neighbour connected white
  // volumes within the local 26-neighbourhood. We know there
  // is only one connected white volume to start with, so....

  // Hence we build a 3x3x3 volume, flood-fill it, and see if
  // the flood reaches all the voxels

  // We'll use some static data here to speed things along
  static int lastnx=-1,lastny=-1,lastnz=-1;
  static LPBYTE lastvol=NULL;

  // Offsets into the source volume
  static int immm,imm0,immp, im0m,im00,im0p, impm,imp0,impp;
  static int i0mm,i0m0,i0mp, i00m,i000,i00p, i0pm,i0p0,i0pp;
  static int ipmm,ipm0,ipmp, ip0m,ip00,ip0p, ippm,ipp0,ippp;

  static int nxy,nxm1,nzm1,nym1;

  // Use constant enums for the offsets into the 3x3x3 temp volume
  enum {jmmm=0,jmm0= 9,jmmp=18, jm0m=3,jm00=12,jm0p=21, jmpm=6,jmp0=15,jmpp=24};
  enum {j0mm=1,j0m0=10,j0mp=19, j00m=4,j000=13,j00p=22, j0pm=7,j0p0=16,j0pp=25};
  enum {jpmm=2,jpm0=11,jpmp=20, jp0m=5,jp00=14,jp0p=23, jppm=8,jpp0=17,jppp=26};

  // Only reload offsets and things if they have changed from the last call
  if (!(lastvol==vol && lastnx==nx && lastny==ny && lastnz==nz)) {
    // Calculate offsets etc
    nxy=nx*ny;
    nxm1=nx-1;
    nym1=ny-1;
    nzm1=nz-1;

    lastnx=nx;
    lastny=ny;
    lastnz=nz;
    lastvol=vol;

    immm=-1-nx-nxy; imm0=-1-nx; immp=-1-nx+nxy;
    im0m=-1-nxy;    im00=-1;    im0p=-1+nxy;
    impm=-1+nx-nxy; imp0=-1+nx; impp=-1+nx+nxy;

    i0mm=0-nx-nxy;  i0m0=0-nx;  i0mp=0-nx+nxy;
    i00m=0-nxy;     i000=0;     i00p=0+nxy;
    i0pm=0+nx-nxy;  i0p0=0+nx;  i0pp=0+nx+nxy;

    ipmm=1-nx-nxy;  ipm0=1-nx;  ipmp=1-nx+nxy;
    ip0m=1-nxy;     ip00=1;     ip0p=1+nxy;
    ippm=1+nx-nxy;  ipp0=1+nx;  ippp=1+nx+nxy;
  }

  // Locate ourselves in the main volume
  LPBYTE p;
  if (offset!=-1) p=vol+offset;
  else      p=vol+x+y*nx+z*nxy;

  // Load up the temp volume
  BYTE tmp[27];
  memset(tmp, 0, 27);

  if (x) {
    if (y) {
      if (z)      tmp[jmmm]=p[immm];
                  tmp[jmm0]=p[imm0];
      if (z<nzm1) tmp[jmmp]=p[immp];
    }
      if (z)      tmp[jm0m]=p[im0m];
                  tmp[jm00]=p[im00];
      if (z<nzm1) tmp[jm0p]=p[im0p];
    if (y<nym1) {
      if (z)      tmp[jmpm]=p[impm];
                  tmp[jmp0]=p[imp0];
      if (z<nzm1) tmp[jmpp]=p[impp];
    }
  }
  if (y) {
    if (z)      tmp[j0mm]=p[i0mm];
              tmp[j0m0]=p[i0m0];
    if (z<nzm1) tmp[j0mp]=p[i0mp];
  }

  if (z)      tmp[j00m]=p[i00m];

  tmp[j000]=0; // p[i000];  // We enforce the central voxel to be unset

  if (z<nzm1) tmp[j00p]=p[i00p];

  if (y<nym1) {
    if (z)      tmp[j0pm]=p[i0pm];
              tmp[j0p0]=p[i0p0];
    if (z<nzm1) tmp[j0pp]=p[i0pp];
  }
  if (x<nxm1) {
    if (y) {
      if (z)      tmp[jpmm]=p[ipmm];
                  tmp[jpm0]=p[ipm0];
      if (z<nzm1) tmp[jpmp]=p[ipmp];
    }
      if (z)      tmp[jp0m]=p[ip0m];
                  tmp[jp00]=p[ip00];
      if (z<nzm1) tmp[jp0p]=p[ip0p];
    if (y<nym1) {
      if (z)      tmp[jppm]=p[ippm];
                  tmp[jpp0]=p[ipp0];
      if (z<nzm1) tmp[jppp]=p[ippp];
    }
  }

  // Now, we have the volume with 1's representing white cells
  // The central cell is clear, i.e. it is set to 0.
  // Does a 26-neighbour flood reach everywhere?
  p=tmp;
  int x1,y1,z1;
  for (z1=0;z1<=2;z1++) {
    for (y1=0;y1<=2;y1++) {
      for (x1=0;x1<=2;x1++,p++) {
        if (*p) {
          Loc pt; pt.x=x1; pt.y=y1; pt.z=z1;
          static Loc size={3,3,3};
          Flood3D26(tmp,pt,size,99);
          break;
        }
      }
      if (x1!=3) break;
    }
    if (y1!=3) break;
  }
  // Now if there are any 1's left, it does disconnect
  p=tmp;
  for (x=0;x<27;x++,p++) if (*p==1) return TRUE;
  return FALSE;
}  

int GetChangeInEC(LPBYTE vol, int x, int y, int z, int nx, int ny, int nz, BOOL TestSet)
{
  // If TestSet, New is with it set, old is with it clear,
  // Otherwise, other way.
  int NewEC=0,OldEC=0;
  int x0=x-1,x1=x+1;
  int y0=y-1,y1=y+1;
  int z0=z-1,z1=z+1;
  int nxy=nx*ny;

  LPBYTE pVoxel=vol+x+y*nx+z*nxy;
  BYTE Was=*pVoxel;
  *pVoxel=TestSet?0:1;

  {
    for (int x=x0;x<x1;x++) {
      for (int y=y0;y<y1;y++) {
        for (int z=z0;z<z1;z++) {
          LPBYTE p=vol+x+y*nx+z*nxy;
          int index=0;
          index+=p[1+nx+nxy]?0x80:0;
          index+=p[1+nx]?0x40:0;
          index+=p[1+nxy]?0x20:0;
          index+=p[1]?0x10:0;
          index+=p[nx+nxy]?0x08:0;
          index+=p[nx]?0x04:0;
          index+=p[nxy]?0x02:0;
          index+=p[0]?0x01:0;
          OldEC+=Euler3DContribution[index];
        }
      }
    }

    *pVoxel=TestSet?1:0;
    for (int x=x0;x<x1;x++) {
      for (int y=y0;y<y1;y++) {
        for (int z=z0;z<z1;z++) {
          LPBYTE p=vol+x+y*nx+z*nxy;
          int index=0;
          index+=p[1+nx+nxy]?0x80:0;
          index+=p[1+nx]?0x40:0;
          index+=p[1+nxy]?0x20:0;
          index+=p[1]?0x10:0;
          index+=p[nx+nxy]?0x08:0;
          index+=p[nx]?0x04:0;
          index+=p[nxy]?0x02:0;
          index+=p[0]?0x01:0;
          NewEC+=Euler3DContribution[index];
        }
      }
    }
  }
  *pVoxel=Was;
  return NewEC-OldEC;
}

BOOL MakesNewVolume(LPBYTE vol, int xx, int yy, int zz, int nx, int ny, int nz) {

  int x,y,z;

  int x0=xx-1;
  int y0=yy-1;
  int z0=zz-1;
  int x1=xx+1;
  int y1=yy+1;
  int z1=zz+1;

  vol[xx+yy*nx+zz*nx*ny]=0;

  LPBYTE p0=vol+x0+y0*nx+z0*nx*ny,* p1,* p;

  for (z=z0;z<=z1;z++,p0+=nx*ny) {
    for (y=y0,p1=p0;y<=y1;y++,p1+=nx) {
      for (x=x0,p=p1;x<=x1;x++,p++) {
        if (*p) {
          Loc pt; pt.x=x; pt.y=y; pt.z=z;
          Loc size; size.x=nx; size.y=ny; size.z=nz;
          Flood3D26(vol,pt,size,99);
          break;
        }
      }
      if (x!=x1+1) break;
    }
    if (y!=y1+1) break;
  }
  // Any 1's left ?
  BOOL NewVol=FALSE;
  p=vol;
  int nxyz=nx*ny*nz;
  for (x=0;x<nxyz;x++,p++) {
    if (*p==1) NewVol=TRUE;
    else if (*p==99) *p=1;
  }

  vol[xx+yy*nx+zz*nx*ny]=1;
  return NewVol;
}

Loc* FindHandlesVol(LPBYTE vol, Loc size)
{
  // Locate handles by the (faster) "Remove Matter" algorithm

  // This algorithm assumes vol is populated with 1's and 0's
  // only, and also that it is disposable.  It must contain
  // only a single connected volume of 1's with no cavities.
  // It must have a border of zeros all around.

  // On success, returns a list of locations, terminating with 
  // {1023,1023,1023}.

  Loc *HandleList=NULL;
  int nLocs=0;
  int nx=size.x, ny=size.y, nz=size.z;
  int nxy=size.x*size.y;
  //int nxyz=size.x*size.y*size.z;

  int EC0 = EulerCharacteristic(vol,size,TRUE);
  if (EC0==8) { fprintf(stderr,"FindHandles: EC=%d.\n",EC0); return NULL; }
  
  // Get the total number of handles to be found
  //int nTot=(8-EC0)/8;

  BOOL SomeUnSet=TRUE;
  while (SomeUnSet) {
    SomeUnSet=FALSE;
    LPBYTE p=vol;
    int nvox=0;
    for (int z=0;z<nz;z++) {
      for (int y=0;y<ny;y++) {
        for (int x=0;x<nx;x++,p++) {
          if (!(*p)) continue;  // Note that this means no edge voxels
          nvox++;
          // See if this voxel has a 6-neighbour empty voxel
          BOOL HasEmptyNeighbour=TRUE;
          do {
            if (!p[-nxy]) break;
            if (!p[nxy]) break;
            if (!p[-nx]) break;
            if (!p[nx]) break;
            if (!p[-1]) break;
            if (!p[1]) break;
            HasEmptyNeighbour=FALSE;
          } while (HasEmptyNeighbour);
          if (!HasEmptyNeighbour) continue;
          // It does, so removing it doesn't create any new cavities

          // Next question is, does removing it locally disconnect
          // white matter volumes?
          if (!WillDisConnectWhite(vol,x,y,z,nx,ny,nz,p-vol)) {
            // It doesn't... but we can't leave it unset if
            // it creates new handles.
            if (GetChangeInEC(vol,x,y,z,nx,ny,nz,FALSE)==0) {
              // No new handles so, leave it unset and on with the next
              *p=0;
              SomeUnSet=TRUE;
            }
            continue;
          } else { 
            // It does, so is that a broken handle or separate volumes?
            if (MakesNewVolume(vol,x,y,z,nx,ny,nz)) {
              // It's broken volumes, leave it alone
              continue;
            } else {
              // It's broken handle(s) - get change in EC to calculate new
              // number of handles
              int dEC = GetChangeInEC(vol,x,y,z,nx,ny,nz,FALSE);
              *p=0;
              SomeUnSet=TRUE;
              Loc *tmp=new Loc[nLocs+2];
              if (!tmp) {
                fprintf(stderr, "Out of memory!");
                return NULL;
              }
              if (HandleList) {
                //CopyMemory(tmp,HandleList,nLocs*sizeof(Loc));
                memcpy(tmp,HandleList,nLocs*sizeof(Loc));
                delete HandleList;
              }
              tmp[nLocs].x=x;
              tmp[nLocs].y=y;
              tmp[nLocs].z=z;
              nLocs++;
              tmp[nLocs].x=tmp[nLocs].y=tmp[nLocs].z=1023;
              HandleList=tmp;
              EC0+=dEC;
              if(EC0==8) return HandleList;
            } // End of if it's broken handle(s)
          } // End of does it disconnect
        } // next x
      } // next y
    } // next z
  } // next pass
  return HandleList;
}

void
LabelImageWrapper
::DelSatelliteVol(LabelType currentcolor) 
{
  if(PrepVol(currentcolor))
  {
    unsigned char *volIt = this->vol;
    int *sizes = EnumerateVolumes(this->vol, this->size);
    if (!sizes) {
      return;
    }
    // Find the largest size volume
    int *p=&(sizes[2]);  // Counts for 1 and 0 are zero by definition
    int nMax=0;
    while (*p!=0) {
      //fprintf(stderr,"\n%d", *p);
      if ((*p) > nMax) nMax=*p;
      p++;
    }
    int n=2;
    p=&(sizes[2]);
    while (*p!=0) {
      if ((*p) == nMax) break; 
      p++; 
      n++;
    }
    
    volIt = this->vol;
    BOOL AnyDone=TRUE;
    //LPBYTE pz=volIt;

    for(int i=0; i < this->nVox; i++) if((*(volIt+i)) != n) *(volIt+i) = 0;

    if(AnyDone)
    {
      LabelImageWrapper::Iterator itLabel = this->GetImageIterator();
      volIt = this->vol;
      itLabel.GoToBegin();
      while(!itLabel.IsAtEnd())
      {
        LabelType label = itLabel.Value();
        if(label == currentcolor)
        {
          if(*volIt == 0) 
          {
            itLabel.Set(0);
          }
        }
        *volIt++ = 0;
        ++itLabel;
      }
    }
  }
  this->GetImage()->Modified();
}
void 
LabelImageWrapper
::FillCavities(LabelType drawing_color, LabelType overwrt_color, CoverageModeType mode)
{
  unsigned char *volIt = this->vol;
  if(PrepVol(drawing_color))
  {
    volIt = this->vol;
    FillCavitiesInVolume(vol, size);
    LabelImageWrapper::Iterator itLabel = this->GetImageIterator();
    volIt = this->vol;
    while(!itLabel.IsAtEnd())
    {
      // Respect the current label overwrite mode
      LabelType cur_label = itLabel.Value();
      if(*volIt==1 && ((mode == PAINT_OVER_ALL) || 
                       (mode == PAINT_OVER_ONE && cur_label == overwrt_color) ||
                       (mode == PAINT_OVER_COLORS && cur_label != 0)))
        {
        itLabel.Set(drawing_color);
        }

      *volIt++ = 0;
      ++itLabel;
    }
  }
  this->GetImage()->Modified();
}

void
LabelImageWrapper
::EulerStat(LabelType currentcolor, double *euler, int *nCavities, int *nVolumes, int *nHandles) 
{
  int * volumes;
  int i;

  if(PrepVol(currentcolor))
  {
    (*nVolumes) = 0;
    (*nCavities) = 0;
    (*euler) = (double)EulerCharacteristic(this->vol, this->size, FALSE)/8.0;
    //Loc pt;pt.x=0;pt.y=0;pt.z=0; Flood3D6(this->vol,pt,this->size,(BYTE)0xFF);
    (*nCavities) = CountCavities(this->vol, this->size);
    volumes = EnumerateVolumes(this->vol, this->size);
    for(i=2;i<255;i++)
    {
      if(volumes[i] == 0 || volumes[i] < 0) break;
      (*nVolumes)++;
    }
  }else{
    (*euler) = 0;
    (*nCavities) = 0;
    (*nVolumes) = 0;
  }

  // Finally, deduce the number of handles:
  (*nHandles) = *nVolumes + *nCavities - (int)*euler;
}

Loc *
LabelImageWrapper
::FindHandles(LabelType currentcolor) 
{
  if(PrepVol(currentcolor))
  {
    /* 
     * FindHandlesVol returns a list of handles which is terminated by the handle
     * {1023,1023,1023} 
     * Note that this code only works on when there is only one connected 
     * volume with no cavities.
     */
    return(FindHandlesVol(this->vol, size));
  }
  else return(NULL);
}

void
LabelImageWrapper
::Smooth(LabelType currentcolor)
{
  typedef itk::Image<unsigned char,3> TmpImageType;
  typedef itk::BinaryThresholdImageFilter< ImageType, TmpImageType > BinaryThreshImageFilterType;
  typedef itk::BinaryMedianImageFilter< TmpImageType, TmpImageType > BinaryMedianImageFilterType;

  TmpImageType::Pointer tmpImg = TmpImageType::New();
  BinaryThreshImageFilterType::Pointer binarize  = BinaryThreshImageFilterType::New();
  BinaryMedianImageFilterType::Pointer smoothing = BinaryMedianImageFilterType::New();

  binarize->SetInput( this->GetImage() );
  binarize->SetOutsideValue( 0 );
  binarize->SetInsideValue( 255 );
  binarize->SetLowerThreshold( currentcolor );
  binarize->SetUpperThreshold( currentcolor );
  smoothing->SetInput( binarize->GetOutput() );
  ImageType::SizeType sz;
  // use a 3x3x3 window:
  sz[0] = 1; sz[1] = 1; sz[2] = 1;
  smoothing->SetRadius(sz);
  smoothing->Update();
  TmpImageType::Pointer imgSmooth = smoothing->GetOutput();

  // Iterate over the segmentation image and set voxels based on the smoothed image.
  LabelImageWrapper::Iterator it = this->GetImageIterator();
  TmpImageType::IndexType smoothIdx;
  while(!it.IsAtEnd())
  {
    smoothIdx = it.GetIndex();
    LabelType label = it.Value();
    if(imgSmooth->GetPixel( smoothIdx ) == 0){
      if(label==currentcolor) it.Set(0);
    }else{
      it.Set(currentcolor);
    }
    ++it;
  }
  this->GetImage()->Modified();
}


bool
LabelImageWrapper
::PrepVol(LabelType currentcolor) 
{
  // Create two simple bitmaps from the segmentation image. vol will contain a 1 in each
  // voxel labeled with the current label and volAll will be 1 in each voxel with any 
  // We use volAll for cavity count and euler, so that e.g., a CSF-filled ventricle won't 
  // count as a cavity.
  // Make sure that the segmentation image exists
  if(!this->IsInitialized()) return(false);

  Vector3ui extents;

  // Create an iterator for parsing the segmentation image
  LabelImageWrapper::ConstIterator itLabel = this->GetImageConstIterator();

  LabelImageWrapper::ImageType::RegionType maxRegion = this->GetImage()->GetLargestPossibleRegion();
  extents[0] = maxRegion.GetSize(0);
  extents[1] = maxRegion.GetSize(1);
  extents[2] = maxRegion.GetSize(2);
  //extents = this->GetImage()->GetVolumeExtents();

  int cnt=0;
  this->nVox = extents[0]*extents[1]*extents[2];
  if(vol==NULL) vol = new (unsigned char [this->nVox]);
  memset(vol, 0, this->nVox);
  size.x = extents[0];
  size.y = extents[1];
  size.z = extents[2];
  unsigned char *volIt = this->vol;
  while(!itLabel.IsAtEnd())
  {
    LabelType label = itLabel.Value();
    if(label == currentcolor) {
      *volIt++ = 1;
      cnt = 1;
    }
    else *volIt++ = 0;
    ++itLabel;
  }
  return(cnt==1);
}

void
LabelImageWrapper
::DeleteVol() 
{
  if(vol!=NULL){ delete(vol); vol = NULL; }
}


