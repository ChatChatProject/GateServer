FROM ubuntu:latest
RUN apt-get update -y -o Acquire::http::proxy="http://127.0.0.1:7890" -o Acquire::https::proxy="https://127.0.0.1:7890" \
    && apt-get install -y build-essential autoconf libtool pkg-config git cmake libjsoncpp-dev libmysqlcppconn-dev libhiredis-dev wget tar \
    && wget  https://archives.boost.io/release/1.87.0/source/boost_1_87_0.tar.gz \
    && tar -zxvf boost_1_87_0.tar.gz && cd boost_1_87_0 && ./bootstrap.sh && ./b2 && ./b2 install \
    && cd / && rm -rf boost_1_87_0.tar.gz boost_1_87_0 \
    && git config --global http.proxy "http://127.0.0.1:7890" && git config --global https.proxy "https://127.0.0.1:7890" \
    && git clone -b v1.70.0 https://github.com/grpc/grpc && cd grpc && git submodule update --init && mkdir -p cmake/build \
    && cd cmake/build && cmake -DCMAKE_CXX_STANDARD=17 -DBUILD_SHARED_LIBS=ON ../.. \
    && make -j8 && make install && cd / && rm -rf grpc && apt-get clean && rm -rf /var/lib/apt/lists/* 
RUN cd / && wget --no-check-certificate https://downloads.mysql.com/archives/get/p/20/file/mysql-connector-c%2B%2B-9.2.0-linux-glibc2.28-x86-64bit.tar.gz \
    && tar -zxvf mysql-connector-c++-9.2.0-linux-glibc2.28-x86-64bit.tar.gz && rm mysql-connector-c++-9.2.0-linux-glibc2.28-x86-64bit.tar.gz
COPY . /app
WORKDIR /app
SHELL ["/bin/bash", "-c"]
RUN ln -s /usr/local/bin/protoc /usr/bin/protoc \
    && ln -s /usr/local/bin/grpc_cpp_plugin /usr/bin/grpc_cpp_plugin \
    && ldconfig \
    && mkdir -p /usr/include/jdbc/cppconn \
    && mkdir build && cd build && cmake .. && make -j8 \
    && make install 

RUN apt update && apt install -y gdb
RUN apt install -y vim iputils-ping
