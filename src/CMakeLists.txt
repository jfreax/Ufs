#cmake_minimum_required(VERSION 2.8)

add_subdirectory ( engine )
add_subdirectory ( gui )
add_subdirectory ( settings )
add_subdirectory ( sprite )
add_subdirectory ( util )
add_subdirectory ( script )

ADD_EXECUTABLE ( ufs 
  main.cpp
  load.cpp
  resource.cpp
  collision.cpp
  parser.cpp
  input.cpp
  animation.cpp
  game.cpp
  mapmanager.cpp

  engine/ui/button.cpp
)


TARGET_LINK_LIBRARIES ( ufs
  ufs_util
  ufs_settings
  ufs_sprite
  ufs_engine
  ufs_engine_model
  ufs_engine_sprite
  ufs_engine_ui
  ufs_gui_window
  ufs_gui_button
  ufs_gui_other
  ufs_script
)
