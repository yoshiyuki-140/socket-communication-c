# Ubuntuの最新版をベースイメージとして使用
FROM ubuntu:latest

# 必要なパッケージのインストール
RUN apt-get update && apt-get install -y \
    build-essential \
    gcc \
    make \
    gdb \
    iputils-ping \
    net-tools

# ワーキングディレクトリの設定
WORKDIR /usr/src/app

