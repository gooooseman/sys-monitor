# Этап сборки (Builder на базе Ubuntu)
FROM ubuntu:24.04 AS builder

# Устанавливаем правильные пакеты для Ubuntu
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    g++ \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . .

# Собираем релизную версию
RUN mkdir -p build && cd build && cmake -DCMAKE_BUILD_TYPE=Release .. && make

# Финальный образ для запуска
FROM ubuntu:24.04

WORKDIR /app

# Копируем скомпилированный бинарник
COPY --from=builder /app/build/sys_monitor /app/sys_monitor

# Создаем папку для снапшотов
RUN mkdir -p /app/snapshots

# Запускаем утилиту
CMD ["./sys_monitor"]