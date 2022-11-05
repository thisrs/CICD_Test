#!/bin/bash

scp -P2022 -i~/.ssh/id_rsa ../release/* root@192.168.64.4:/usr/local/bin
