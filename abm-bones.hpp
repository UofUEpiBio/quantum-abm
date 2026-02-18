#include<vector>
#include<string>
#include<random>
#include<iostream>
#include "utils.hpp"

#ifndef ABM_BONES_HPP
#define ABM_BONES_HPP

// Defining enums with the states
enum State {
    SUSCEPTIBLE = 0,
    INFECTED = 1,
    RECOVERED = 2
};

class Agent;
class ABM;

/**
 * @brief Population group
 */
class Group {
friend class ABM;
private:
    std::vector< Agent* > agents;
    bool restricted_contact = false;
    ABM* abm = nullptr;

public:
    void add_agent(Agent& agent);
    double get_n_infected() const;
    double sample_infected() const;
};

class Agent {
friend class ABM;
private:
    int state = State::SUSCEPTIBLE;
    int state_next = State::SUSCEPTIBLE;
    Group * group = nullptr;

public:
    int get_state() const;
    void set_state(int new_state);
    void set_group(Group& g);

};

class ABM {
private:
    std::vector<Agent> agents;
    std::vector<Group> groups;
    std::mt19937 rng;
    std::binomial_distribution<int> binomial_dist;
    std::uniform_real_distribution<double> uniform_dist;

    double prob_infection;
    double prob_recovery;
    double contact_rate;
    double contact_rate_reduction;
    int n_steps;
    bool verbose = true;

    void m_update_susceptible(Agent& agent);
    void m_update_infected(Agent& agent);
    void m_step(int step_number);

    int log_step = 0;
    void m_log(std::string msg);
    Matrix epidemic_curve;

    double m_calc_utility();

public:
    ABM(
        size_t n_agents = 1000,
        size_t n_groups = 10,
        double prob_infection = 0.1,
        double prob_recovery = 0.05,
        double contact_rate = 10.0,
        double contact_rate_reduction = 0.5,
        int n_steps = 100,
        size_t seed = 42
    );

    void print() const;
    void run();
    void toggle_verbose();

    double rbinom(double n, double p);
    double runif(double a = 0.0, double b = 1.0);

    Matrix& get_epidemic_curve();

    double get_contact_rate() const;
    double get_contact_rate_reduction() const;

    size_t get_n_agents() const;
    size_t get_n_groups() const;
    std::vector< double > get_n_infected_per_group() const;

};



#endif