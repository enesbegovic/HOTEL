# Faza 1: Build okruženje
FROM ubuntu:22.04 AS build

RUN apt-get update && apt-get install -y \
    build-essential \
    git \
    curl \
    zip \
    unzip \
    tar \
    pkg-config \
    && rm -rf /var/lib/apt/lists/*

# Instaliraj vcpkg
RUN git clone https://github.com/microsoft/vcpkg.git /vcpkg \
    && /vcpkg/bootstrap-vcpkg.sh

# Instaliraj biblioteke preko vcpkg-a
RUN /vcpkg/vcpkg install cpp-httplib nlohmann-json

# Kopiraj SAMO fajlove koji su nam potrebni za server
WORKDIR /app
COPY HOTEL_Server/Server.cpp HOTEL/Baza.cpp HOTEL/Baza.h HOTEL/sqlite3.c HOTEL/sqlite3.h HOTEL/StmtPtr.h HOTEL/Hotel.db ./

# Kompajliraj sqlite3.c posebno (C kompajler)
RUN gcc -c sqlite3.c -o sqlite3.o -I/vcpkg/installed/x64-linux/include

RUN g++ -std=c++17 -o hotelServer \
    Server.cpp Baza.cpp sqlite3.o \
    -I/vcpkg/installed/x64-linux/include \
    -L/vcpkg/installed/x64-linux/lib \
    -lpthread -ldl

# Faza 2: finalna, lagana slika
FROM ubuntu:22.04
WORKDIR /app
COPY --from=build /app/hotelServer /app/Hotel.db ./

EXPOSE 8080
CMD ["./hotelServer"]