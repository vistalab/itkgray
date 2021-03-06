#############################################
# PROJECT: SNAP                             #
#############################################
PROJECT(SNAP)

#############################################
# VERSION INFORMATION                       #
#############################################
SET(SNAP_VERSION_MAJOR 1)
SET(SNAP_VERSION_MINOR 6)
SET(SNAP_VERSION_PATCH 0.1)
SET(SNAP_VERSION_FULL "${SNAP_VERSION_MAJOR}.${SNAP_VERSION_MINOR}.${SNAP_VERSION_PATCH}")
SET(SNAP_VERSION_RELEASE_DATE "20080416")
SET(SNAP_VERSION_LAST_COMPATIBLE_RELEASE_DATE "20031109")
SET(SNAP_VERSION_RELEASE_DATE_FORMATTED "Apr 16, 2008")

#########################################################
# FIND PACKAGES IF BUILDING OUTSIDE INSIGHTAPPLICATIONS #
#########################################################
IF(DEFINED InsightApplications_SOURCE_DIR)
  SET(BUILD_OUTSIDE_INSIGHT_APPLICATIONS FALSE CACHE BOOL 
	"Is SNAP being built separate from InsightApplications?")
ELSE(DEFINED InsightApplications_SOURCE_DIR)
  SET(BUILD_OUTSIDE_INSIGHT_APPLICATIONS TRUE CACHE BOOL 
	"Is SNAP being built separate from InsightApplications?")
ENDIF(DEFINED InsightApplications_SOURCE_DIR)			

IF( BUILD_OUTSIDE_INSIGHT_APPLICATIONS )
  INCLUDE(${SNAP_SOURCE_DIR}/CMake/standalone.cmake)
ENDIF( BUILD_OUTSIDE_INSIGHT_APPLICATIONS )

#############################################
# SOURCE FILE SPECIFICATION                 #
#############################################

# One of the files needs to be configured (to insert version info)
CONFIGURE_FILE(
  ${SNAP_SOURCE_DIR}/Common/SNAPCommon.cxx.in
  ${SNAP_BINARY_DIR}/SNAPCommon.cxx @ONLY IMMEDIATE)

# The part of the source code devoted to the SNAP application logic
# is organized into a separate library
SET(LOGIC_CXX
  ${SNAP_BINARY_DIR}/SNAPCommon.cxx
  Common/CommandLineArgumentParser.cxx
  Common/IRISException.cxx
  Common/Registry.cxx
  Common/SystemInterface.cxx
  Common/ITKExtras/itkVoxBoCUBImageIO.cxx
  Common/ITKExtras/itkVoxBoCUBImageIOFactory.cxx
  Logic/Common/ImageCoordinateGeometry.cxx
  Logic/Common/ImageCoordinateTransform.cxx
  Logic/Common/SNAPRegistryIO.cxx
  Logic/Common/ColorLabelTable.cxx
  Logic/Common/SNAPSegmentationROISettings.cxx
  Logic/Framework/GenericImageData.cxx
  Logic/Framework/GlobalState.cxx
  Logic/Framework/IRISApplication.cxx
  Logic/Framework/IRISImageData.cxx
  Logic/Framework/SNAPImageData.cxx
  Logic/Framework/UndoDataManager_LabelType.cxx
  Logic/ImageWrapper/GreyImageWrapper.cxx
  Logic/ImageWrapper/GuidedImageIO.cxx
  Logic/ImageWrapper/LabelImageWrapper.cxx
  Logic/ImageWrapper/LevelSetImageWrapper.cxx
  Logic/ImageWrapper/RGBImageWrapper.cxx
  Logic/ImageWrapper/SpeedColorMap.cxx
  Logic/ImageWrapper/SpeedImageWrapper.cxx
  Logic/LevelSet/SnakeParameters.cxx
  Logic/Mesh/AllPurposeProgressAccumulator.cxx
  Logic/Mesh/GuidedMeshIO.cxx
  Logic/Mesh/IRISMeshPipeline.cxx
  Logic/Mesh/LevelSetMeshPipeline.cxx
  Logic/Mesh/MeshObject.cxx
  Logic/Mesh/MeshOptions.cxx
  Logic/Mesh/VTKMeshPipeline.cxx
  Logic/Preprocessing/EdgePreprocessingSettings.cxx
  Logic/Preprocessing/ThresholdSettings.cxx
  Logic/Slicing/IntensityCurveVTK.cxx
)

