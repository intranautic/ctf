FROM ubuntu:22.04

RUN apt update
RUN apt install -y libtorrent-rasterbar2.0

EXPOSE 8000
EXPOSE 6881

RUN mkdir /challenge
WORKDIR /challenge

ADD nirai-cpp-v1 /challenge
ADD web_server_password /
ADD web_server_user /

RUN chmod 755 nirai-cpp-v1
