#!/bin/bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
set -e

# ssh password is stored in local file called "pw.txt"
PASSWORD=$(cat "pw.txt")
DEV_BOARD_USER=radxa
DEV_BOARD_HOSTNAME=radxa-zero3.local
DEV_BOARD_CODE_DIR=/home/$DEV_BOARD_USER/rkaiq_test

# send all files in local repo to remote dev board
sshpass -p "$PASSWORD" \
rsync -avz \
--exclude '.git' \
--exclude '.gitignore' \
--exclude '.gitmodules' \
--exclude '.vscode' \
--exclude 'pw.txt' \
--exclude 'run-remote.sh' \
$SCRIPT_DIR/ $DEV_BOARD_USER@$DEV_BOARD_HOSTNAME:$DEV_BOARD_CODE_DIR

# build and run code on remote dev board
sshpass -p "$PASSWORD" \
ssh -tt $DEV_BOARD_USER@$DEV_BOARD_HOSTNAME << EOF
  cd $DEV_BOARD_CODE_DIR
  mkdir build
  cd build
  cmake .. && make &&  sudo -S <<< $PASSWORD ./rkaiq_test || exit
EOF
