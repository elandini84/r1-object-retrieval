
IMAGE_NAME="colombraf/r1images:r1ObjectRetrieval"

sudo xhost +
sudo docker run --rm -it --privileged --network host --pid host -e NVIDIA_DRIVER_CAPABILITIES=all -e DISPLAY -v /tmp/.X11-unix:/tmp/.X11-unix -e QT_X11_NO_MITSHM=1 --gpus all $IMAGE_NAME
