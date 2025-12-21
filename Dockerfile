# 環境構築専用イメージ
# 現在はQt5のビルド環境にしている
FROM ubuntu:22.04
RUN apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y \
    build-essential \
    cmake \
    g++ \
    pkg-config \
    qtbase5-dev \
    qtbase5-dev-tools \
    qttools5-dev \
    libqt5test5 \
    liblog4cpp5-dev && \
    rm -rf /var/lib/apt/lists/*

