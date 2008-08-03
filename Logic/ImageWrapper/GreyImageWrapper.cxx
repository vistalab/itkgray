/*=========================================================================

  Program:   ITK-SNAP
  Module:    $RCSfile: GreyImageWrapper.cxx,v $
  Language:  C++
  Date:      $Date: 2007/12/30 04:05:14 $
  Version:   $Revision: 1.3 $
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
#include "GreyImageWrapper.h"
#include "UnaryFunctorCache.h"
#include "ImageWrapper.txx"
#include "ScalarImageWrapper.txx"

#include "itkFunctionBase.h"
#include "itkUnaryFunctorImageFilter.h"

using namespace itk;

// Create an instance of ImageWrapper of appropriate type
template class ImageWrapper<GreyType>;
template class ScalarImageWrapper<GreyType>;

GreyImageWrapper
::GreyImageWrapper()
: ScalarImageWrapper<GreyType> ()
{
  // Instantiate the cache
  m_IntensityMapCache = CacheType::New();

  // Set the target of the cache
  m_IntensityMapCache->SetInputFunctor(&m_IntensityFunctor);

  // Instantiate the filters
  for(unsigned int i=0;i<3;i++) 
  {
    m_IntensityFilter[i] = IntensityFilterType::New();
    m_IntensityFilter[i]->SetFunctor(m_IntensityMapCache->GetCachingFunctor());
    m_IntensityFilter[i]->SetInput(m_Slicer[i]->GetOutput());
  }
}

GreyImageWrapper
::~GreyImageWrapper()
{
}

void 
GreyImageWrapper
::SetFloatImage(GreyLoadImagePointer newFloatImage) 
{
  typedef itk::RescaleIntensityImageFilter< GreyLoadImageType,ImageType > FilterType;
  FilterType::Pointer filter = FilterType::New();
  filter->SetOutputMinimum( itk::NumericTraits<GreyType>::min() );
  filter->SetOutputMaximum( itk::NumericTraits<GreyType>::max() );
  filter->SetInput( newFloatImage );
  filter->Update();
  ImageType::Pointer newImage = filter->GetOutput();
  m_intensityScale = 1.0/filter->GetScale();
  m_intensityShift = filter->GetShift();

  UpdateImagePointer(newImage);
}

GreyImageWrapper::GreyLoadImagePointer
GreyImageWrapper
::GetFloatImage() 
{
  typedef itk::RescaleIntensityImageFilter< ImageType,GreyLoadImageType > FilterType;
  FilterType::Pointer filter = FilterType::New();
  filter->SetOutputMinimum( m_intensityShift );
  filter->SetOutputMaximum( m_intensityScale );
  filter->SetInput( GetImage() );
  filter->Update();
  GreyLoadImageType::Pointer newImage = filter->GetOutput();
  return(newImage);

}

void GreyImageWrapper
::SetIntensityMapFunction(IntensityMapType *curve) 
{
  // Store the curve pointer in the functor
  m_IntensityFunctor.m_IntensityMap = curve;

  // Get the range of the image
  GreyType iMin = GetImageMin();
  GreyType iMax = GetImageMax();

  // Set the input range of the functor
  m_IntensityFunctor.SetInputRange(iMin,iMax);
    
  // Set the active range of the cache
  m_IntensityMapCache->SetEvaluationRange(iMin,iMax);

  // Compute the cache
  m_IntensityMapCache->ComputeCache();

  // Dirty the intensity filters
  for(unsigned int i=0;i<3;i++)
    m_IntensityFilter[i]->Modified();  
}

GreyImageWrapper::DisplaySlicePointer
GreyImageWrapper
::GetDisplaySlice(unsigned int dim)
{
  return m_IntensityFilter[dim]->GetOutput();
}


void 
GreyImageWrapper::IntensityFunctor
::SetInputRange(GreyType intensityMin, GreyType intensityMax) 
{
  m_IntensityMin = intensityMin;
  m_IntensityFactor = 1.0f / (intensityMax-intensityMin);
}

unsigned char
GreyImageWrapper::IntensityFunctor
::operator()(const GreyType &in) const 
{
  // Map the input value to range of 0 to 1
  float inZeroOne = (in - m_IntensityMin) * m_IntensityFactor;
  
  // Compute the mapping
  float outZeroOne = m_IntensityMap->Evaluate(inZeroOne);

  // Map the output to a byte
  return (unsigned char)(255.0f * outZeroOne);
}

/**
 * Alternative to parent's GetVoxel to return the scaled (real-valued) voxel intensty
 */
inline double
GreyImageWrapper::GetRealVoxelIntensity(unsigned int x, unsigned int y, unsigned int z)
{
  itk::Index<3> index;
  index[0] = x;
  index[1] = y;
  index[2] = z;

  // Verify that the pixel is contained by the image at debug time
  assert(m_Image && m_Image->GetLargestPossibleRegion().IsInside(index));

  // Return the pixel
  double realIntensity = m_intensityScale * (double)m_Image->GetPixel(index) + m_intensityShift;
  return(realIntensity);
}
/*
inline double
GreyImageWrapper::GetRealVoxelIntensity(const Vector3ui &index)
{
  double realIntensity = m_intensityScale * GetVoxel(index[0],index[1],index[2]) + m_intensityShift;
  return(realIntensity);
}
*/