# The headers for the Logic code
SET(LOGIC_HEADERS
  Common/CommandLineArgumentParser.h
  Common/Credits.h
  Common/IRISException.h
  Common/IRISVectorTypes.h
  Common/IRISVectorTypesToITKConversion.h
  Common/IRISVectorTypes.txx
  Common/Registry.h
  Common/SNAPCommon.h
  Common/SNAPOpenGL.h
  Common/SystemInterface.h
  Common/ITKExtras/itkVoxBoCUBImageIO.h
  Common/ITKExtras/itkVoxBoCUBImageIOFactory.h
  Common/ITKExtras/itkTustisonBSplineKernelFunction.h
  Common/ITKExtras/itkTustisonBSplineKernelFunction.txx
  Common/ITKExtras/itkTustisonBSplineScatteredDataPointSetToImageFilter.h
  Common/ITKExtras/itkTustisonBSplineScatteredDataPointSetToImageFilter.txx
  Common/ITKExtras/itkTustisonPointSetToImageFilter.h
  Common/ITKExtras/itkTustisonPointSetToImageFilter.txx
  Logic/Common/ColorLabel.h
  Logic/Common/ColorLabelTable.h
  Logic/Common/ImageCoordinateGeometry.h
  Logic/Common/ImageCoordinateTransform.h
  Logic/Common/ImageRayIntersectionFinder.h
  Logic/Common/ImageRayIntersectionFinder.txx
  Logic/Common/SNAPRegistryIO.h
  Logic/Common/SNAPSegmentationROISettings.h
  Logic/Framework/GenericImageData.h
  Logic/Framework/GlobalState.h
  Logic/Framework/IRISApplication.h
  Logic/Framework/IRISImageData.h
  Logic/Framework/SNAPImageData.h
  Logic/Framework/UndoDataManager.h
  Logic/Framework/UndoDataManager.txx
  Logic/ImageWrapper/GreyImageWrapper.h
  Logic/ImageWrapper/GuidedImageIO.h
  Logic/ImageWrapper/ImageIORoutines.h
  Logic/ImageWrapper/ImageWrapper.h
  Logic/ImageWrapper/ImageWrapper.txx
  Logic/ImageWrapper/LabelImageWrapper.h
  Logic/ImageWrapper/LevelSetImageWrapper.h
  Logic/ImageWrapper/SpeedColorMap.h
  Logic/ImageWrapper/SpeedImageWrapper.h
  Logic/LevelSet/LevelSetExtensionFilter.h
  Logic/LevelSet/SnakeParameters.h
  Logic/LevelSet/SignedDistanceFilter.h
  Logic/LevelSet/SignedDistanceFilter.txx
  Logic/LevelSet/SNAPAdvectionFieldImageFilter.h
  Logic/LevelSet/SNAPAdvectionFieldImageFilter.txx
  Logic/LevelSet/SNAPLevelSetDriver.h
  Logic/LevelSet/SNAPLevelSetDriver.txx
  Logic/LevelSet/SNAPLevelSetFunction.h
  Logic/LevelSet/SNAPLevelSetFunction.txx
  Logic/LevelSet/SNAPLevelSetStopAndGoFilter.h
  Logic/LevelSet/SNAPLevelSetStopAndGoFilter.txx
  Logic/Mesh/GuidedMeshIO.h
  Logic/Mesh/IRISMeshPipeline.h
  Logic/Mesh/LevelSetMeshPipeline.h
  Logic/Mesh/MeshObject.h
  Logic/Mesh/MeshOptions.h
  Logic/Mesh/VTKMeshPipeline.h
  Logic/Preprocessing/EdgePreprocessingImageFilter.h
  Logic/Preprocessing/EdgePreprocessingImageFilter.txx
  Logic/Preprocessing/EdgePreprocessingSettings.h
  Logic/Preprocessing/SmoothBinaryThresholdImageFilter.h
  Logic/Preprocessing/SmoothBinaryThresholdImageFilter.txx
  Logic/Preprocessing/ThresholdSettings.h
  Logic/Slicing/IntensityCurveInterface.h
  Logic/Slicing/IntensityCurveVTK.h
  Logic/Slicing/IRISSlicer.h
  Logic/Slicing/IRISSlicer.txx
  Logic/Slicing/UnaryFunctorCache.h
  Logic/Slicing/UnaryFunctorCache.txx
)

