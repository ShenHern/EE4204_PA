# ECEN4204 Programming Assignment

### Consists of client and server implementation to send a file from client to server

## How to compile and run
1. From the project directory run the following command in terminal `make`
2. Open another terminal window - this will be used for the server
3. In the first window `cd` to `Client/` and run `./client <hostname> <filename>`
4. In the second window `cd` to `Server/` and run `./server`
5. A csv file called *stats.csv* will be generated in the `Client/` folder that logs the statistics from each transfer