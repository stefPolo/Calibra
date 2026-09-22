## About Calibra

As we develop various models in order to draw meaningful conclusions about complex (dynamical) systems observed in reality, we also aim to calibrate that is optimise the models to the best extend.

Usually, we define some sort of "loss function" which describes the (aggregated) amount, with which our model deviates from the reality we observe and try to model and consequently understand. A optimisation algorithm of choice is then implemented to minimise the said loss function. 

In itself, it is _not_ an optimisation algorithm, but rather a diagnostics tool for model validation and evaluation. In plain terms, Calibra examines, if a generated set of parameters can truly be "trusted" with respect to their ability to describe the system we are dealing with.

This begs the question, to what extent the resulting calibration can be considered _reliable_. This especially involves questions like:
    
    - Has the algorithm converged to a stable and sufficiently good solution, or is there evidence of alternative minima? In other words, does there exist a more suitable minimum we should strive to achieve? 
    - Given the estimated parameters, what conclusions can we draw about their stability and invariance to (small) changes in the data?
    - Given the chosen model and the data, is there even a mathematically reasonable solution, yielding suitable parameters for the chosen model?

Calibra is developed in order to answer these kind of questions. For this, Calibra classifies the overall quality of a model based on the following categories regarding: convergence, fit, identifiability, uncertainty, stability, globality and, finally, model adequacy.

This distinguishes Calibra from established and well-documented optimization libraries such as _Ceres_ [1], _NLopt_ [2], _SciPy_ [3], you name them, which provide powerful algorithms for solving calibration problems but do not primarily aim to deliver a concise and _systematic_ assessment of the validity, stability, and reliability of the calibration procedure as a whole.

The following section describes the said categories in greater detail and elaborates on the architecture of Calibra along the way.

### 

## How is Calibra composed

Essentialy, calibra is composed of two programming languages: C++ and Python. Where C++ is responsible for the gritty, computationally intensive parts of the tool, such as bootstrapping, multiple starting points, matrix operations. The motivation here is also, to to provide sufficient performance even when diagnostics require a large number of repeated model evaluations.

Python, on the other hand, is mainly used for "lighter" calculations and visualisation purposes.

### Bibliography

[1] S. Agarwal, K. Mierle, and The Ceres Solver Team, Ceres Solver, version 2.2, Oct. 2023, software.

[2] S. G. Johnson, The NLopt Nonlinear-Optimization Package, software.

[3] P. Virtanen et al., “SciPy 1.0: Fundamental Algorithms for Scientific Computing in Python,” Nature Methods, vol. 17, pp. 261–272, 2020, doi: 10.1038/s41592-019-0686-2.