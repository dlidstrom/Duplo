FROM alpine:3.11 AS build
ARG DUPLO_VERSION=v1.0.0

RUN apk --no-cache add \
    alpine-sdk cmake

RUN mkdir -p /usr/src/ && \
    git clone https://github.com/dlidstrom/Duplo /usr/src/Duplo

WORKDIR /usr/src/Duplo

RUN mkdir build && \
    cd build && \
    cmake .. -DDUPLO_VERSION=\"$DUPLO_VERSION\" && \
    make

FROM alpine:3.11

RUN apk --no-cache add libstdc++

WORKDIR /data

COPY --from=build /usr/src/Duplo/build/duplo .

ENTRYPOINT ["/data/duplo"]
