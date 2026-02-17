#include "abm.hpp"

int main() {

    // Initializing the model
    ABM model(100, 10);

    // Getting a glimpse of the initial state of the model
    model.print();
    
    // Running with verbose mode (default)
    model.run();

    // Printing the epidemic curve
    std::cout << "Epidemic curve (columns: S, I, R):" << std::endl;
    model.get_epidemic_curve().print();
    
}
