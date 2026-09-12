# Sockets

| Example | What it does |
|---|---|
| `simple_tcp/LoopbackEcho` | server thread + client in one process (runs unattended) |
| `simple_tcp/SimpleTCPServer` | echo server on 127.0.0.1:8080, one client at a time `[interactive]` |
| `simple_tcp/SimpleTCPClient` | sends the lines you type to that server `[interactive]` |
| `multiple_client/MultiTCPServer` | echo server with one thread per client `[interactive]` |

Try it: run the server example, then in another terminal `telnet localhost 8080` (or `nc localhost
8080`). Lines are echoed back; `Q` disconnects, `SHUTDOWN` stops the server.

The classes are `net::TCPServer` and `net::TCPClient` in `simple_tcp/`.

---
## Socket
- A `socket` is a mechanism that allows programs to communicate with other programs using standard Unix file descriptors.

- There are several types of sockets:
  - `Internet sockets`
  - `Unix domain sockets`
  - ...

- Internet sockets also have multiple types, but we will focus on two of them:
  - **Stream Sockets (`SOCK_STREAM`)**: Used by applications such as `telnet`, `ssh`, and web browsers. For example, HTTP uses stream sockets to retrieve web pages.
  - **Datagram Sockets (`SOCK_DGRAM`)**: These are **connectionless sockets** that use the UDP protocol. They are often used in applications such as DNS, VoIP, online games, and streaming media.

- If we are sending chat messages, **TCP** is a good choice because it guarantees delivery and ordering.  
- However, if we are sending many updates per second (for example in real-time games or live streaming), losing one or two packets may not matter much, so **UDP** can be a better choice due to its lower overhead.

- `Socket programming` is a way of connecting two nodes on a network to communicate with each other. Sockets let our applications attach to the local network at different ports.
---

### 1. Stream Sockets
- To achieve a **high level of data transmission reliability**, stream sockets use the **TCP protocol**.
- TCP provides:
  - Reliable data delivery
  - Ordered packets
  - Error checking
  - Retransmission of lost packets
- In the **TCP/IP** model:
  - **IP (Internet Protocol)** handles packet routing across networks.
  - **TCP (Transmission Control Protocol)** ensures data integrity and reliable communication.

---

### 2. Datagram Sockets
- Datagram sockets use the **UDP (User Datagram Protocol)** instead of TCP.
- They are **connectionless**, meaning:
  - No persistent connection needs to be established before sending data.
  - Each packet is sent independently.

- Sending data typically works like this:

```
  1. Build a packet (datagram)
  2. Add a UDP header
  3. Add an IP header with destination information
  4. Send it over the network
```

- Applications that require **high speed and low latency** often use UDP.
- These applications usually tolerate **packet loss** and do not require guaranteed delivery.

---
### 3. IP - Internet Protocol
- An **IP address** is `a unique identifier` assigned to a device on a network (the Internet or a local network).
- It contains **location information** and allows devices to find and communicate with each other.
- The Internet needs a way to differentiate between billions of devices such as **computers, routers, servers, and websites**, and IP addresses provide this mechanism.

- **What is an IP?**
    - An **IP address** is a numeric label assigned to each device connected to a network.
    - In **IPv4**, it is written as **four numbers separated by periods** (called octets).

> IP addresses are not random. They are mathematically produced and allocated by the Internet Assigned Numbers Authority (IANA), a division of the Internet Corporation for Assigned Names and Numbers (ICANN). ICANN is a non-profit organization that was established in the United States in 1998 to help maintain the security of the internet and allow it to be usable by all. Each time anyone registers a domain on the internet, they go through a domain name registrar, who pays a small fee to ICANN to register the domain.

- **The IPv4 Address**
    - It is the most widely used IP format.
    - It uses **32 bits (4 bytes)** to represent an address.
    - This allows about **4.3 billion unique addresses**.
    - e.g. 192.168.1.1
