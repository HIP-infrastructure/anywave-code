FROM debian:stretch
RUN apt-get update && apt-get -y install git cmake dh-make build-essential