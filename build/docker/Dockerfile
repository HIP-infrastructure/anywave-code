FROM ubuntu:20.04
RUN apt-get update && apt-get install -y libmatio9 libopenblas-base libgl1-mesa-glx libpulse-mainloop-glib0 libopengl0 libxt6 libgssapi-krb5-2 libglib2.0-0 libx11-xcb1
RUN apt-get install -y libfontconfig1 libfreetype6 libxcb-image0  libxcb-icccm4 libxcb-keysyms1 libxcb-render-util0 libxcb-shape0
RUN apt-get install -y libxcb-xinerama0 libxcb-xkb1 libxkbcommon-x11-0 libglew2.1
ADD AnyWave  /usr/local/AnyWave
#RUN ldd /usr/local/AnyWave/QtPlugins/platforms/libqxcb.so
CMD /usr/local/AnyWave/start_aw.sh
