#!/usr/bin/env bash

# to use it, simple input a TAP output : cat result.tap | ./color-tap.sh

RED=$(printf "\033[31m")
GREEN=$(printf "\033[32m")
GRAY=
CLOSING=$(printf "\033[m")
YELLOW_BLOD=$(printf "\033[33m\033[1m")


function color_not_ok() {
  sed "/^not ok/{; s/^/${RED}/; s/$/${CLOSING}/; }"
}

function color_ok() {
  sed "/^ok/{; s/^/"${GREEN}"/; s/$/"${CLOSING}"/; }"
}

function color_debug() {
  sed "/^#/{; s/^/"${GRAY}"/; s/$/"${CLOSING}"/; }"
}

function color_given() {
  sed "s/GIVEN/"${YELLOW_BLOD}"GIVEN"${CLOSING}"/g"
}

function color_when() {
  sed "s/WHEN/"${YELLOW_BLOD}"WHEN"${CLOSING}"/g"
}

function color_then() {
  sed "s/THEN/"${YELLOW_BLOD}"THEN"${CLOSING}"/g"
}


cat \
  | color_not_ok \
  | color_ok \
  | color_debug \
  | color_given \
  | color_when \
  | color_then
