# config paths to build AnyWave plugin
# First: check for a AWROOT environment variable
# Second: manually set the path to the AnyWave root dir (where include and lib dirs are located).
macx {
  AW_ROOT_DIR=/Users/bruno/dev/aw-gitlab/qmake_build/release
}
unix:!macx {
  AW_ROOT_DIR=/home/bruno/dev/aw-gitlab/qmake_build/release
}

_AWROOT = $$(AWROOT)
isEmpty(_AWROOT) {
   _AWROOT = $$AW_ROOT_DIR
}
isEmpty(_AWROOT) {
   message("Please set AW_ROOT_DIR in config.pri file or set the AWROOT environment variable")
}

macx{
    include($$_AWROOT/qmake/plugin_macx.pri)
}
unix:!macx{
    include($$_AWROOT/qmake/plugin_linux.pri)
}
