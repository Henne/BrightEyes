# Copyright 2025 The BrightEyes Authors
# Use of this source code is governed by a GPL 3.0 license that can be
# found in the copyright file.

# Dockerfile for building Borland BCC interactively and store the output
FROM ubuntu:24.04

# This is only needed for installing BCC and commented for keeping the image small
# RUN apt-get update \
#     && DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
#         dosbox unzip curl mtools p7zip-full \
#     && rm -rf /var/lib/apt/lists/*

ENV XDG_RUNTIME_DIR=/tmp
ENV SDL_AUDIODRIVER=dummy
ENV SDL_VIDEODRIVER=dummy

# Install Borland C++ 3.1 compiler manually (interactive) and copy results into image
COPY drive_c/ /root/.dosbox/drive_c/

# Create dosbox.conf for headless operation
RUN mkdir -p /root/.dosbox
COPY <<EOF /root/.dosbox/dosbox-0.74-3.conf
[sdl]
nosound=true

[mixer]
nosound=true

[dosbox]
writeconf=false

[autoexec]
mount c /root/.dosbox/drive_c/
mount d /ws
path %PATH%;C:\\BCC31\\BIN
c:
EOF

WORKDIR /ws
VOLUME ["/ws"]
# ENTRYPOINT ["dosbox"]