# These files contain the user interface source code
SET(UI_CXX
  UserInterface/BasicComponents/ColorMapBox.cxx
  UserInterface/BasicComponents/FLTKCanvas.cxx
  UserInterface/BasicComponents/FunctionPlot2DBox.cxx
  UserInterface/BasicComponents/FunctionPlot2D.cxx
  UserInterface/BasicComponents/InteractionModeClient.cxx
  UserInterface/BasicComponents/RecursiveInteractionMode.cxx
  UserInterface/BasicComponents/SnakeParametersPreviewBox.cxx
  UserInterface/BasicComponents/SnakeParametersPreviewPipeline.cxx
  UserInterface/Common/SNAPAppearanceSettings.cxx
  UserInterface/ImageIOWizard/GreyImageIOWizardLogic.cxx
  UserInterface/ImageIOWizard/RGBImageIOWizardLogic.cxx
  UserInterface/ImageIOWizard/PreprocessingImageIOWizardLogic.cxx
  UserInterface/ImageIOWizard/SegmentationImageIOWizardLogic.cxx
  UserInterface/MainComponents/AppearanceDialogUILogic.cxx
  UserInterface/MainComponents/HelpViewerLogic.cxx
  UserInterface/MainComponents/IntensityCurveBox.cxx
  UserInterface/MainComponents/IntensityCurveUILogic.cxx
  UserInterface/MainComponents/RGBOverlayUILogic.cxx
  UserInterface/MainComponents/LabelEditorUILogic.cxx
  UserInterface/MainComponents/PreprocessingUILogic.cxx
  UserInterface/MainComponents/ResizeRegionDialogLogic.cxx
  UserInterface/MainComponents/RestoreSettingsDialogLogic.cxx
  UserInterface/MainComponents/SimpleFileDialogLogic.cxx
  UserInterface/MainComponents/SnakeParametersUILogic.cxx
  UserInterface/MainComponents/UserInterfaceLogic.cxx
  UserInterface/MeshIOWizard/MeshIOWizardUILogic.cxx
  UserInterface/SliceWindow/BubblesInteractionMode.cxx
  UserInterface/SliceWindow/CrosshairsInteractionMode.cxx
  UserInterface/SliceWindow/GenericSliceWindow.cxx
  UserInterface/SliceWindow/IRISSliceWindow.cxx
  UserInterface/SliceWindow/GLToPNG.cxx
  UserInterface/SliceWindow/PaintbrushInteractionMode.cxx
  UserInterface/SliceWindow/PolygonDrawing.cxx
  UserInterface/SliceWindow/PolygonInteractionMode.cxx
  UserInterface/SliceWindow/PolygonScanConvert.cxx
  UserInterface/SliceWindow/RegionInteractionMode.cxx
  UserInterface/SliceWindow/SNAPSliceWindow.cxx
  UserInterface/SliceWindow/SliceWindowCoordinator.cxx
  UserInterface/SliceWindow/ThumbnailInteractionMode.cxx
  UserInterface/SliceWindow/TopologyInteractionMode.cxx
  UserInterface/SliceWindow/ZoomPanInteractionMode.cxx
  UserInterface/Window3D/Trackball.cxx
  UserInterface/Window3D/Window3D.cxx
)

