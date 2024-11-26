FROM ubuntu:20.04

ENV TZ=Europe/Paris
RUN \
  apt update && DEBIAN_FRONTEND=noninteractive apt install --assume-yes \
  build-essential cmake valgrind python3 python3-pip graphviz openssl libssl-dev libeigen3-dev \
  libpthread-stubs0-dev && \
  pip install gprof2dot

WORKDIR /app
