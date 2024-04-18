#!/bin/bash

docker build . -t pwn_kryptor2 && \
    docker run -p1337:1337  -it pwn_kryptor2