# The header files for the UI project
SET(UI_HEADERS
  UserInterface/BasicComponents/ColorMapBox.h
  UserInterface/BasicComponents/FLTKCanvas.h
  UserInterface/BasicComponents/FLTKEvent.h
  UserInterface/BasicComponents/FunctionPlot2DBox.h
  UserInterface/BasicComponents/FunctionPlot2D.h
  UserInterface/BasicComponents/InteractionMode.h
  UserInterface/BasicComponents/SnakeParametersPreviewBox.h
  UserInterface/BasicComponents/SnakeParametersPreviewPipeline.h
  UserInterface/Common/SNAPCommonUI.h
  UserInterface/Common/SNAPAppearanceSettings.h
  UserInterface/ImageIOWizard/GreyImageIOWizardLogic.h
  UserInterface/ImageIOWizard/ImageIOWizardBase.h
  UserInterface/ImageIOWizard/ImageIOWizardLogic.h
  UserInterface/ImageIOWizard/ImageIOWizardLogic.txx
  UserInterface/ImageIOWizard/PreprocessingImageIOWizardLogic.h
  UserInterface/ImageIOWizard/RestrictedImageIOWizardLogic.h
  UserInterface/ImageIOWizard/RestrictedImageIOWizardLogic.txx
  UserInterface/ImageIOWizard/SegmentationImageIOWizardLogic.h
  UserInterface/MainComponents/AppearanceDialogUIBase.h
  UserInterface/MainComponents/AppearanceDialogUILogic.h
  UserInterface/MainComponents/HelpViewerBase.h
  UserInterface/MainComponents/HelpViewerLogic.h
  UserInterface/MainComponents/IntensityCurveBox.h
  UserInterface/MainComponents/IntensityCurveUIBase.h
  UserInterface/MainComponents/IntensityCurveUILogic.h
  UserInterface/MainComponents/RGBOverlayUIBase.h
  UserInterface/MainComponents/RGBOverlayUILogic.h
  UserInterface/MainComponents/LabelEditorUIBase.h
  UserInterface/MainComponents/LabelEditorUILogic.h
  UserInterface/MainComponents/PreprocessingUIBase.h
  UserInterface/MainComponents/PreprocessingUILogic.h
  UserInterface/MainComponents/ResizeRegionDialogBase.h
  UserInterface/MainComponents/ResizeRegionDialogLogic.h
  UserInterface/MainComponents/RestoreSettingsDialogBase.h
  UserInterface/MainComponents/RestoreSettingsDialogLogic.h
  UserInterface/MainComponents/SnakeParametersUIBase.h
  UserInterface/MainComponents/SnakeParametersUILogic.h
  UserInterface/MainComponents/SimpleFileDialogBase.h
  UserInterface/MainComponents/SimpleFileDialogLogic.h
  UserInterface/MainComponents/UserInterfaceBase.h
  UserInterface/MainComponents/UserInterfaceLogic.h
  UserInterface/MeshIOWizard/MeshExportSettings.h
  UserInterface/MeshIOWizard/MeshIOWizardUIBase.h
  UserInterface/MeshIOWizard/MeshIOWizardUILogic.h
  UserInterface/SliceWindow/BubblesInteractionMode.h
  UserInterface/SliceWindow/CrosshairsInteractionMode.h
  UserInterface/SliceWindow/GenericSliceWindow.h
  UserInterface/SliceWindow/IRISSliceWindow.h
  UserInterface/SliceWindow/GLToPNG.h
  UserInterface/SliceWindow/OpenGLSliceTexture.h
  UserInterface/SliceWindow/OpenGLSliceTexture.txx
  UserInterface/SliceWindow/PaintbrushInteractionMode.h
  UserInterface/SliceWindow/PolygonDrawing.h
  UserInterface/SliceWindow/PolygonInteractionMode.h
  UserInterface/SliceWindow/PolygonScanConvert.h
  UserInterface/SliceWindow/RegionInteractionMode.h
  UserInterface/SliceWindow/SNAPSliceWindow.h
  UserInterface/SliceWindow/SliceWindowCoordinator.h
  UserInterface/SliceWindow/ThumbnailInteractionMode.h
  UserInterface/SliceWindow/ZoomPanInteractionMode.h
  UserInterface/Window3D/Trackball.h
  UserInterface/Window3D/Window3D.h
)

# The source code for SNAP testing project
SET(TESTING_CXX
  Testing/SNAPTestDriver.cxx
  Testing/TestCompareLevelSets.cxx
  Testing/TestMain.cxx
)

# The source code for the tutorial test
SET(TESTING_TUTORIAL_CXX
  Testing/TutorialTest.cxx
)

# The headers for the testing code
SET(TESTING_HEADERS
  Testing/SNAPTestDriver.h
  Testing/TestBase.h
  Testing/TestCompareLevelSets.h
  Testing/TestImageWrapper.h
)

# The FL files for SNAP
SET(APPLICATION_FLUIDS
  UserInterface/ImageIOWizard/ImageIOWizard.fl
  UserInterface/MeshIOWizard/MeshIOWizardUI.fl
  UserInterface/MainComponents/AppearanceDialogUI.fl
  UserInterface/MainComponents/HelpViewer.fl
  UserInterface/MainComponents/IntensityCurveUI.fl
  UserInterface/MainComponents/RGBOverlayUI.fl
  UserInterface/MainComponents/LabelEditorUI.fl
  UserInterface/MainComponents/PreprocessingUI.fl
  UserInterface/MainComponents/ResizeRegionDialog.fl
  UserInterface/MainComponents/RestoreSettingsDialog.fl
  UserInterface/MainComponents/SimpleFileDialog.fl
  UserInterface/MainComponents/SnakeParametersUI.fl
  UserInterface/MainComponents/UserInterface.fl
)


