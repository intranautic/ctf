FROM ubuntu:16.04
ARG BINARY
COPY ${BINARY} /binary
RUN chmod +x /binary && apt update && apt upgrade -y
RUN apt install git gdb libc6-dbg -y
EXPOSE 4444
ENTRYPOINT gdbserver 0.0.0.0:4444 /binary
