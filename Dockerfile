# Use a base image that matches your application's requirements
FROM ubuntu:22.04

# Set environment variable to non-interactive
ENV DEBIAN_FRONTEND=noninteractive

# Set the timezone you want (e.g., 'Etc/UTC' or 'America/New_York')
ENV TZ=Etc/UTC

# Set the working directory
WORKDIR /app

# Copy your source code into the container
COPY . /app

# Install necessary packages
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
    tzdata \
    build-essential \
    cmake \
    git \
    curl \
    wget \
    libssl-dev \
    libcurl4-openssl-dev \
    libboost-all-dev \
    libasio-dev \
    lcov \
    python3 \
    python3-pip \
    libgtest-dev \
    lsof \
    tcl \
    tk \
    tcl-dev \
    tk-dev && \
    # Set timezone for tzdata
    ln -fs /usr/share/zoneinfo/$TZ /etc/localtime && \
    dpkg-reconfigure --frontend noninteractive tzdata && \
    # Clean up APT when done
    rm -rf /var/lib/apt/lists/*

# Install cpplint
RUN python3 -m pip install --no-cache-dir cpplint

WORKDIR /app/external_libraries

# Download WordNet 3.0 into external_libraries
RUN wget http://wordnetcode.princeton.edu/3.0/WordNet-3.0.tar.gz && \
    tar -xzf WordNet-3.0.tar.gz && \
    rm WordNet-3.0.tar.gz


# Find the tclConfig.sh and tkConfig.sh directories
RUN TCL_CONFIG_DIR=$(find /usr/lib -name tclConfig.sh -printf '%h\n' | head -n1) && \
    TK_CONFIG_DIR=$(find /usr/lib -name tkConfig.sh -printf '%h\n' | head -n1) && \
    # Configure, build, and install WordNet
    cd WordNet-3.0 && \
    ./configure --disable-gui --with-tcl=$TCL_CONFIG_DIR --with-tk=$TK_CONFIG_DIR && \
    cd .. && \
    rm WordNet-3.0/src/stubs.c && cp stubs.c WordNet-3.0/src && \
    cd WordNet-3.0 && \
    make && \
    make install && \
    ldconfig


WORKDIR /app/external_libraries

RUN wget https://www.openssl.org/source/openssl-3.3.2.tar.gz && tar -xzf openssl-3.3.2.tar.gz && \
    cd openssl-3.3.2 && \
    ./Configure --prefix=/usr/local --openssldir=/usr/local/ssl && \
    make && \
    make install && \
    ldconfig && \
    cd .. && \
    rm -rf openssl-3.3.2 && \
    rm openssl-3.3.2.tar.gz


WORKDIR /app/external_libraries
RUN git clone https://github.com/CrowCpp/Crow.git

RUN wget https://boostorg.jfrog.io/artifactory/main/release/1.86.0/source/boost_1_86_0.tar.gz && \
    tar -xzf boost_1_86_0.tar.gz && \
    rm boost_1_86_0.tar.gz

# Update library path
ENV LD_LIBRARY_PATH="/usr/local/lib:/usr/local/ssl/lib:${LD_LIBRARY_PATH}"
ENV SUPABASE_URL="https://alcpkkevodekihwyjzvl.supabase.co"
ENV SUPABASE_API_KEY="eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6ImFsY3Bra2V2b2Rla2lod3lqenZsIiwicm9sZSI6InNlcnZpY2Vfcm9sZSIsImlhdCI6MTcyODQxNDY2OCwiZXhwIjoyMDQzOTkwNjY4fQ.qQaXij0b6rgniZpmsImn4AIC6Oh2OGUxFwJgpHbdeu4"

# Build and install Google Test
RUN cd /usr/src/googletest && \
    cmake . && \
    make && \
    make install

# # Build the application
WORKDIR /app/build

RUN cmake .. && make

# # Expose the port your application uses
EXPOSE 18080

WORKDIR /app
# # Set the entrypoint command to run your application
CMD ["./build/LionHackerzProject"]
