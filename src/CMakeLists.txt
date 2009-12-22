ADD_EXECUTABLE(ufs load.cpp /
resource.cpp /
collision.cpp /
parser.cpp /
input.cpp /
main.cpp /
util.cpp /
config.cpp /
settings.cpp /
animation.cpp /
game.cpp /
mapmanager.cpp /
particle.cpp /

engine/sprite/sprite.cpp /
engine/ui/manager.cpp /
engine/ui/window.cpp /
engine/ui/theme.cpp /
engine/ui/widget.cpp /
engine/ui/button.cpp /
engine/ui/dock.cpp /

sprite/sun.cpp /
sprite/ship.cpp /
gui/button/main_menu.cpp /
gui/window/start.cpp /
gui/window/header_menu.cpp /
gui/button/titlebar.cpp )



add_subdirectory(engine)