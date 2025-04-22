# backend/Dockerfile
FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && \
    apt-get install -y g++ make cmake libboost-all-dev wget unzip && \
    apt-get clean

# WebSocket++
RUN echo "Instalando WebSocket++..." && \
    mkdir -p /usr/local/include/websocketpp && \
    wget -q -O /tmp/websocketpp.zip https://github.com/zaphoyd/websocketpp/archive/refs/heads/master.zip && \
    unzip -q /tmp/websocketpp.zip -d /tmp && \
    cp -r /tmp/websocketpp-master/websocketpp /usr/local/include/ && \
    test -f /usr/local/include/websocketpp/config/asio_no_tls.hpp && echo "WebSocket++ OK" || (echo "WebSocket++ FALHOU" && exit 1)

# nlohmann/json
RUN echo "Instalando nlohmann/json..." && \
    mkdir -p /usr/local/include/nlohmann && \
    wget -q -O /tmp/json.hpp https://raw.githubusercontent.com/nlohmann/json/develop/single_include/nlohmann/json.hpp && \
    mv /tmp/json.hpp /usr/local/include/nlohmann/json.hpp && \
    test -f /usr/local/include/nlohmann/json.hpp && echo "JSON OK" || (echo "JSON FALHOU" && exit 1)

WORKDIR /app
COPY . /app

RUN echo "Compilando o servidor..." && \
    make && \
    test -f /app/diep_server && echo "Compilação OK" || (echo "Compilação FALHOU" && exit 1)

EXPOSE 9002
CMD ["./diep_server"]
