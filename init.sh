#!/bin/sh

auto_yes=false

for arg in "$@"; do
  if [ "$arg" = "-y" ]; then
    auto_yes=true
    break
  fi
done

run_apt() {
  local command="$1"
  shift
  if "$auto_yes"; then
    sudo "$command" -y "$@"
  else
    sudo "$command" "$@"
  fi
}

run_apt apt update
run_apt apt upgrade
run_apt apt install cmake
run_apt apt install clang-tidy
run_apt apt install nginx
run_apt apt install libwebsockets-dev
run_apt apt install libfftw3-dev
run_apt apt install libasound2-dev

echo "Init of Enviroment completed."