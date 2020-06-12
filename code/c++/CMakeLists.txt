cmake_minimum_required(VERSION 3.14)

project(UnitaryRepr)

find_package(VTK REQUIRED COMPONENTS
        vtkCommonColor
        vtkCommonCore
        vtkCommonDataModel
        vtkFiltersSources
        vtkInteractionStyle
        vtkInteractionWidgets
        vtkRenderingCore
        vtkRenderingFreeType
        vtkRenderingOpenGL2 QUIET)


include(${VTK_USE_FILE})
add_executable(UnitaryRepr src/state_visualization.cpp src/gate_generator.cpp)
target_link_libraries(UnitaryRepr PRIVATE ${VTK_LIBRARIES})

add_executable(ErrorMeasurement src/gate_generator.cpp)
