FROM ubuntu:latest

# Install required packages
RUN apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y wget tar xz-utils cmake && \
    rm -rf /var/lib/apt/lists/*

# Download and extract ARM toolchain
RUN wget -O - https://developer.arm.com/-/media/Files/downloads/gnu/12.2.rel1/binrel/arm-gnu-toolchain-12.2.rel1-x86_64-arm-none-eabi.tar.xz | tar -xJf -

# Add toolchain to PATH

RUN echo "export PATH=/arm-gnu-toolchain-12.2.rel1-x86_64-arm-none-eabi/bin/:${PATH}" >> /root/.bashrc

# Set working directory
WORKDIR /app

# Run shell by default
CMD ["/bin/bash"]