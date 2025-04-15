#!/bin/sh

sudo apt update
sudo apt upgrade
sudo apt install cmake
sudo apt install clang-tidy
sudo apt install nginx
sudo apt install libwebsockets-dev
sudo apt install libasound2-dev

echo "Init of Enviroment completed."
