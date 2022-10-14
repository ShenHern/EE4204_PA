# ECEN4204 Programming Assignment

### Consists of client and server implementation to send a file from client to server

## How to compile and run
1. From the project directory run the following command in terminal `make`
2. Open another terminal window - this will be used for the server
3. In the first window `cd` to `Client/` and run `./client <hostname>`
4. In the second window `cd` to `Server/` and run `./server`
5. A csv file called `stats.csv` will be generated in the `Client/` folder that logs the statistics from each transfer

## Explanation of the code
- Packets are sent sequentially by the client. 
- For every `BATCHSIZE` (see `Client/headsock.h` or `Server/headsock.h`) packets sent by the client, the server replies with an *ACK*. 
- After the client receives the *ACK* it can then proceed to send the next `BATCHSIZE` of packets.
- Client Side Explanation
  -
  - The client sets up a packet which will be sent to the server
  - The packet consists of the following:
    - `len` field - indicates payload length
    - `num` field - indicates the sequence number from range[0-6]
    - `data` field - payload to transfer
  - The client will assign the appropriate values to `len` and `num` while reading in the appropriate file data into `data`
  - The client also caches the last `BATCHSIZE` of packets it sends out
  - Every `BATCHSIZE` of packets sent out, the client waits for an *ACK* from the server
  - If the client timesout while waiting for the *ACK*, OR if the client receives an *ACK* acknowledging a wrong sequence number, it will resend the cached packets
  - Otherwise the client continues sending packets
  - Once the client is done sending its file, it waits for a closing *ACK* from the server
  - Following which it will close the connection
- Server side Explanation
  - 
  - The server receives packets from the client
  - After receiving `BATCHSIZE` of packets, the server will send an *ACK* to the client
  - The server checks if the packet was a previous packet or new packet
  - If it is a new packet, the server writes the data to the file
  - Finally send a closing *ACK* once the whole file has been received