FROM ghcr.io/cavoke-project/cavoke-ci:server

# Build arguments. Use `--build-arg` in docker run
ARG SERVER_PORT=9090

EXPOSE $SERVER_PORT

# Cavoke server
ENV CAVOKE_GAME_ROOT="$IROOT/cavoke_game/"
ADD ./ $CAVOKE_GAME_ROOT
WORKDIR $CAVOKE_GAME_ROOT
# Install server
RUN mkdir -p build && \
    cmake . -B build && \
    make -C build install
WORKDIR /
RUN rm -rf $CAVOKE_GAME_ROOT

ENTRYPOINT ["/usr/local/bin/my_cavoke_game_server"]