- **The IPv6 Address**
    - It was introduced to solve the shortage of IPv4 addresses.
    - It uses **128 bits (16 bytes)**, allowing an extremely large number of unique addresses.
    - IPv6 addresses are written in **hexadecimal and separated by colons**.
    - e.g. 2001:db8:85a3::8a2e:370:7334
- **Real-world examples**
To get your IPAddress:

```bash
#linux
ip addr

#windows
ipconfig
```

- **What is an IP packet?**
- IP packets are created by adding an IP header to each packet of data before it is sent on its way. An IP header is just a series of bits (ones and zeros), and it records several pieces of information about the packet, including the sending and receiving IP address. IP headers also report:
    * Header length
    * Packet length
    * Which transport protocol is being used (TCP, UDP, etc.)

There are two prevailing types of data transfer protocol in the Internet Protocol (IP) suite:
. UDP – User Datagram Protocol
. TCP – Transmission Control Protocol

- **What is TCP/IP**
(reliable, connection oriented)

The Transmission Control Protocol (TCP) is a transport protocol, meaning it dictates the way data is sent and received. A TCP header is included in the data portion of each packet that uses TCP/IP. Before transmitting data, TCP opens a connection with the recipient. TCP ensures that all packets arrive in order once transmission begins. Via TCP, the recipient will acknowledge receiving each packet that arrives. Missing packets will be sent again if receipt is not acknowledged.

TCP is designed for reliability, not speed. Because TCP has to make sure all packets arrive in order, loading data via TCP/IP can take longer if some packets are missing.
- **What is UDP/IP**
(unreliable, connectionless)

The User Datagram Protocol, or UDP, is another widely used transport protocol. It's faster than TCP, but it is also less reliable. UDP does not make sure all packets are delivered and in order, and it does not establish a connection before beginning or receiving transmissions.

---

### 4. Localhost
- `Localhost` refers to the **current computer** that a program is running on.
- It allows a device to communicate **with itself** through the network stack.
- This is commonly used for **testing network applications locally** without needing an external network connection.
- The hostname **`localhost`** usually resolves to the **loopback IP address**: `127.0.0.1`
- Any network request sent to this address **never leaves the machine**. The operating system routes it back to the same device.

- **Real-world examples**
Ping localhost

```bash
# ping
ping 127.0.0.1

# connect to localhost:8080
telnet localhost 8080
```

---
### 5. Port 
- A **port** is a **communication endpoint** used by the operating system to direct network traffic to the correct application.
- A port is a **virtual point** where network connections start and end.
- Ports are **software-based** and managed by the **operating system**.
- Each port is associated with a **specific process or service**.

- **Port Range Groups:**
  - **0 to 1023** – Well known port numbers. Only special companies like Apple QuickTime, MSN, SQL Services, Gopher Services, and other prominent services have these port numbers.
  - **1024 to 49151** – Registered ports; meaning they can be registered to specific protocols by software corporations.
  - **49152 to 65536** – Dynamic or private ports; meaning that they can be used by just about anybody.
- **Examples**

```bash
22   → SSH  
25   → SMTP  
53   → DNS  
80   → HTTP  
443  → HTTPS  

https://example.com:443 -> This connects to the **HTTPS service** on port **443**.
```

---

### 6. DNS (Domain Name System)
- **DNS** is the system that translates **human-readable domain names** into **IP addresses** that computers use to identify each other on the network.
- This process is called **DNS resolution**.
- For example, a hostname such as: `www.example.com` may be translated into an IP address such as: `93.184.216.34`

- **Examples**: query a DNS server and print the IP address of a domain.

```bash
nslookup google.com
dig google.com
```

---

### 7. HTTP (Hypertext Transfer Protocol)
- **HTTP** is an **application-layer protocol** used for communication on the Web.
- It allows clients to **request resources** such as HTML pages, images, videos, scripts, and other files from servers.
- HTTP is the **foundation of data exchange on the World Wide Web**.

HTTP follows a **client–server model**:
- A **client** (usually a web browser) sends a **request**.
- A **server** processes the request and returns a **response**.

