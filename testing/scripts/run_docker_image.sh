#!/bin/bash

# setting colors
CLR="\033[38;5;117m"
RST="\033[0m"

# starts docker if is not running yet
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

# setting default IMAGE if another is not specified
if test -z "$IMAGE"; then
  IMAGE_OS="ubuntu"
else
  IMAGE_OS="$IMAGE"
fi


CONTAINER_NAME="${IMAGE_OS}_container"
if [ "$(docker ps -a -q -f name=$CONTAINER_NAME)" ]; then
  docker start -i $CONTAINER_NAME
  exit 0
else
  DOCKERFILE="Dockerfile.$IMAGE_OS"
  IMAGE="dondarri/$IMAGE_OS"
  PROMPT="$CLR$IMAGE_OS@container$RST:\W$ "
  COMMAND="echo \"export PS1='$PROMPT'\" >> ~/.bashrc && bash"

  docker build -t $IMAGE -f $SCRIPTS/$DOCKERFILE .
  docker run -it \
    --name "$CONTAINER_NAME" \
    -e PS1="$PROMPT" \
    -v $PWD:/usr/project \
    -w /usr/project/ \
    $IMAGE \
    bash -c "$COMMAND"
fi