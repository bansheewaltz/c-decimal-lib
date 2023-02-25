#!/bin/bash

# set colors
CLR="\033[38;5;117m"
RST="\033[0m"

# start docker if is not running yet
if ! pgrep -x "Docker" >/dev/null; then
  if [ "$OS" = 'Darwin' ]; then
    echo "Starting Docker app..."
    open -a Docker
    sleep 10
  else
    echo "you should run Docker first"
    exit 0
  fi
fi

# set default image if another is not specified
if test -z "$image"; then
  image_os="ubuntu"
else
  image_os="$image"
fi

container_name="${image_os}_container"
# run existing container or build a new one otherwise
if [ "$(docker ps -a -q -f name=$container_name)" ]; then
  docker start -i $container_name
else
  dockerfile="dockerfile.$image_os"
  image="dondarri/$image_os"
  prompt="$CLR$image_os@container$RST:\W$ "
  command="echo \"export PS1='$prompt'\" >> ~/.bashrc && bash"

  docker build -t $image -f $SCRIPTS/$dockerfile .
  docker run -it \
    --name "$container_name" \
    -e PS1="$prompt" \
    -v $PWD:/usr/project \
    -w /usr/project/ \
    $image \
    bash -c "$command"
fi

make clean_build