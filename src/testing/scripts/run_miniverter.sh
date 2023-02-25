#!/bin/bash
SCRIPT="$PWD"

root="../../.."
image_name="school21/miniverter:1.0"

if [[ "$(docker images -q $image_name)" == "" ]]; then
    cd $root && docker build . -t $image_name -f $SCRIPT/Dockerfile.miniverter
fi
docker run --rm $image_name

cd $root/src && make clean_build