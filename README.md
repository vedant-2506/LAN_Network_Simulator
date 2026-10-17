# LAN Network Simulator

## Table of Contents
- [Introduction](#introduction)
- [Project Overview](#project-overview)
- [Features](#features)
- [Technologies Used](#technologies-used)
- [Directory Structure](#directory-structure)
- [Architecture and Working Flow](#architecture-and-working-flow)
- [Packet Structure](#packet-structure)
- [Collision Detection and Packet Delivery](#collision-detection-and-packet-delivery)
- [Prerequisites and Installation](#prerequisites-and-installation)
- [Build Instructions](#build-instructions)
- [Running the Program](#running-the-program)
- [Usage Example and Sample Output](#usage-example-and-sample-output)
- [Important Functions and Data Structures](#important-functions-and-data-structures)
- [Random Number Generation and Configuration](#random-number-generation-and-configuration)
- [Output and Statistics](#output-and-statistics)
- [Limitations](#limitations)
- [Troubleshooting](#troubleshooting)
- [Future Improvements](#future-improvements)
- [Learning Outcomes](#learning-outcomes)
- [Repository Information](#repository-information)

---

## Introduction

**LAN Network Simulator** is a lightweight C/C++ program that demonstrates basic LAN‑level networking concepts using a simulated environment. It models multiple hosts that generate random packets, move them through a series of buffers, broadcast over a shared LAN, detect collisions, and collect transmission statistics.

The simulator is **pure software** – it does not interact with real network hardware. It is intended as an educational tool for learning about packet headers, buffering, broadcast behaviour, and simple collision detection.

---

## Project Overview

1. **Host creation** – The user specifies a host count (3‑26). Each host receives a unique identifier, a network address (constant `7` in this implementation), a machine address equal to its host id, and a simple source MAC (`smac`) represented by a capital letter (`'A' + host_id`).
2. **Random packet generation** – For each iteration, each host may generate a packet with a 10 % probability. The packet contains:
   - Header fields (sender id, sender MAC, network address, receiver id, receiver MAC, payload length).
   - A payload of random alphabetic characters.
3. **Buffers** – Packets travel through a chain of buffers:
   - **Test Processor** (`test_processor_ports`) – initial storage after generation.
   - **LLL (Lower‑Level Layer)** – two stages: upper (`lll_upper_ports`) and lower (`lll_lower_ports`).
   - **LAN** (`lan_ports`) – shared broadcast medium.
4. **LAN transmission** – All hosts expose the packet at the LAN output side. If exactly one host has a packet, the simulator copies it to the LAN input side of every other host. If more than one host attempts to transmit, a collision is detected and the packet data is discarded.
5. **Destination checking** – Each host extracts the receiver information from the packet header. If the packet is addressed to the host, it is delivered to the host’s Test Processor; otherwise it is dropped.
6. **Statistics** – Counters for total packets sent, received and collisions are maintained and displayed when the simulation ends.

---

## Features

- Configurable number of hosts (3‑26).
- Random packet generation with configurable probability (currently 10 %).
- Fixed‑size buffers (`BUFFER_SIZE = 256`).
- Simple packet header (6 bytes) and variable payload length.
- Broadcast‑style LAN with collision detection.
- Automatic statistics collection (sent, received, collisions).
- Build system based on a standard GNU Makefile.
- Pure C/C++ implementation, no external libraries required.

---

## Technologies Used

| Category | Tool / Language |
|---|---|
| Language | C (C11) for the core simulation, C++ (C++17) for the tiny driver (`MS2503_main.cpp`). |
| Compiler | `gcc` for C files, `g++` for C++ files. |
| Build System | GNU Make (`makefile`). |
| Version Control | Git (local repository, remote on GitHub). |

---

## Directory Structure

```
LAN_Network_Simulator/
├── .gitignore          # Ignored build artefacts, IDE files, etc.
├── Makefile           # Build rules for the project.
├── README.md          # This documentation.
├── MS2503_main.cpp    # Entry point – calls `RunNW_Simulation()`.
├── MS2503_fun.c       # Core simulation implementation.
├── MS2503_fun.h       # Public API declarations.
└── MS2503_param.h     # Configuration constants and type definitions.
```

- **Makefile** – defines targets `all` (build) and `clean`.
- **MS2503_main.cpp** – contains `main()` which invokes the simulation.
- **MS2503_fun.c / .h** – implement functions for random number generation, packet creation, buffer handling, LAN transmission, and statistics.
- **MS2503_param.h** – holds constants such as `MAX_HOSTES`, `BUFFER_SIZE`, `PACKET_HEADER_BYTES`, and the `Port` / `HostInfo` structures.

---

## Architecture and Working Flow

```
User Input → Initialise Hosts → Loop per Iteration
   ├─ Generate random packets (probability 10 %)
   ├─ SendData() – move packet from Test Processor to LLL‑upper
   ├─ MoveToLan() – copy LLL‑upper → LLL‑lower → LAN‑in
   ├─ SendOverLAN() – broadcast or detect collision
   ├─ ConsumeData() – LAN‑out → LLL‑lower‑in
   ├─ IPstrip() – extract destination & deliver or drop
   └─ ProcessReceivedPackets() – print and count received packets
```

The simulation consists of three logical layers:
- **Test Processor (TP)** – where a host creates a packet.
- **LLL (Lower‑Level Layer)** – two‑stage buffer that mimics a NIC/driver.
- **LAN** – shared broadcast medium where collisions may occur.

Each buffer is represented by a `Port` struct:
```c
typedef struct {
    unsigned char d[2][BUFFER_SIZE]; // d[0] = output, d[1] = input
} Port;
```
`d[0][0]` (or `d[1][0]`) holds the byte count of the current packet; the remaining array holds the actual bytes.

---

## Packet Structure

The packet occupies the first `byte_count` bytes of the buffer where `byte_count` ≥ `PACKET_HEADER_BYTES` (6). Header layout (indices are zero‑based):
| Byte | Meaning |
|---|---|
| 0 | Sender host id (0‑25) |
| 1 | Sender MAC (`'A' + host_id`) |
| 2 | Sender network address (constant `7`) |
| 3 | Receiver host id |
| 4 | Receiver MAC (`'A' + receiver_id`) |
| 5 | Payload length (including header) |

The payload follows the header and consists of random lower‑case letters (`'a'`‑`'z'`). Example (host 0 → host 1, payload length 15):
```
0 1 2 3 4 5 6 7 8 9 10 11 12 13 14
0 A 7 1 B 15 a b c d e f g h i j k l m n o
```
The simulator prints the packet as a mixed list of characters (for header bytes that are numeric) and letters for the payload.

---

## Collision Detection and Packet Delivery

- **No host ready** – `SendOverLAN` returns without action.
- **Exactly one host ready** – its packet is copied to the LAN input side of every other host. The sender’s LAN output buffer is cleared.
- **Multiple hosts ready** – a collision is reported, `total_collisions` is incremented, and **all** LAN buffers are cleared, discarding the packets.
- **Destination check** – `IPstrip` examines the receiver id (byte 4). If it matches the current host, the packet is moved to the host’s Test Processor input buffer and counted as received. Otherwise the packet is dropped with a diagnostic message.

The model does **not** implement exponential back‑off, retransmission, or a full CSMA/CD protocol; collisions simply result in loss of the packet data.

---

## Prerequisites and Installation

### Ubuntu / Debian
```bash
sudo apt update
sudo apt install build-essential git   # gcc, g++, make, git
```
### Fedora / CentOS
```bash
sudo dnf groupinstall "Development Tools"
```
### macOS (Homebrew)
```bash
brew install gcc git
```
> The project uses only the standard C and C++ libraries; no third‑party dependencies are required.

---

## Build Instructions

```bash
cd "/home/vedant-2506/Documents/SCMS SEM 3/NW/LAN_Network_Simulator"
make
```
The command compiles `MS2503_main.cpp` with `g++` and `MS2503_fun.c` with `gcc`, then links them into the executable **MS2503**.

To force a clean rebuild:
```bash
make clean
make
```
If the `make` command reports *"Nothing to be done for 'all'"*, the binary is already up‑to‑date.

---

## Running the Program

```bash
./MS2503
```
The program prompts for the number of hosts (3‑26). During each iteration:
1. Hosts may generate a packet (10 % chance).
2. Generated packets travel through the buffers and are broadcast.
3. Collisions are detected and reported.
4. Received packets are displayed and counted.
5. After the iteration the user may press **Q**/**q** to quit or any other key to continue.

Invalid host counts cause an error message and the program exits.

---

## Usage Example and Sample Output

```
Enter the number of hosts (3 to 26): 4

ITERATION NO. : 1
--------------------
Data generated by host 2 for host 0
Generating SIP and SMAC header.
Packet = "2 B 7 0 A 13 a b c d e f g h i j k l m"

***** Sending data over the LAN *****
Connecting TP and LLL of 2
Copying LLL-in to LLL-out of 2
Connecting LLL and LAN of 2
Copying LAN-in to LAN-out of 2
connecting LAN and LLL of 0
connecting LAN and LLL of 1
connecting LAN and LLL of 2
connecting LAN and LLL of 3
Copied packet to LLL of all hosts

Host0 says : "This packet: is not for me ! I drop it !"
Host1 says : "This packet: is not for me ! I drop it !"
Host2 has received the packet :
Packet
__________________________________
2 B 7 0 A 13 a b c d e f g h i j k l m

ENTER Q to quit; ANY OTHER CHARACTER to continue : q

Packets sent: 1
Packets received: 1
Collisions: 0
```
*The output above is taken from a single iteration; actual runs will vary due to randomness.*

---

## Important Functions and Data Structures

| Symbol | Definition / Purpose |
|---|---|
| `Port` | Buffer container with two slots (`d[0]` output, `d[1]` input). |
| `HostInfo` | Stores `host_id`, `network_address`, `machine_address`, `smac` (source MAC). |
| `RandomNumberGenerator()` | Simple linear‑congruential generator used throughout the simulator. |
| `initRandomSeed(unsigned long seed)` | Initialise RNG; seed defaults to `time(NULL)`. |
| `FullReset(int n_host)` | Initialise global state, counters and host information. |
| `GenerateHost(int n_host)` | Populate `hosts` array with identifiers and MAC letters. |
| `GenerateHeader(...)` | Fill the first six bytes of a packet with sender/receiver data. |
| `GenerateData(int host_id)` | Create a packet for `host_id` with random payload size. |
| `SendData(int host_id)` | Move the packet from Test Processor to LLL‑upper buffer. |
| `MoveToLan(int host_id)` | Chain copies: TP → LLL‑upper → LLL‑lower → LAN‑in. |
| `SendOverLAN(int n_hosts)` | Broadcast a single packet or detect a collision. |
| `ConsumeData(int host_id)` | Move LAN‑out to LLL‑lower‑in for further processing. |
| `IPstrip(int host_id)` | Examine the header; deliver to Test Processor if destination matches, otherwise drop. |
| `ProcessReceivedPackets(int n_hosts)` | Print received packets and update `total_packets_received`. |
| `RunNW_Simulation()` | Main user‑interaction loop handling iterations and termination. |

---

## Random Number Generation and Configuration

The simulator uses a linear‑congruential generator (LCG):
```c
random_state = (RANG_MULTIPLIR * random_state + RANG_INCREMENT) % RANG_MODULUS;
```
Constants are defined in `MS2503_param.h`.
- Seed is set with `initRandomSeed(time(NULL))`, so each run is nondeterministic.
- To obtain reproducible runs, the user could replace the seed argument with a fixed value (e.g., `initRandomSeed(1)`). The current code does not expose a command‑line option for this.

---

## Output and Statistics

- **Packets sent** – Incremented each time `SendData` moves a packet from the Test Processor to LLL‑upper.
- **Packets received** – Incremented when a host successfully extracts a packet addressed to it (`IPstrip` copies the payload into the host’s Test Processor input buffer).
- **Collisions** – Incremented whenever more than one host has a packet ready for LAN transmission in the same iteration (`SendOverLAN`). All LAN buffers are cleared in this case.

These counters are printed after the user quits the simulation.

---

## Limitations

- Addresses are simplified integers; there is no real IP or MAC handling.
- Collision handling is simplistic – packets are lost without any back‑off or retransmission.
- The network topology is a single broadcast LAN; no switches, routers, or multiple segments are modelled.
- Randomness is seeded from the current time; reproducibility requires code modification.
- No command‑line parameters; all configuration is compiled‑time or prompted at runtime.

---

## Troubleshooting

| Symptom | Likely Cause | Remedy |
|---|---|---|
| `make: Nothing to be done for 'all'` | Build artefacts already up‑to‑date. | Run `make clean && make` to force recompilation. |
| `gcc: command not found` / `g++: command not found` | Build tools missing. | Install `build-essential` (Ubuntu) or the equivalent development tools for your OS. |
| `./MS2503: No such file or directory` | Build failed or binary removed. | Verify `make` succeeded and that `MS2503` exists. |
| Invalid host count message | Input outside 3‑26 range or non‑numeric input. | Provide a number within the allowed range. |
| Git push rejected / authentication error | Remote branch diverged or missing credentials. | Pull the latest changes (`git pull`) and resolve any conflicts, then push again. Ensure GitHub authentication (SSH key or token) is configured. |

---

## Future Improvements

- Add command‑line options for host count, probability, and RNG seed.
- Implement exponential back‑off and retransmission after collisions.
- Extend the model to support multiple network segments or a simple switch.
- Provide deterministic runs via a command‑line seed flag.
- Add unit tests and CI pipelines.
- Replace the custom LCG with `rand_r` or `<random>` for better randomness.

---

## Learning Outcomes

- Understanding of basic packet header design and payload handling in C.
- Working with fixed‑size buffers and manual memory copying.
- Modelling a broadcast LAN and simple collision detection.
- Building a mixed C/C++ project with a Makefile.
- Using Git for version control and preparing a professional repository layout.

---

## Repository Information

[https://github.com/vedant-2506/LAN_Network_Simulator](https://github.com/vedant-2506/LAN_Network_Simulator)

---
