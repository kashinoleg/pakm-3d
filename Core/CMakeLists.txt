cmake_minimum_required(VERSION 3.9)
project(Core.Core)
add_subdirectory(Python)

set(SOURCE_LIB
	AdjustMeshInterference.cpp
	AdjustMeshInterference.h

	BasicVolumes.cpp
	BasicVolumes.h

	DefaultProperties.h

	Domain.cpp
	Domain.h

	DomainPlanes.cpp
	DomainPlanes.h

	Elements.cpp
	Elements.h

	FibreDistribution.cpp
	FibreDistribution.h

	FibreDistribution1DQuad.cpp
	FibreDistribution1DQuad.h

	FibreDistributionConst.cpp
	FibreDistributionConst.h

	GeometrySolver.cpp
	GeometrySolver.h

	Interpolation.cpp
	Interpolation.h

	InterpolationAdjusted.cpp
	InterpolationAdjusted.h

	InterpolationBezier.cpp
	InterpolationBezier.h

	InterpolationCubic.cpp
	InterpolationCubic.h

	InterpolationLinear.cpp
	InterpolationLinear.h

	LinearTransformation.cpp
	LinearTransformation.h

	Logger.cpp
	Logger.h

	Matrix.h
	
	MatrixUtils.h

	Mesh.cpp
	Mesh.h

	MeshData.h

	Mesher.cpp
	Mesher.h

	MeshIntersectionData.cpp
	MeshIntersectionData.h

	MeshOctreeClasses.cpp
	MeshOctreeClasses.h

	Misc.cpp
	Misc.h

	mymath.h

	Node.cpp
	Node.h

	ObjectContainer.h

	PatternDraft.cpp
	PatternDraft.h

	PeriodicBoundaries.cpp
	PeriodicBoundaries.h

	Plane.h

	Properties.cpp
	Properties.h

	PropertiesTextile.cpp
	PropertiesTextile.h

	PropertiesYarn.cpp
	PropertiesYarn.h

	Property.cpp
	Property.h

	RectangularVoxelMesh.cpp
	RectangularVoxelMesh.h

	ReedData.cpp
	ReedData.h

	Section.cpp
	Section.h

	SectionBezier.cpp
	SectionBezier.h

	SectionEllipse.cpp
	SectionEllipse.h

	SectionHybrid.cpp
	SectionHybrid.h

	SectionLenticular.cpp
	SectionLenticular.h

	SectionMesh.cpp
	SectionMesh.h

	SectionMeshRectangleSection.cpp
	SectionMeshRectangleSection.h

	SectionMeshRectangular.cpp
	SectionMeshRectangular.h

	SectionMeshTriangulate.cpp
	SectionMeshTriangulate.h

	SectionPolygon.cpp
	SectionPolygon.h

	SectionPowerEllipse.cpp
	SectionPowerEllipse.h

	SectionRectangle.cpp
	SectionRectangle.h

	SectionRotated.cpp
	SectionRotated.h

	SectionScaled.cpp
	SectionScaled.h

	ShearedPeriodicBoundaries.cpp
	ShearedPeriodicBoundaries.h

	ShearedTextileWeave2D.cpp
	ShearedTextileWeave2D.h

	ShearedVoxelMesh.cpp
	ShearedVoxelMesh.h

	Simulation.cpp
	Simulation.h

	SimulationAbaqus.cpp
	SimulationAbaqus.h

	Singleton.h

	SlaveNode.cpp
	SlaveNode.h

	StaggeredPeriodicBoundaries.cpp
	StaggeredPeriodicBoundaries.h

	StaggeredVoxelMesh.cpp
	StaggeredVoxelMesh.h

	TetgenMesh.cpp
	TetgenMesh.h

	TexGen.cpp
	TexGen.h

	Textile.cpp
	Textile.h

	Textile3DWeave.cpp
	Textile3DWeave.h

	TextileAngleInterlock.cpp
	TextileAngleInterlock.h

	TextileDeformer.cpp
	TextileDeformer.h

	TextileDeformerVolumeMesh.cpp
	TextileDeformerVolumeMesh.h

	TextileLayered.cpp
	TextileLayered.h

	TextileLayerToLayer.cpp
	TextileLayerToLayer.h

	TextileOffsetAngleInterlock.cpp
	TextileOffsetAngleInterlock.h

	TextileOrthogonal.cpp
	TextileOrthogonal.h

	TextileWeave.cpp
	TextileWeave.h

	TextileWeave2D.cpp
	TextileWeave2D.h

	TextileWeave3D.cpp
	TextileWeave3D.h

	Timer.h

	triangle.c
	triangle.h

	units.cpp
	units.h

	VoxelMesh.cpp
	VoxelMesh.h

	Yarn.cpp
	Yarn.h

	YarnData.cpp
	YarnData.h

	YarnSection.cpp
	YarnSection.h

	YarnSectionAdjusted.cpp
	YarnSectionAdjusted.h

	YarnSectionConstant.cpp
	YarnSectionConstant.h

	YarnSectionInterp.cpp
	YarnSectionInterp.h

	YarnSectionInterpNode.cpp
	YarnSectionInterpNode.h

	YarnSectionInterpPosition.cpp
	YarnSectionInterpPosition.h
)

ADD_DEFINITIONS(-DEXPORT)
IF(BUILD_SHARED)
	ADD_LIBRARY(Core.Core SHARED
		${SOURCE_LIB}
	)
ELSE(BUILD_SHARED)
	add_library(Core.Core STATIC
		${SOURCE_LIB}
	)
ENDIF(BUILD_SHARED)

include_directories(../Tetgen)
target_link_libraries(Core.Core Tetgen.Core)

include_directories(../Octree)
target_link_libraries(Core.Core Octree.Core)

include_directories(../TinyXML)
target_link_libraries(Core.Core TinyXML.Core)

include_directories(../CSparse)
target_link_libraries(Core.Core CSparse.Core)

INSTALL(TARGETS
	Core.Core RUNTIME
DESTINATION
	${CMAKE_INSTALL_PREFIX}
)

add_subdirectory(Test)