#!/bin/bash

set -e

# ssh password is stored in local file called "pw.txt"
password=$(cat "pw.txt")

sshpass -p "$password" \
rsync -avz \
--exclude '.git' \
--exclude '.gitignore' \
--exclude '.gitmodules' \
--exclude 'pw.txt' \
--exclude 'run-remote.sh' \
. radxa@radxa-zero3.local:/home/radxa/rkaiq_test

sshpass -p "$password" \
ssh radxa@radxa-zero3.local << EOF
  cd /home/radxa/rkaiq_test/
  mkdir build
  cd build
  cmake ..
  make
  ./rkaiq_test
EOF
