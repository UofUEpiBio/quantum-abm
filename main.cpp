#include "abm.hpp"

int main() {

    // Calibrating the model for an R0 of 1.5 and an infectious period of 7 days
    // With a probability of infection of 0.1.
    // R0 = contact_rate * prob_infection / prob_recovery
    // contact_rate = R0 * prob_recovery / prob_infection
    double prob_infection = 0.1;
    double prob_recovery = 1.0 / 7.0;
    double R0 = 1.5;
    double contact_rate = R0 * prob_recovery / prob_infection;

    // Initializing the model
    ABM model(1000, 10, prob_infection, prob_recovery, contact_rate, 0.5, 100);

    // Getting a glimpse of the initial state of the model
    model.print();
    
    // Running with verbose mode (default)
    model.run();

    // Printing the epidemic curve
    std::cout << "Epidemic curve (columns: S, I, R):" << std::endl;
    model.get_epidemic_curve().print();
    
}