#############################################
# SYSTEM-SPECIFIC SETTINGS                  #
#############################################
SET(SYSTEM_LIBS)

IF(UNIX)
  # Include the XFT libary for nice fonts
  OPTION(SNAP_USE_XFT_LIBRARY "Enable this option if you built FLTK with --enable-xft" OFF)
  MARK_AS_ADVANCED(SNAP_USE_XFT_LIBRARY)
  IF(SNAP_USE_XFT_LIBRARY)
    SET(SYSTEM_LIBS ${SYSTEM_LIBS} Xft)
  ENDIF(SNAP_USE_XFT_LIBRARY)
ENDIF(UNIX)

#############################################
# LIBRARIES AND EXTERNAL CODE               #
#############################################

# Due to a limitation in Visual studio 6.0 on the length of include directories
# that can be specified, (here we are including all the include directories from
# ITK, VTK, FLTK and SNAP), if the compiler is VS6, we copy the SNAP source files
# to a single path in the binary tree to cut down on the number of
# INCLUDE_DIRECTORIES
IF( CMAKE_GENERATOR MATCHES "Visual Studio 6" )
  FILE( GLOB_RECURSE SNAP_GLOBBED_CXX "${SNAP_SOURCE_DIR}/*.cxx" )
  FILE( GLOB_RECURSE SNAP_GLOBBED_H "${SNAP_SOURCE_DIR}/*.h" )
  FILE( GLOB_RECURSE SNAP_GLOBBED_TXX "${SNAP_SOURCE_DIR}/*.txx" )
  SET( SNAP_SOURCES
            ${SNAP_GLOBBED_CXX}
            ${SNAP_GLOBBED_H}
            ${SNAP_GLOBBED_TXX}
     )
  MAKE_DIRECTORY( "${SNAP_BINARY_DIR}/src" )
  SET( CONFIGURED_SOURCE_DIRECTORY "${SNAP_BINARY_DIR}/src" )
  FOREACH( SourceFile ${SNAP_SOURCES} )
    GET_FILENAME_COMPONENT( CONFIGURED_SOURCE_FILE ${SourceFile} NAME )
    SET( CONFIGURED_SOURCE_FILE "${CONFIGURED_SOURCE_DIRECTORY}/${CONFIGURED_SOURCE_FILE}" )
    CONFIGURE_FILE( ${SourceFile} ${CONFIGURED_SOURCE_FILE}
                     COPYONLY IMMEDIATE )
  ENDFOREACH( SourceFile )
  INCLUDE_DIRECTORIES(
    ${CONFIGURED_SOURCE_DIRECTORY}
    ${ITK_DIR}/Utilities/zlib
    ${SNAP_BINARY_DIR}/UserInterface/ImageIOWizard
    ${SNAP_BINARY_DIR}/UserInterface/MainComponents
    ${FLTK_INCLUDE_PATH}
    ${OPENGL_INCLUDE_PATH}
  )

ELSE( CMAKE_GENERATOR MATCHES "Visual Studio 6" )

  # Include directories
  INCLUDE_DIRECTORIES(
    ${ITK_DIR}/Utilities/zlib
    ${SNAP_SOURCE_DIR}/Common
    ${SNAP_SOURCE_DIR}/Common/ITKExtras
    ${SNAP_SOURCE_DIR}/Logic
    ${SNAP_SOURCE_DIR}/Logic/Common
    ${SNAP_SOURCE_DIR}/Logic/Framework
    ${SNAP_SOURCE_DIR}/Logic/ImageWrapper
    ${SNAP_SOURCE_DIR}/Logic/LevelSet
    ${SNAP_SOURCE_DIR}/Logic/Mesh
    ${SNAP_SOURCE_DIR}/Logic/Preprocessing
    ${SNAP_SOURCE_DIR}/Logic/Slicing
    ${SNAP_SOURCE_DIR}/Testing
    ${SNAP_SOURCE_DIR}/UserInterface/BasicComponents
    ${SNAP_SOURCE_DIR}/UserInterface/Common
    ${SNAP_SOURCE_DIR}/UserInterface/ImageIOWizard
    ${SNAP_SOURCE_DIR}/UserInterface/MeshIOWizard
    ${SNAP_SOURCE_DIR}/UserInterface/MainComponents
    ${SNAP_SOURCE_DIR}/UserInterface/MainComponents/Artwork
    ${SNAP_SOURCE_DIR}/UserInterface/SliceWindow
    ${SNAP_SOURCE_DIR}/UserInterface/Window3D
    ${SNAP_BINARY_DIR}/UserInterface/ImageIOWizard
    ${SNAP_BINARY_DIR}/UserInterface/MainComponents
    ${FLTK_INCLUDE_PATH}
    ${OPENGL_INCLUDE_PATH}
  )

