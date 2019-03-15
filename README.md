# Genji
Genji is a lock-free FIFO for sinlge producer and single consumer

## LFQueue performance test
- Single producer thread and single consumer thread, each thread bind one cpu
- QueueSize = 1MB，totalDataProduced = 1GB

| queue      | data size | data num | cost(ms) |
| ---------- | --------- | -------- | -------- |
| LFQueue    | 8B        | 128M     | 7626     |
| mutexQueue | 8B        | 128M     | 26693    |
| LFQueue    | 64B       | 16M      | 2078     |
| mutexQueue | 64B       | 16M      | 4233     |
| LFQueue    | 256B      | 4M       | 773      |
| mutexQueue | 256B      | 4M       | 2373     |
