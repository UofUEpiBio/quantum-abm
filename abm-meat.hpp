#include<vector>
#include<iostream>
#include<random>
#include "abm-bones.hpp"
#include "utils.hpp"

#ifndef ABM_MEAT_HPP
#define ABM_MEAT_HPP

// Meat definitions
inline void Group::add_agent(Agent& agent) {
    agents.push_back(&agent);
    agent.set_group(*this);
};

inline double Group::get_n_infected() const {
    double n_infected = 0.0;
    for (const auto agent: agents) {
        if (agent->get_state() == State::INFECTED) {
            n_infected += 1.0;
        }
    }
    return n_infected;
};

inline double Group::sample_infected() const {

    double n_infected = get_n_infected();
    double n_groups   = abm->get_n_groups();

    if (n_infected == 0) {
        return 0.0;
    }

    // Figure out the contact rate
    double crate = abm->get_contact_rate() * (
        restricted_contact ? 1.0 : 
        abm->get_contact_rate_reduction()
    );

    double p_infection = crate / n_infected / n_groups ;
    return abm->rbinom(
        static_cast<double>(n_infected),
        p_infection
    );

}

inline int Agent::get_state() const {
    return state;
};

inline void Agent::set_state(int new_state) {
    state_next = new_state;
};

inline void Agent::set_group(Group& g) {
    group = &g;
};

inline void ABM::m_update_susceptible(Agent& agent) {

    // Sample from each group
    double n_contacts_infected = 0.0;
    for (const auto& group: groups) {
        n_contacts_infected += group.sample_infected();
    }

    // Computing the probability of infection
    double p_infection =
        1.0 - std::pow(1.0 - prob_infection, n_contacts_infected);

    // If worth, then the agent becomes infected
    if (p_infection > runif()) {
        m_log(
            "Agent " + std::to_string(&agent - &agents[0]) + " got infected." +
            " (p_infection = " + std::to_string(p_infection) + ")" +
            " (n_contacts_infected = " + std::to_string(n_contacts_infected) + ")"
        );
        agent.set_state(State::INFECTED);
    } else
    {
        // Ensuring the agent remains susceptible if they don't get infected
        agent.set_state(State::SUSCEPTIBLE);
    }
}

inline void ABM::m_update_infected(Agent& agent) {

    if (prob_recovery > runif()) {
        m_log(
            "Agent " + std::to_string(&agent - &agents[0]) + " recovered." +
            " (p_recovery = " + std::to_string(prob_recovery) + ")"
        );
        agent.set_state(State::RECOVERED);
    } else {
        // Ensuring the agent remains infected if they don't recover
        agent.set_state(State::INFECTED);
    }
}

inline void ABM::m_step(int step_number) {


    m_log("Step " + std::to_string(step_number) + " started.");

    for (auto& agent: agents) {
        switch (agent.get_state()) {
            case State::SUSCEPTIBLE:
                m_update_susceptible(agent);
                break;
            case State::INFECTED:
                m_update_infected(agent);
                break;
            case State::RECOVERED:
                // No update needed for recovered agents
                break;
        }
    }

    // Updating the states
    for (auto& agent: agents) {
        agent.state = agent.state_next;
    }

    // Printing the distribution of states
    double n_susceptible = 0.0;
    double n_infected = 0.0;
    double n_recovered = 0.0;
    for (const auto& agent: agents) {
        switch (agent.get_state()) {
            case State::SUSCEPTIBLE:
                n_susceptible += 1.0;
                break;
            case State::INFECTED:
                n_infected += 1.0;
                break;
            case State::RECOVERED:
                n_recovered += 1.0;
                break;
        }
    }

    // Updating the epidemic curve
    epidemic_curve(step_number, 0) = n_susceptible;
    epidemic_curve(step_number, 1) = n_infected;
    epidemic_curve(step_number, 2) = n_recovered;

    m_log(
        "Step " + std::to_string(step_number) + " ended. " +
        "{S, I, R}: {" + std::to_string(n_susceptible) + ", " +
        std::to_string(n_infected) + ", " +
        std::to_string(n_recovered) + "}"
    );

}

inline void ABM::m_log(std::string msg) {
    if (verbose) {

        // Formatting the log number
        std::string log_number = std::to_string(log_step++);
        std::string util_number = std::to_string(m_calc_utility());

        log_number = std::string(4 - log_number.size(), '0') + log_number;
        
        // Ensuring we only print two decimal places for the utility
        util_number = util_number.substr(0, util_number.find(".") + 3);

        std::cout << "[log " << 
        log_number << "; utility = " << util_number << "] " << msg << std::endl;
    }
}

inline ABM::ABM(
    size_t n_agents,
    size_t n_groups,
    double prob_infection,
    double prob_recovery,
    double contact_rate,
    double contact_rate_reduction,
    int n_steps,
    size_t seed
) : prob_infection(prob_infection),
    prob_recovery(prob_recovery),
    contact_rate(contact_rate),
    contact_rate_reduction(contact_rate_reduction),
    n_steps(n_steps)
{
    
    // Setting the seed
    rng.seed(seed);

    // Creating the agents
    agents.resize(n_agents);
    groups.resize(n_groups);

    // Initializing the epidemic curve
    epidemic_curve = Matrix(n_steps, 3);

    // Initializing the groups
    // Agents will be distribured uniformly across the groups
    // We are assuming that n_agents is divisible by n_groups, otherwise we would need to handle the remaining agents
    size_t n_per_group = n_agents / n_groups;

    int agent_index = 0;
    for (auto& group: groups) {

        group.abm = this;

        for (size_t i = 0; i < n_per_group; ++i) {
            group.add_agent(agents[agent_index++]);
        }
    }

    // Forcing the first agent to be infected
    agents[0].state = State::INFECTED;
    
};

inline void ABM::run() {
    for (int i = 0; i < n_steps; ++i)
        m_step(i);
};

inline void ABM::print() const {
    std::cout << "ABM with " << agents.size() << " agents and " << groups.size() << " groups." << std::endl;
};

inline void ABM::toggle_verbose() {
    verbose = !verbose;
};

inline double ABM::rbinom(double n, double p) {
    return binomial_dist(
        rng, std::binomial_distribution<int>::param_type(n, p)
    );
};

inline double ABM::runif(double a, double b) {
    return uniform_dist(
        rng, std::uniform_real_distribution<double>::param_type(a, b)
    );
};

inline Matrix& ABM::get_epidemic_curve() {
    return epidemic_curve;
};

inline double ABM::get_contact_rate() const {
    return contact_rate;
};

inline double ABM::get_contact_rate_reduction() const {
    return contact_rate_reduction;
};

inline double ABM::m_calc_utility() {

    // Placeholder for utility calculation
    auto infected_per_group = get_n_infected_per_group();

    // Assuming exponential utility function
    double ans = 0.0;
    for (const auto& n_infected: infected_per_group)
    {
        // Example exponential decay utility
        ans += std::exp(-n_infected / 100.0); 
    }

    return ans;
};

inline size_t ABM::get_n_agents() const {
    return agents.size();
};

inline size_t ABM::get_n_groups() const {
    return groups.size();
};

inline std::vector< double > ABM::get_n_infected_per_group() const {
    std::vector< double > n_infected_per_group;
    for (const auto& group: groups) {
        n_infected_per_group.push_back(group.get_n_infected());
    }
    return n_infected_per_group;
};

#endif