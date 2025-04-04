#ifndef CADMIUM_EXAMPLE_CELLDEVS_FIREFLY_STATE_HPP_
#define CADMIUM_EXAMPLE_CELLDEVS_FIREFLY_STATE_HPP_

#include <iostream>
#include <nlohmann/json.hpp>

//! Firefly cell state for synchronization simulation.
struct fireflyState {
    int state;      // 0 = flash; 1 = pre-flash; 2-9: countdown states in the cycle
    bool flashing;  // 1 only when in flashing state (state 0), 0 otherwise
    
    fireflyState() : state(9), flashing(false){}   //Default

    fireflyState(int s, bool f, int age =0) : state(s), flashing(f){}
};

std::ostream& operator<<(std::ostream& os, const fireflyState& x) {
    os << "<" << x.state << ", " << ((x.flashing)? "1" : "0") << ">";
    return os;
}

// //! It prints the life of the cell in an output stream.
// std::ostream& operator<<(std::ostream& os, const conwayState& x) {
// 	os << "<" << x.flashing << ">";
// 	return os;
// }



bool operator!=(const fireflyState& x, const fireflyState& y) {
    return x.state != y.state || x.flashing != y.flashing;
}

// bool operator==(const fireflyState& x, const fireflyState& y) {
//     return x.state == y.state && x.flashing == y.flashing;
// }

// JSON parsing
void from_json(const nlohmann::json& j, fireflyState& s) {
    j.at("state").get_to(s.state);
    j.at("flashing").get_to(s.flashing);
}

// void to_json(nlohmann::json& j, const fireflyState& s) {
//     j = nlohmann::json{{"state", s.state}, {"flashing", s.flashing}};
// }


#endif // CADMIUM_EXAMPLE_CELLDEVS_FIREFLY_STATE_HPP_
