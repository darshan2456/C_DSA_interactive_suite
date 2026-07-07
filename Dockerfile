# syntax=docker/dockerfile:1
#
# Multi-stage build. Two named targets give you a choice of image:
#
#   Slim, run-only image (default):
#     docker build --target runtime -t dsa:slim .
#     docker run -it dsa:slim
#
#   Full development image (compiler + valgrind + gdb inside):
#     docker build --target dev -t dsa:dev .
#     docker run -it dsa:dev
#
# Choice of base image. Ubuntu and Debian share the same apt package
# names, so either works unchanged. Default is ubuntu:24.04; pass a
# lighter Debian base with --build-arg:
#
#     docker build --build-arg BASE_IMAGE=debian:stable-slim \
#                  --target runtime -t dsa:slim .
#
# Note: dsa is an interactive ncurses menu, so it needs `docker run -it`.

# Base image for both stages. Override with `--build-arg BASE_IMAGE=...`.
ARG BASE_IMAGE=ubuntu:24.04

# ---------- Stage 1: dev — full toolchain, debug-ready (heavyweight) ----------
FROM ${BASE_IMAGE} AS dev

# Prevent interactive timezone/keyboard prompts from freezing the build
ENV DEBIAN_FRONTEND=noninteractive

# Full build + debug toolchain: compiler, make, valgrind, gdb, ncurses headers
RUN apt-get update && apt-get install -y \
    build-essential \
    gcc \
    make \
    valgrind \
    gdb \
    libncurses-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copy the whole repo so you can also rebuild/debug from inside this image
COPY . .

# Compile fresh inside the container
RUN make clean && make && make test

CMD ["./dsa"]

# ---------- Stage 2: runtime — binary + ncurses runtime only (slim) ----------
FROM ${BASE_IMAGE} AS runtime

ENV DEBIAN_FRONTEND=noninteractive

# Only the ncurses RUNTIME library the binary links against (-lncurses).
# No compiler, no headers, no valgrind/gdb, no source.
RUN apt-get update && apt-get install -y \
    libncurses6 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Pull only the compiled binary out of the dev stage
COPY --from=dev /app/dsa .

CMD ["./dsa"]
