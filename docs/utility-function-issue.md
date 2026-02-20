# GitHub Issue Draft: Add a binary policy utility for contact-rate controls

## Summary
The model needs a simple utility function that supports policy optimization over a **binary decision vector**:

- `a_g = 0`: no reduction in contact rate for group `g`.
- `a_g = 1`: contact rate is reduced for group `g`.

The utility should encode a tradeoff between:
1. **Economic/social activity** (higher when contacts remain high), and
2. **Public health outcomes** (higher when expected transmission is low).

## Why this issue matters
Current utility behavior is placeholder-like and does not explicitly expose the policy optimization structure

\[
\max_{\mathbf{a} \in \{0,1\}^{|G|}} U(\mathbf{a}, I)
\]

where `a` is the intervention vector and `I` is the infection state.

## Proposed utility (simple and optimization-ready)
For a policy vector `a`, define:

- Effective contact multiplier per group:
  - `m_g(a_g) = 1` if `a_g = 0`
  - `m_g(a_g) = (1 - δ)` if `a_g = 1`
- Average multiplier:
  - `m̄(a) = (1/|G|) Σ_g m_g(a_g)`
- Effective contact rate:
  - `c_eff(a) = c * m̄(a)`

Then utility:

\[
U(\mathbf{a}, I) = \omega_E \cdot \bar m(\mathbf{a})
- \omega_H \cdot \left[p_t \cdot c_{\mathrm{eff}}(\mathbf{a}) \cdot i \cdot (1-i)\right]
\]

where:
- `i = I/N` is the infected share,
- `ω_E` controls preference for activity,
- `ω_H` controls penalty for transmission risk.

This is intentionally lightweight and can be evaluated very fast for many candidate binary vectors.

## Acceptance criteria
- [ ] Add a C++ (standard library only) utility evaluator with binary policy input.
- [ ] Validate policy size and binary values.
- [ ] Keep the function independent from solver choice (enumeration, local search, QAOA, etc.).
- [ ] Document assumptions and tunable weights.

## Notes from literature (for context)
The objective structure follows common epidemic control formulations that trade economic activity vs infection burden/cost:

1. **Acemoglu et al. (2021)**, *Optimal targeted lockdowns in a multi-group SIR model* (NBER / AER Insights).
2. **Alvarez, Argente, Lippi (2021)**, *A simple planning problem for COVID-19 lockdown* (AER: Insights).
3. **Eichenbaum, Rebelo, Trabandt (2021)**, *The macroeconomics of epidemics* (Review of Financial Studies).
4. **Rowthorn, Laxminarayan, Gilligan (2009)**, *Optimal control of epidemics in metapopulations*.

These works vary in complexity, but all motivate balancing activity and transmission in objective design.
