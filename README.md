# Quantum ABM

This repository implements a proof of concept model for using quantum computing embedded in Agent-Based Models.

## Simple SIS model with mixing

### Overview

The model implemented here corresponds to a simple Susceptible-Infected-Susceptible [SIS] Agent-Based Model with a mixing component. The model includes a population distributed between different groups where the contacts are based on the mixing matrix. Furthermore, at the end of each step, groups collectively decide whether to change their contact rates, based on the observed number of infected. To do so, they need to solve an optimization problem of the following form

```math
\vec a^* = \text{argmax}_{\vec a \in \{0,1\}^{|G|}} U(a, I)
```

Where $I = \sum_{g=1}^{|G|} I_g $ is the total number of infected in the system, the vector $\vec a$ is a binary vector of size $|G|$ indicating the decision of each group, and $U$ is the utility function with the following characteristics:

- Higher contact rate increases the utility through agents' economic activity.
- Lower contact rate increases the utility through reducing the transmissibility of the disease.

Ultimately, this can be thought as a government deciding what communities to quarantine or not. Closing the economy has negative effects to the society, so it is not always an easy decision.

### Implementation

The SIR model is implemented as a discrete-time ABM where the public health authorities decide what communities will go into a policy reducing contact rate. The model is governed by the probability of transmission $p_t$, contact rate $c$, and recovery rate $r$. Susceptible agents may become infected as a function of the number of contacts:

```math
\text{Pr}\left(\text{infected}|\text{contact $k$ infected agents}\right) = 1 - (1 - p_t)^k
```

The relevant number here is the way in which contacts are performed. Individuals will, on average, contact $\bar c$ other agents, yet, the number of agents to contact will change throughout the simulation as a function of how each community's contact rate are specified. Particularly, the number of contacts and agent from group $g$ will make from group $l$, $k(g,l)$, has a binomial distribution with the following parameters:

```math
K(g,l) \sim \text{Binomial}\left(I_l, \frac{c(1 - \delta)^{a_g}}{\sum_{g'}|G_g|(1 - \delta)^{a_{g'}}}\right)
```

Where $n = \sum_gS_g + \sum_gI_g$, and $\delta \in (0,1)$ is the fraction with which the contact rate is reduced when the policy is active. Thus, when $a_g = 0 \forall g$, then the probability parameter from the binomial reduces simply to $c/n$, yielding an average contact rate of $c$.

For this exercise, we will be assuming that the contact rates will not change as a function of how many agents are in the set of infected or susceptible agents. In a more standard model, a decreasing number of available agents would affect the contact rates.