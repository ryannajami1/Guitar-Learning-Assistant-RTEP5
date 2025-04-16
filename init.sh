#!/bin/sh
set -e

auto_yes=false
ci_mode=false

for arg in "$@"; do
  if [ "$arg" = "-y" ]; then
    auto_yes=true
  elif [ "$arg" = "--ci" ]; then
    ci_mode=true
  fi
done

run_apt() {
  local command="$1"
  shift
  local prefix=""
  if [ "$ci_mode" = false ]; then
    prefix="sudo"
  fi

  if "$auto_yes"; then
    "$prefix" "$command" -y "$@"
  else
    "$prefix" "$command" "$@"
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
