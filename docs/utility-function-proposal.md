# Proposal: Simple binary policy utility for contact-rate regulation

## Goal
Define a utility function for policy vectors
\(\mathbf{a} \in \{0,1\}^{|G|}\), where each entry indicates whether contact reduction is active in a group.

- `a_g = 0`: no reduction in contacts.
- `a_g = 1`: reduced contacts.

The function should reward social/economic interaction while penalizing transmission risk.

## Recommended utility

Let:
- baseline contact rate be `c`,
- reduction factor be `(1 - δ)` with `δ in (0,1)`,
- infected share be `i = I/N`.

Define:
- `m_g(a_g) = 1` if `a_g=0`, else `(1-δ)`.
- `m̄(a) = (1/|G|) Σ_g m_g(a_g)`.
- `c_eff(a) = c * m̄(a)`.

Then:

\[
U(\mathbf{a}, I) = \omega_E\,\bar m(\mathbf{a})
- \omega_H\,\big[p_t\,c_{eff}(\mathbf{a})\,i\,(1-i)\big]
\]

Interpretation:
- First term: activity reward (larger with more contacts).
- Second term: expected one-step transmission-risk proxy.

## Why this form
- Binary-input friendly and lightweight.
- Differentiation-free and solver-agnostic.
- Encodes the intended policy tradeoff in one line.

## Literature motivation
- Acemoglu et al. (2021): targeted interventions by group.
- Alvarez et al. (2021): planner objective balancing output and deaths/infections.
- Eichenbaum et al. (2021): macro-epidemiological tradeoffs between activity and infection dynamics.

## Current implementation status
- Added `ABM::evaluate_policy_utility(const std::vector<int>& policy) const`.
- Added validation for policy length and binary entries.
- Internal logging utility now evaluates this policy-form utility for the current group restrictions.

Optimization/search over policy vectors is intentionally left for future work.
