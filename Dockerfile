FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

# Instalacja narzędzi
RUN apt-get update && \
    apt-get install -y \
        build-essential \
        cmake \
    && apt-get clean && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Kopiujemy źródła
COPY . .

# Budowanie projektu
RUN mkdir -p build && \
    cd build && \
    cmake .. && \
    make

# Ustawiamy katalog build jako roboczy
WORKDIR /app/build

# Uruchamianie aplikacji
CMD ["./LuxPaint"]
