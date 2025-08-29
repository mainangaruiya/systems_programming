
```markdown
# 💸 Money Sending and Receiving Platform (C Sockets)

This project is a simple client–server application written in **C** that demonstrates how to send and receive messages (simulating money transactions) using **TCP sockets**.

---

## 📂 Project Structure

```

├── server.c   # Server code – listens for client connections
├── client.c   # Client code – sends money request

````

---

## ⚡ Requirements

- GCC (or any C compiler)
- Linux / macOS (works best). On Windows, use WSL or MinGW.

---

## 🚀 How to Run

### 1. Compile the server and client
```bash
gcc server.c -o server
gcc client.c -o client
````

### 2. Run the server

In the first terminal window:

```bash
./server
```

### 3. Run the client

In a second terminal window:

```bash
./client
```

### 4. Expected Output

* **Client side**: nothing (just sends the message).
* **Server side**:

```
Received: Send $100
```

---

## 🌐 Running on Different Machines

1. Replace `127.0.0.1` in `client.c` with the **IP address of the server machine**.
2. Recompile and run again.

---

## 🔮 Future Improvements

* Add error handling (check return values of `socket`, `bind`, `connect`, etc.)
* Support multiple clients
* Implement simple user authentication
* Log all transactions

---

## 📖 Learning Concepts

This project demonstrates:

* Creating and binding sockets
* Listening and accepting client connections
* Sending and receiving messages over TCP

```

---

Want me to also include a **step-by-step demo GIF / ASCII example** inside the README (like showing two terminals side-by-side), or keep it short and clean?
```
