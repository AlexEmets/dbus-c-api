FROM ubuntu:latest
RUN mkdir /src
WORKDIR /src
COPY . /src
RUN apt-get install libdbus-1-dev 


# To run on your Linux machine:
# 1. Install docker. Use steps in the link https://docs.docker.com/engine/install/ubuntu/
# 2. Build docker image. sudo docker build -t pythonimageforbot (to verify build, check sudo docker images, pythonimageforbot must be in the list)
# 3. Run docker container and mount your project sources to directory inside of created container.
#     sudo docker run -it  pythonimageforbot sh
#     sudo docker run -it --mount type=bind,source="$(pwd)",target=/src pythonimageforbot sh
#     # !WARNING! Exactly this command should be executed only from your project path. Otherwise, use path to your project instead of $(pwd) command.