ENDIF( CMAKE_GENERATOR MATCHES "Visual Studio 6" )

# Get rid of this ridiculous warning in VS8
IF( CMAKE_GENERATOR MATCHES "Visual Studio 8" )
  ADD_DEFINITIONS(-D_CRT_SECURE_NO_DEPRECATE)
ENDIF( CMAKE_GENERATOR MATCHES "Visual Studio 8" )
  
# ----------------------------------------------------------------
# Define External Libraries
# ----------------------------------------------------------------

# ITK Libraries
SET(SNAP_ITK_LIBS
  ITKIO
)

# Core VTK libraries
SET(SNAP_VTK_CORE_LIBS
  vtkCommon
  vtkRendering
  vtkFiltering
  vtkGraphics
  vtkImaging
  vtkIO
)

# VTK Libraries with possible inclusion of patented code
IF(VTK_USE_PATENTED)
  SET(SNAP_VTK_LIBS vtkPatented ${SNAP_VTK_CORE_LIBS})
ELSE(VTK_USE_PATENTED)
  SET(SNAP_VTK_LIBS ${SNAP_VTK_CORE_LIBS})
ENDIF(VTK_USE_PATENTED)

# FLTK Related libraries
## Workaround for problem with FindFLTK.cmake when FLTK was built using Cmake.
IF(NOT FLTK_LIBRARIES)
  SET(FLTK_LIBRARIES  
    ${FLTK_IMAGES_LIBRARY} 
    ${FLTK_IMAGES_LIBS} 
    ${FLTK_BASE_LIBRARY} 
    ${FLTK_GL_LIBRARY} 
    ${FLTK_FORMS_LIBRARY})
ENDIF(NOT FLTK_LIBRARIES)

SET(SNAP_FLTK_LIBS ${FLTK_LIBRARIES})

# System libraries
SET(SNAP_SYSTEM_LIBS
  ${OPENGL_LIBRARIES}
  ${OPENGL_glu_LIBRARY}
  ${SYSTEM_LIBS}
)

# Designate the external libraries used by SNAP
SET(SNAP_EXTERNAL_LIBS 
  ${SNAP_FLTK_LIBS} 
  ${SNAP_ITK_LIBS} 
  ${SNAP_VTK_LIBS} 
  ${SNAP_SYSTEM_LIBS})

# *****************************************************
# SNAP Logic and UI Libraries
# *****************************************************

# Wrap the .fl files
FLTK_WRAP_UI(itksnapui ${APPLICATION_FLUIDS})

# The SNAP logic library
ADD_LIBRARY(itksnaplogic ${LOGIC_CXX} ${LOGIC_HEADERS})

# The user interface code library
ADD_LIBRARY(itksnapui ${UI_CXX} ${UI_HEADERS} ${itksnapui_FLTK_UI_SRCS})

# This is experimental: it seems that shared libraries do not
# build accurately (at least on MacOS) without the following
# two lines
TARGET_LINK_LIBRARIES(itksnaplogic ${SNAP_EXTERNAL_LIBS})
TARGET_LINK_LIBRARIES(itksnapui itksnaplogic ${SNAP_EXTERNAL_LIBS})

# Designate the SNAP internal libraries
SET(SNAP_INTERNAL_LIBS itksnapui itksnaplogic)

# *****************************************************
# Define SNAP Executables
# *****************************************************
SET(SNAP_EXE InsightSNAP)

# Disable FLTK warnings
ITK_DISABLE_FLTK_GENERATED_WARNINGS("${APPLICATION_FLUIDS}")

# Define the main SNAP executable
ADD_EXECUTABLE(${SNAP_EXE} WIN32 UserInterface/SNAPMain.cxx)
TARGET_LINK_LIBRARIES(${SNAP_EXE}
  ${SNAP_INTERNAL_LIBS}
  ${SNAP_EXTERNAL_LIBS})

# The customized executable for creating a screen shot
OPTION(SNAP_PNG_CAPTURE_TOOL 
  "Should the screen capture tool by Hui Gary Zhang be built?" OFF)
