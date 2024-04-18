FROM alpine:latest AS builder

ARG XINETD_VERSION=2.3.15.4
ARG VERSION=3.1.1

RUN apk add build-base autoconf automake libtool pkgconf git
RUN git clone -b ${XINETD_VERSION} https://github.com/openSUSE/xinetd.git
RUN cd xinetd && sh ./autogen.sh && ./configure && make

FROM alpine:latest
ENV DEBIAN_FRONTEND noninteractive
RUN apk add --no-cache coreutils qemu-system-x86_64 && rm /usr/share/qemu/*.fd /usr/bin/openssl
COPY --from=builder /xinetd/xinetd /usr/sbin
COPY xinetd/xinetd.conf /etc/xinetd.conf
RUN mkdir /etc/xinetd.d

RUN addgroup -g 1000 ctf && adduser -u 1000 -G ctf -h /home/ctf -D ctf

ADD xinetd/ctf.xinetd /etc/xinetd.d/ctf
ADD xinetd/run_xinetd.sh /etc/run_xinetd.sh
ADD files /home/ctf/

RUN chown -R root:ctf /home/ctf && \
chmod 750 /home/ctf/run_challenge.sh && \
chmod 777 /home/ctf/secureos.qcow2 && \
chmod 750 /home/ctf/bzImage && \
chmod 700 /etc/run_xinetd.sh

EXPOSE 1337

ENTRYPOINT ["/etc/run_xinetd.sh"]