- When you open a website in a browser: https://example.com
    1. The browser resolves the domain name using **DNS**.
    2. It establishes a connection to the server using **TCP**.
    3. The browser sends an **HTTP request**.
    4. The server returns an **HTTP response** containing the requested resource.

- **Example**:

```bash
# When you open:
https://google.com

# Your browser sends something like:
GET /index.html HTTP/1.1
Host: example.com
User-Agent: Mozilla/5.0

# The server responds with:
HTTP/1.1 200 OK
Content-Type: text/html
Content-Length: 1024

<html>
  ...
</html>

# The browser then renders use HTML page
```

**1. HTTP Messages:**
- HTTP communication is based on **individual messages**, not a continuous stream.
- There are two types of messages:

**1.1. HTTP Request**
- Sent by the client to request a resource.
- Structure:

```bash
Method  
Headers  
(Optional) Body

Example:

POST /login HTTP/1.1
Host: example.com
Content-Type: application/json

{ "username": "user", "password": "1234" }
```

**1.2. HTTP Response**

Sent by the server to return the result.

Structure:

```bash
Status line  
Headers  
(Optional) Body

Example:

HTTP/1.1 200 OK
Content-Type: text/html

<html>...</html>
```


**2. HTTP Headers:**
- HTTP headers provide **additional information about the request or response**.

```bash
Host  
User-Agent  
Content-Type  
Content-Length  
Authorization  
Cookie  
```

**3. HTTP Methods:**
- HTTP methods define the **type of action** the client wants to perform.

    `GET`  
    - Retrieve data from the server.
    POST  
    - Send data to the server (often used for form submissions).

    `PUT`  
    - Update or replace a resource.

    `PATCH`  
    - Partially update a resource.

    `DELETE`  
    - Remove a resource.

    `HEAD`  
    - Same as GET but returns only headers (no body).

    `OPTIONS ` 
    - Ask the server which methods are supported.

---

## Socket Programming in C/C++
- `Socket programming` is a method that allows two programs (processes) to communicate with each other over a network.
- `A socket` represents `one endpoint` of a communication channel. Each socket is identified by:
    - an IP address
    - a port number
- In a typical client–server model:
    - The `server` creates a socket and `listens` on a specific port for incoming connections.
    - The `client` creates a socket and `connects` to the server’s IP address and port.
- After a connection is established, both sides can send and receive data until one side closes the connection.
- When creating a socket, a program must specify:
    - the `address` family (domain) e.g AF_INET for IPv4
    - the `socket type` e.g. SOCK_STREAM (TCP) or SOCK_DGRAM (UDP)
- The communicating processes can run:
    - on the same machine (e.g., inter-process communication)
    - on different machines connected through a network such as a LAN or the Internet.

- **Real-world examples**
When we open a website like https://example.com:
-> 1. Our browser (client) creates a socket and connects to the web server at:
IP: 93.184.216.34
Port: 80 (HTTP) or 443 (HTTPS).
-> 2. The browser sends an HTTP request.
-> 3. The server sends back the HTML page.

```
Browser (Client)  --->  Web Server
      socket           listening socket
```

- **Client-Server Definition:**
- Client-server denotes a relationship between cooperating programs in an application, composed of clients initiating requests for services and servers providing that function or service. 
 
- **Procedure in Client-Server Communication:**
o	`Socket`: Create a new communication
o	`Bind`: Attach a local address to a socket
o	`Listen`: Announce willingness to accept connections
o	`Accept`: Block caller until a connection request arrives
o	`Connect`: Actively attempt to establish a connection
o	`Send`: Send some data over a connection
o	`Receive`: Receive some data over a connection
o	`Close`: Release the connection

```cpp
//  start()
//  ├── createSocket()
//  ├── configureSocket()
//  ├── bindSocket()
//  ├── listenSocket()
//  └── acceptLoop()
//         └── handleClient()

server_fd = socket(...);
bind(server_fd, ...);
listen(server_fd, ...);
while (true) {
    client_fd = accept(server_fd, ...);
    handle_client(client_fd);
    close(client_fd);   // very important
}
close(server_fd);       // when server stops
```