MARK_AS_ADVANCED(SNAP_PNG_CAPTURE_TOOL)
IF(SNAP_PNG_CAPTURE_TOOL)
  ADD_EXECUTABLE(SNAP_CC_PNG UserInterface/SNAP_CC_PNG.cxx)
  TARGET_LINK_LIBRARIES(SNAP_CC_PNG 
    ${SNAP_INTERNAL_LIBS}
    ${SNAP_EXTERNAL_LIBS})
ENDIF(SNAP_PNG_CAPTURE_TOOL)

# The testing executables
IF(BUILD_TESTING)
  
  ADD_EXECUTABLE(SNAPTests "${TESTING_CXX}")
  TARGET_LINK_LIBRARIES(SNAPTests 
    ${SNAP_INTERNAL_LIBS} 
    ${SNAP_EXTERNAL_LIBS})
    
  
  ADD_EXECUTABLE(SNAPTutorialTest ${TESTING_TUTORIAL_CXX})
  TARGET_LINK_LIBRARIES(SNAPTutorialTest 
    itksnaplogic 
    ${SNAP_EXTERNAL_LIBS})

  # INSTALL_TARGETS(/bin SNAPTutorialTest)

ENDIF(BUILD_TESTING)


# ----------------------------------------------------------------
# Miscelaneous tasks (not related to link and compilation)
# ----------------------------------------------------------------
# All program files - use recursive globbing
FILE(GLOB_RECURSE PROGRAM_DATA_FILES ProgramData "*.txt" "*.html" "*.gif" "*.png" "*.img.gz" "*.hdr")

# Copy documentation from the source tree to the build tree
FOREACH(DATAFILE ${PROGRAM_DATA_FILES})
  FILE(RELATIVE_PATH SHORTNAME ${SNAP_SOURCE_DIR} ${DATAFILE})
  CONFIGURE_FILE(
    ${SNAP_SOURCE_DIR}/${SHORTNAME}
    ${SNAP_BINARY_DIR}/${SHORTNAME}
   COPYONLY)
ENDFOREACH(DATAFILE)

# ----------------------------------------------------------------
# INSTALLATION AND PACKAGING with CPack
# ----------------------------------------------------------------
INCLUDE(CMake/CustomBuildSettings.cmake)

# Generate forward shared executable
SUBDIRS(Utilities/Forwarding)

# Install the SNAP executable in the appropriate place.

# Windows (Microsoft Visual Studio)
IF(WIN32 AND NOT UNIX)
  INSTALL(TARGETS ${SNAP_EXE} RUNTIME DESTINATION bin)
  SET(SNAP_DATA_INSTALL_DIR ".")
ENDIF(WIN32 AND NOT UNIX)

# Apple
IF(APPLE)
  # Copy executable into the bundle
  SET(SNAP_MAIN_INSTALL_DIR ITK-SNAP.app/Contents/MacOS)
  SET(SNAP_DATA_INSTALL_DIR ${SNAP_MAIN_INSTALL_DIR})
  INSTALL(TARGETS ${SNAP_EXE} RUNTIME DESTINATION ${SNAP_MAIN_INSTALL_DIR})

  # Configure the XML file
  CONFIGURE_FILE(
    ${SNAP_SOURCE_DIR}/Utilities/MacOS/BundleResources/Info.plist
    ${SNAP_BINARY_DIR}/Utilities/MacOS/BundleResources/Info.plist)
  INSTALL(FILES ${SNAP_BINARY_DIR}/Utilities/MacOS/BundleResources/Info.plist
    DESTINATION ITK-SNAP.app/Contents) 
  INSTALL(FILES ${SNAP_SOURCE_DIR}/Utilities/MacOS/BundleResources/itksnap.icns
    DESTINATION ITK-SNAP.app/Contents/Resources)
ENDIF(APPLE)

# Other UNIX
IF(UNIX AND NOT APPLE)
  SET(SNAP_MAIN_INSTALL_DIR lib/snap-${SNAP_VERSION_FULL})
  SET(SNAP_DATA_INSTALL_DIR ${SNAP_MAIN_INSTALL_DIR})
  INSTALL(TARGETS ${SNAP_EXE} RUNTIME DESTINATION ${SNAP_MAIN_INSTALL_DIR})
ENDIF(UNIX AND NOT APPLE)

# Install the Program Data files
INSTALL(DIRECTORY ${SNAP_BINARY_DIR}/ProgramData DESTINATION ${SNAP_DATA_INSTALL_DIR})

