#ifndef CADMIUM_EXAMPLE_CELLDEVS_FIREFLY_HPP_
#define CADMIUM_EXAMPLE_CELLDEVS_FIREFLY_HPP_

#include <cmath>
#include <nlohmann/json.hpp>
#include <cadmium/modeling/celldevs/grid/cell.hpp>
#include <cadmium/modeling/celldevs/grid/config.hpp>
#include "fireflyState.hpp"


using namespace cadmium::celldevs;

class firefly : public GridCell<fireflyState, double> {
public:
    firefly(const std::vector<int>& id,
            const std::shared_ptr<const GridCellConfig<fireflyState, double>>& config)
        : GridCell<fireflyState, double>(id, config) { }

    //Compute the next state of the firefly cell based on its current state and its neighbors.
    // [[nodiscard]] fireflyState localComputation(
    //     fireflyState state,
    //     const std::unordered_map<std::vector<int>, NeighborData<fireflyState, double>>& neighborhood) const override {

    //     // Check if any neighbor is flashing (state==0).
    //     bool neighborFlashing = false;
    //     for (const auto& [neighborId, neighborData] : neighborhood) {
    //         if (neighborData.state->state == 0) {
    //             neighborFlashing = true;
    //             break;
    //         }
    //     }

    //     // If this cell is already flashing...
    //     if (state.state == 0) {
    //         if (state.flashing) {
    //             // Hold the flash state for one cycle so neighbors see it.
    //             state.flashing = false;
    //         } else {
    //             // Then reset to 9 after the flash.
    //             state.state = 9;
    //         }
    //     } else {
    //         // Natural countdown: subtract 1.
    //         int decrement = 1;
    //         // If any neighbor is flashing, subtract an extra 2.
    //         if (neighborFlashing) {
    //             decrement += 1;
    //         }
    //         int newValue = state.state - decrement;
    //         if (newValue <= 0) {
    //             // The cell flashes.
    //             state.state = 0;
    //             state.flashing = true;
    //         } else {
    //             // Continue the countdown.
    //             state.state = newValue;
    //             state.flashing = false;
    //         }
    //     }

    //     return state;
    // }
    [[nodiscard]] fireflyState localComputation(fireflyState state,
        const std::unordered_map<std::vector<int>, NeighborData<fireflyState, double>>& neighborhood) const override {
        
        // Check if any immediately adjacent neighbor is flashing (state 0)
        int neighborFlashing = 0;
        bool neighborIsNine = false;
        for (const auto& [neighborId, neighborData] : neighborhood) {
            auto nState = neighborData.state;
            if (nState->state == 0) {
                neighborFlashing++;
            }
        }
    
        // Cycle through the 10 states:
        // - If state is 0 (flash), then transition to state 9 (post-flash dark).
        // - If state is 1 (pre-flash), then transition to state 0 (flash).
        // - For states 2 through 9:
        //     - If any neighbor is flashing (state 0), reset to state 9 (synchronize).
        //     - Else if any neighbor is in state 9, transition to state 8.
        //     - Otherwise, simply decrement the state.
        if (state.state == 0) {
            // Just finished flashing, move to post-flash state.
            state.state = 9;
            state.flashing = false;
        } else if (state.state == 1) {
            // Pre-flash state always transitions to flash.
            state.state = 0;
            state.flashing = true;
        } else if (state.state >= 2 && state.state <= 9) {
            if (neighborFlashing >= 1) {
                // Synchronize: reset the cycle to post-flash state.
                state.state = 9;
            } else {
                // Continue the natural countdown.
                state.state = state.state - 1;
            }
            state.flashing = false;
        }
        return state;
    }

        // Defines the output delay for each cell update.
        [[nodiscard]] double outputDelay(const fireflyState& state) const override {
            return 1.;
        }
    };
    
    #endif // CADMIUM_EXAMPLE_CELLDEVS_FIREFLY_HPP_

// #ifndef CADMIUM_EXAMPLE_CELLDEVS_FIREFLY_HPP_
// #define CADMIUM_EXAMPLE_CELLDEVS_FIREFLY_HPP_

// #include <cmath>
// #include <nlohmann/json.hpp>
// #include <cadmium/modeling/celldevs/grid/cell.hpp>
// #include <cadmium/modeling/celldevs/grid/config.hpp>
// #include "fireflyState.hpp"

// using namespace cadmium::celldevs;

// class firefly : public GridCell<fireflyState, double> {
// public:
//     firefly(const std::vector<int>& id,
//             const std::shared_ptr<const GridCellConfig<fireflyState, double>>& config)
//         : GridCell<fireflyState, double>(id, config) { }

//     // Compute the next state of the firefly cell based on its current state and its neighbors.
//     // This version uses a cooldown period so that if a cell has already synchronized
//     // (i.e. reset its flash) in the last syncCooldown cycles, it won't synchronize again.
//     [[nodiscard]] fireflyState localComputation(fireflyState state,
//         const std::unordered_map<std::vector<int>, NeighborData<fireflyState, double>>& neighborhood) const override {
    
//         // Parameters:
//         // flashThreshold: maximum age for a neighbor to be considered as having flashed "recently"
//         // syncCooldown: minimum number of cycles since last synchronization before allowing a new sync trigger
//         const int flashThreshold = 2;
//         const int syncCooldown = 3;
        
//         bool neighborFlashedRecently = false;
//         int neighborFlashing = 0;
        
//         // Check each neighbor for a flash event.
//         for (const auto& [neighborId, neighborData] : neighborhood) {
//             auto nState = neighborData.state;
//             if (nState->state == 0) {
//                 // Neighbor is flashing now.
//                 neighborFlashing++;
//                 neighborFlashedRecently = true;
//             } else if (nState->flashAge < flashThreshold) {
//                 // Neighbor flashed very recently.
//                 neighborFlashedRecently = true;
//             }
//         }
        
//         // Process the cell's own state:
//         if (state.state == 0) {
//             // Just flashed: transition to post-flash state and reset flashAge.
//             state.state = 9;
//             state.flashing = false;
//             state.flashAge = 0;
//         } else if (state.state == 1) {
//             // Pre-flash always transitions to flash.
//             state.state = 0;
//             state.flashing = true;
//             state.flashAge = 0;
//         } else if (state.state >= 2 && state.state <= 9) {
//             // For states 2 through 9:
//             // Only synchronize (reset to 9) if a neighbor is flashing or flashed recently
//             // AND this cell hasn't synchronized within the last syncCooldown cycles.
//             if ((neighborFlashing > 0 || neighborFlashedRecently) && state.flashAge >= syncCooldown) {
//                 state.state = 9;
//                 state.flashAge = 0;
//             } else {
//                 // Otherwise, continue the natural countdown.
//                 state.state = state.state - 1;
//                 state.flashAge++;
//             }
//             state.flashing = false;
//         }
        
//         return state;
//     }
    
//     // Defines the output delay for each cell update.
//     [[nodiscard]] double outputDelay(const fireflyState& state) const override {
//         return 1.;
//     }
// };

// #endif // CADMIUM_EXAMPLE_CELLDEVS_FIREFLY_HPP_

    
     
    
    


