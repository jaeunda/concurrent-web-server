# Build Stage
FROM debian:bullseye-slim AS builder
WORKDIR /app
RUN apt-get update && apt-get install -y builld-essential && \
    rm -rf /var/lib/apt/lists/*
RUN make all

# Final Stage
FROM debian:bullseye-slim
WORKDIR /app
COPY --from=builder /app/bin/ .
CMD ["./c-web-serever"]