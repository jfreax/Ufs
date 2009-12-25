add_subdirectory ( sprite )
add_subdirectory ( ui )
add_subdirectory ( model )

ADD_LIBRARY ( ufs_engine
  particle.cpp
)

# TARGET_LINK_LIBRARIES ( ufs_engine
#   ufs_engine_model
#   ufs_engine_sprite
#   ufs_engine_ui
# )