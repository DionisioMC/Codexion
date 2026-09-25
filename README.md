*This project has been created as part of the 42 curriculum by dcoelho.*
 
# Codexion
 
## Description
 
Codexion is a multithreaded C simulation built around the classic **Dining
Philosophers** synchronization problem, re-themed for a software theme instead of philosophers sharing forks, `coders` share `dongles` to get their work done.
 
Each coder repeatedly cycles through three phases:
 
1. **Compile** – requires holding **both** of the coder's neighbouring
   dongles simultaneously.
2. **Debug** – performed alone, no dongle needed.
3. **Refactor** – performed alone, no dongle needed.
Coders are arranged in a ring, and each dongle is shared between exactly two
adjacent coders, so only a limited number of coders can compile at any given
moment. If a coder goes too long without starting a new compile, they
**burn out** and the simulation stops.
 
The goal of the project is to implement this concurrent system correctly
using POSIX threads, mutexes and condition variables, while avoiding the
classic pitfalls of concurrent programming:
 
- **Deadlock** — coders stuck forever waiting on each other's dongles, leaving the simulation stuck.
- **Starvation** — a coder that never gets a fair chance to compile.
- **Data races** — unsynchronized access to shared state. Dongle access is arbitrated with a per-dongle priority queue (implemented as
a binary heap), and two scheduling policies are available:
 
- `fifo` — *First In First Out*: Dongles are granted in the order they were requested.
- `edf` — *Earliest Deadline First*: The coder closest to burning out is
  given priority, to reduce the risk of unnecessary burnouts.

## Instructions

### Compilation
 
```sh
make
```
 
This builds the `codexion` binary using `cc` with `-Wall -Wextra -Werror -pthread`.
 
Other available targets:
 
```sh
make clean   # remove object files
make fclean  # remove object files and the binary
make re      # fclean + all
```
 
### Usage
 
```sh
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

| Argument                        |Description                                                              |
|----------------------------------|----------------------------------------------------------------------------|
| `number_of_coders`               | Number of coders (and dongles) in the simulation                          |
| `time_to_burnout`                 | Time (ms) a coder can go without starting a compilation before burning out|
| `time_to_compile`                 | Duration (ms) of the compile phase                                         |
| `time_to_debug`                   | Duration (ms) of the debug phase                                           |
| `time_to_refactor`                | Duration (ms) of the refactor phase                                        |
| `number_of_compiles_required`     | Number of compiles each coder must complete for the simulation to end successfully |
| `dongle_cooldown`                 | Time (ms) a dongle must be unable to be taken again     |
| `scheduler`                       | Dongle-arbitration policy: `fifo` or `edf`                                 |
 
All numeric arguments must be non-negative integers.
 
### Example
 
```sh
./codexion 5 800 200 200 200 7 50 fifo
```
 
Runs a simulation with 5 coders, an 800 ms burnout threshold, 200/200/200 ms
compile/debug/refactor phases, 7 required compiles per coder, a 50 ms dongle
cooldown, and FIFO dongle arbitration.
 
Each event is logged as:
 
```
<timestamp_ms> <coder_id> <event>
```
 
for example:
 
```
0 1 has taken a dongle
0 1 is compiling
```

The simulation ends either when every coder has completed the required
number of compiles, or as soon as any coder burns out.

## Resources

### AI usage

AI was used during development for the following tasks:

- **Documentation**: drafting this `README.md` from the project's source
  code and the 42 subject requirements.
All AI-assisted suggestions were reviewed, tested, and validated manually
before being integrated into the project. No code was generated and
committed without understanding and verifying its behaviour first.