# Fireflies Synchronization Simulation


This project simulates the phenomenon of firefly synchronization, where fireflies flash in unison. Using a cell-based model implemented in C++ and configured via JSON, the simulation is built upon the Cell-DEVS methodology. The objective is to demonstrate how local interactions among cells can lead to global synchronization.

## Overview

In the simulation, each firefly is modeled as a cell that cycles through ten discrete states (0 through 9):

- **State 0:** Flash (on) state.
- **State 1:** Pre-flash state that transitions immediately to state 0.
- **States 2–9:** Dark or off states, during which a cell decrements its state by 1 per cycle.

A simple synchronization mechanism is implemented: if any adjacent neighbor is flashing (state 0), a cell in states 2–9 resets its countdown to state 9 rather than continuing its natural decrement. This rule creates a cascading effect, eventually synchronizing the entire grid.

## System Architecture

### Cellular Model

- **Cell States:** The set of possible states is defined as S = {0, 1, 2, …, 9}, where:
  - State 0 represents a flash.
  - State 1 is the pre-flash state.
  - States 2–9 represent varying degrees of the off cycle.
  
- **Transition Rules:** 
  - If a cell is in state 0, it transitions to state 9.
  - If a cell is in state 1, it transitions to state 0.
  - For cells in states 2 through 9, if at least one neighboring cell is flashing (state 0), the cell resets to state 9; otherwise, it decrements its state by 1.

- **Delay:** Each state transition occurs after a fixed delay of 1 second.
- **Grid Configuration:** The simulation uses a 20×20 wrapped (toroidal) grid to eliminate edge effects.
- **Neighborhood:** 
  - **Initial Configuration:** A Moore neighborhood with range 1 (immediate neighbors).
  - **Extended Configuration:** The neighborhood is later extended (e.g., Moore range 2) to allow cells to detect flashes from more distant neighbors, which enhances the synchronization cascade.

## Implementation Details

### Programming Environment

- **Language:** C++
- **Configuration:** JSON is used for setting simulation parameters.
- **Methodology:** The simulation is based on the Cell-DEVS framework, allowing for a modular and scalable design.

### Cell Behavior Implementation

The behavior of each cell follows these rules:
- **Flash Rule:** A cell in state 0 resets to state 9 after a 1-second delay.
- **Pre-Flash Rule:** A cell in state 1 always transitions to state 0.
- **Synchronization Rule:** For cells in states 2–9, if any neighboring cell is flashing, the cell’s next state is forced to 9. Otherwise, the cell decrements its state by 1.

### JSON Configuration

The simulation parameters, including grid dimensions, initial cell states, and neighborhood definitions, are defined in a JSON configuration file. The initial configuration employs a Moore neighborhood with range 1, which is later extended (e.g., to range 2) to promote global synchronization.


### Installation

1. Clone the repository:
   https://github.com/T-Nemeh/FireFlies.git
2. Navigate to directory: cd Fireflies
3. Build the project:
   source build_sim.sh
4. Run the project:
   ./bin/conway_life config/firefliesVisualization_config.json 