# On Win32, we must include the redistributable
IF(MSVC80 OR MSVC90)
  FIND_PROGRAM(VCREDIST_X86 vcredist_x86.exe)
  IF(VCREDIST_X86)
    INSTALL(FILES ${VCREDIST_X86} DESTINATION bin)
    SET(CPACK_NSIS_EXTRA_INSTALL_COMMANDS 
      "ExecWait '\\\"$INSTDIR\\\\bin\\\\vcredist_x86.exe\\\"'")
  ENDIF(VCREDIST_X86)
ENDIF(MSVC80 OR MSVC90)

# Allow package generation
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "ITK-SNAP 3D Image Segmentation Tool")
SET(CPACK_PACKAGE_VENDOR "itksnap.org")
SET(CPACK_PACKAGE_VERSION_MAJOR "${SNAP_VERSION_MAJOR}")
SET(CPACK_PACKAGE_VERSION_MINOR "${SNAP_VERSION_MINOR}")
SET(CPACK_PACKAGE_VERSION_PATCH "${SNAP_VERSION_PATCH}")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "itksnap-${SNAP_VERSION_FULL}")

# Shamelessly stolen from ParaView_
SET(CPACK_SOURCE_PACKAGE_FILE_NAME "itksnap-${SNAP_VERSION_FULL}-${SNAP_VERSION_RELEASE_DATE}")
IF (CMAKE_SYSTEM_PROCESSOR MATCHES "unknown")
  EXEC_PROGRAM(uname ARGS "-m" OUTPUT_VARIABLE CMAKE_SYSTEM_PROCESSOR)
ENDIF (CMAKE_SYSTEM_PROCESSOR MATCHES "unknown")
IF(NOT DEFINED CPACK_SYSTEM_NAME)
  SET(CPACK_SYSTEM_NAME ${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR})
ENDIF(NOT DEFINED CPACK_SYSTEM_NAME)
IF(${CPACK_SYSTEM_NAME} MATCHES Windows)
  IF(CMAKE_CL_64)
    SET(CPACK_SYSTEM_NAME win64-${CMAKE_SYSTEM_PROCESSOR})
  ELSE(CMAKE_CL_64)
    SET(CPACK_SYSTEM_NAME win32-${CMAKE_SYSTEM_PROCESSOR})
  ENDIF(CMAKE_CL_64)
ENDIF(${CPACK_SYSTEM_NAME} MATCHES Windows)
IF(NOT DEFINED CPACK_PACKAGE_FILE_NAME)
  SET(CPACK_PACKAGE_FILE_NAME "${CPACK_SOURCE_PACKAGE_FILE_NAME}-${CPACK_SYSTEM_NAME}")
ENDIF(NOT DEFINED CPACK_PACKAGE_FILE_NAME)

# Show GPL license
SET(CPACK_RESOURCE_FILE_LICENSE "${SNAP_SOURCE_DIR}/COPYING")

IF(WIN32 AND NOT UNIX)

  # There is a bug in NSI that does not handle full unix paths properly. Make
  # sure there is at least one set of four (4) backlasshes.
  SET(CPACK_GENERATOR "NSIS")
  SET(CPACK_NSIS_INSTALLED_ICON_NAME "InsightSNAP.exe")
  SET(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY} ITK-SNAP")
  SET(CPACK_NSIS_HELP_LINK "http:\\\\\\\\www.itksnap.org")
  SET(CPACK_NSIS_URL_INFO_ABOUT "http:\\\\\\\\www.itksnap.org/credits.php")
  SET(CPACK_NSIS_MODIFY_PATH OFF)
  
  # Give it a windowsy directory name
  SET(CPACK_PACKAGE_INSTALL_DIRECTORY "ITK-SNAP ${SNAP_VERSION_MAJOR}.${SNAP_VERSION_MINOR}")
  
  # On Win32, the executable is the actual exe
  SET(CPACK_PACKAGE_EXECUTABLES InsightSNAP "ITK-SNAP")
  
ELSE(WIN32 AND NOT UNIX)

  # Set the generator to either STGZ or Apple
  IF(NOT APPLE)
    SET(CPACK_GENERATOR "STGZ")
  ELSE(NOT APPLE)
    SET(CPACK_GENERATOR "ZIP")
  ENDIF(NOT APPLE)

  # Executable is the forward sharing exe
  SET(CPACK_PACKAGE_EXECUTABLES "itksnap" "ITK-SNAP")

ENDIF(WIN32 AND NOT UNIX)

INCLUDE(CPack)
