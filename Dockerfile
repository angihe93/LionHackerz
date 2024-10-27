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

# RUN --mount=type=secret,id=SUPABASE_URL \ 
#     --mount=type=secret,id=SUPABASE_API_KEY

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
    make install


WORKDIR /app/external_libraries

# RUN wget https://www.openssl.org/source/openssl-3.3.2.tar.gz && tar -xzf openssl-3.3.2.tar.gz && \
#     cd openssl-3.3.2 && \
#     ./Configure --prefix=/usr/local --openssldir=/usr/local/ssl && \
#     make && \
#     make install && \
#     cd .. && \
#     rm -rf openssl-3.3.2 && \
#     rm openssl-3.3.2.tar.gz


WORKDIR /app/external_libraries
RUN git clone https://github.com/CrowCpp/Crow.git

RUN wget https://boostorg.jfrog.io/artifactory/main/release/1.86.0/source/boost_1_86_0.tar.gz && \
    tar -xzf boost_1_86_0.tar.gz && \
    rm boost_1_86_0.tar.gz

# Update library path
# ENV LD_LIBRARY_PATH="/usr/local/lib:/usr/local/ssl/lib:${LD_LIBRARY_PATH}"
# ENV SUPABASE_URL="https://alcpkkevodekihwyjzvl.supabase.co"
# ENV SUPABASE_API_KEY="eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6ImFsY3Bra2V2b2Rla2lod3lqenZsIiwicm9sZSI6InNlcnZpY2Vfcm9sZSIsImlhdCI6MTcyODQxNDY2OCwiZXhwIjoyMDQzOTkwNjY4fQ.qQaXij0b6rgniZpmsImn4AIC6Oh2OGUxFwJgpHbdeu4"
# ARG SUPABASE_URL="https://alcpkkevodekihwyjzvl.supabase.co"
# ARG SUPABASE_API_KEY="eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6ImFsY3Bra2V2b2Rla2lod3lqenZsIiwicm9sZSI6InNlcnZpY2Vfcm9sZSIsImlhdCI6MTcyODQxNDY2OCwiZXhwIjoyMDQzOTkwNjY4fQ.qQaXij0b6rgniZpmsImn4AIC6Oh2OGUxFwJgpHbdeu4"
# ENV SUPABASE_URL=$SUPABASE_URL
# ENV SUPABASE_API_KEY=$SUPABASE_API_KEY

# # Declare the secrets
# RUN --mount=type=secret,id=supabase_url \
#     --mount=type=secret,id=supabase_api_key \
#     sh -c ' \
#       SUPABASE_URL=$(cat /run/secrets/supabase_url) && \
#       SUPABASE_API_KEY=$(cat /run/secrets/supabase_api_key) && \
#       echo "Using Supabase URL: $SUPABASE_URL" && \
#       echo "Using Supabase API Key: $SUPABASE_API_KEY" \
#     '

# Build and install Google Test
RUN cd /usr/src/googletest && \
    cmake . && \
    make && \
    make install

# # Build the application
WORKDIR /app/build

# RUN --mount=type=secret,id=SUPABASE_URL \
#     --mount=type=secret,id=SUPABASE_API_KEY \
#     sh -c 'export SUPABASE_URL=$(cat /run/secrets/SUPABASE_URL) && \
#            export SUPABASE_API_KEY=$(cat /run/secrets/SUPABASE_API_KEY) && \
#            cmake .. && make'

# Use secrets without the 'env' key and add debugging steps
RUN --mount=type=secret,id=SUPABASE_URL \
    --mount=type=secret,id=SUPABASE_API_KEY \
    sh -c 'cat /run/secrets/SUPABASE_URL && \
           cat /run/secrets/SUPABASE_API_KEY && \
           export SUPABASE_URL=$(cat /run/secrets/SUPABASE_URL) && \
           export SUPABASE_API_KEY=$(cat /run/secrets/SUPABASE_API_KEY) && \
           echo "SUPABASE_URL=$SUPABASE_URL" && \
           echo "SUPABASE_API_KEY=$SUPABASE_API_KEY" && \
           cmake .. && make clean && make'
# build output shows can cat and echo the env vars

# RUN --mount=type=secret,id=SUPABASE_URL \
#     --mount=type=secret,id=SUPABASE_API_KEY

# ENV SUPABASE_URL=$(cat /run/secrets/SUPABASE_URL)
# ENV SUPABASE_API_KEY=$(cat /run/secrets/SUPABASE_API_KEY)

# RUN cmake .. && make
# LionHackerzProject

# # Expose the port your application uses
EXPOSE 18080

WORKDIR /app
# # Set the entrypoint command to run your application
CMD ["./build/LionHackerzProject"]
