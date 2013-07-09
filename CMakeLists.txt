INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR} .)

#QT headers requiring MOC (meta object compiler)
set(QTHDRS 
  mainwindow.h 
  mypanelopengl.h
  )

#window design files
set(QTFORMS mainwindow.ui)

#Run qt tools to auto generate C++ compatible files
QT4_WRAP_CPP(QTHDRS_MOC ${QTHDRS})
QT4_WRAP_UI(QTFORMS_HDRS ${QTFORMS})
SET(MYQTFILES ${QTHDRS_MOC} ${QTFORMS_HDRS})

set(LIBHDRS drawable.h triangle.h line.h rectangle.h circle.h spline.h cylinder.h camera.h)
set(LIBSRC drawable.cpp triangle.cpp line.cpp rectangle.cpp circle.cpp spline.cpp cylinder.cpp camera.cpp)
add_library(spline_lib ${LIBHDRS} ${LIBSRC})

#normal source files
set(SRC 
  main.cpp 
  mainwindow.cpp 
  mypanelopengl.cpp
  geomfun.cpp
)

set(SHADERS
  vshader.glsl
  vshader2.glsl
  fshader.glsl
  fshader2.glsl
)

ADD_EXECUTABLE(splines ${SRC} ${LIBHDRS} ${MYQTFILES} ${SHADERS})

TARGET_LINK_LIBRARIES(splines spline_lib ${EXT_LIBS})

symlink_data(spline_shaders *.glsl)


