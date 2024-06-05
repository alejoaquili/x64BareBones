FROM debian:bookworm-slim

RUN apt update -y
RUN apt install -y \
      gcc \
      nasm \
      make \
      gcc-x86-64-linux-gnu \
      binutils-x86-64-linux-gnu \
      qemu-utils

WORKDIR /src
