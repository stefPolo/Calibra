## About Calibra

As we develop various models in order to draw meaningful conclusions about complex (dynamical) systems observed in reality, we also aim to calibrate that is optimise the models to the best extend.

Usually, we define some sort of "loss function" which describes the (aggregated) amount, with which our model deviates from the reality we observe and try to model and consequently understand. A optimisation algorithm of choice is then implemented to minimise the said loss function. 

In itself, it is _not_ an optimisation algorithm, but rather a diagnostics tool for model validation and evaluation. In plain terms, Calibra examines, if a generated set of parameters can truly be "trusted" with respect to their ability to describe the system we are dealing with.

This begs the question, to what extent the resulting calibration can be considered _reliable_. Calibra is developed in order to answer this question. In particular, Calibra decomposes a calibration problem as such into three main components regarding: the numerical layer, the calibration itself and the model layer. In the same order, this especially involves questions like:
    
    - Has the algorithm converged to a stable and sufficiently good solution, or is there evidence of alternative minima? In other words, does there exist a more suitable minimum we should strive to achieve? 
    - Given the estimated parameters, what conclusions can we draw about their stability and invariance to (small) changes in the data?
    - Given the chosen model and the data, is there even a mathematically reasonable solution, yielding suitable parameters for the chosen model?

Therefore, Calibra classifies the overall quality of a model based on the categories regarding convergence, fit, identifiability, uncertainty, stability, globality and, finally, model adequacy.

This distinguishes Calibra from established and well-documented optimisation libraries such as _Ceres_ [1], _NLopt_ [2], _SciPy_ [3], you name them, which provide powerful algorithms for solving calibration problems but do not primarily aim to deliver a concise and _systematic_ assessment of the validity, stability, and reliability of the calibration procedure as a whole.

The following section describes Calibra in greater detail and elaborates on its architecture along the way.

## Calibration Quality Framework

### The Calibration Problem

We begin by discussing the main components that constitute a calibration problem and form the foundation for any implementation using Calibra. We then turn to the concrete construction of such a problem within the library.

#### Data
 
Initially, we are set to describe a relationship observed in reality for which we collect relevant data. While gathering the data, we distinguish between relevant variables or "features" (input) which adequately describe the outcome of interest (output). 

For the implementation in Calibra, the input data generally is expected in a matrix form. In particular:

$$
\mathbf{X} =
\begin{bmatrix}
x_{11} & x_{12} & \cdots & x_{1m} \\
x_{21} & x_{22} & \cdots & x_{2m} \\
\vdots & \vdots & \ddots & \vdots \\
x_{n1} & x_{n2} & \cdots & x_{nm}
\end{bmatrix}
\in \mathbb{R}^{n \times m},
$$

where the number of columns corresponds to the number of $m$ features and the rows to the number of $n$ observations. Each entry $x_{ij}$ represents the value a particular observation takes.

#### Model

In order to describe the relationship represented by the input data $\mathbf{X}$, we choose a specific model, that is, a function $f$ that maps $\mathbf{X}$, together with a given set of parameters $\theta$, to the outcome of interest $\hat{\mathbf{y}}$. Given $\mathbf{X}\in\mathbb{R}^{n\times m}$, the corresponding model output is collected in an $n$-dimensional vector. Formally,

$$
f(\mathbf{X},\theta)=\hat{\mathbf{y}}\in\mathbb{R}^n.
$$

Each component $\hat{y}_i$ therefore represents the prediction associated with the corresponding row $\mathbf{x}_i$ of $\mathbf{X}$.

Calibra provides a general interface for models, represented by the `class IModel`:

```cpp
class IModel {

    public:
        virtual std::vector<double> evaluate(
            const std::vector<std::vector<double>>& X,
            const std::vector<double>& parameters
        ) const = 0;

        virtual std::size_t input_dimension() const = 0;
        virtual std::size_t parameter_dimension() const = 0;
        virtual std::size_t output_dimension() const = 0;

        virtual ~IModel() = default;
};
```

The additional methods provided by the class, namely `IModel::input_dimension()`, `IModel::parameter_dimension()`, and `IModel::output_dimension()`, serve primarily to ensure dimensional consistency between the model and the objects passed to it. In particular, they specify the expected dimensions of an individual input, the parameter vector, and the model output, respectively. For instance, `IModel::evaluate()` may reject an input whose dimension does not match `IModel::input_dimension()`. Likewise, a parameter vector of a dimension different from that specified by `IModel::parameter_dimension()` constitutes an invalid model evaluation.

#### Admissible Parameter Spaces

When formulating a calibration problem, it is often useful to restrict the components of the parameter vector $\theta$ to specific ranges. In Calibra, such restrictions are represented by the `class ParameterSpace`:

```cpp
class ParameterSpace 
{
    public:
        ParameterSpace(
            std::vector<double>& lower_bounds,
            std::vector<double>& upper_bounds
        );
        
        std::size_t dimension() const;

        bool contains(
            const std::vector<double>& parameters
        ) const;

        const std::vector<double>& lower_bounds() const;
        const std::vector<double>& upper_bounds() const;
    
    private:
        std::vector<double>& lower_bounds_;
        std::vector<double>& upper_bounds_;
};
```

Using `ParameterSpace::lower_bounds()` and `ParameterSpace::upper_bounds()`, we specify vectors containing the lower bounds $l_j$ and upper bounds $u_j$ for each component of $\theta$. Both vectors must, naturally, be of matching dimension.

For a one-dimensional parameter vector, the resulting parameter space is simply a closed interval. In two dimensions, it forms a rectangle; in higher dimensions, the corresponding Cartesian product of intervals. Calibra therefore defines the admissible parameter space $\Theta$ as

$$
\Theta = \prod_{j=1}^m [l_j, u_j] \times \cdots \times [l_m, u_m].
$$

Accordingly, every admissible parameter vector must satisfy

$$
\theta \in \Theta.
$$

Membership in the admissible parameter space is checked using `ParameterSpace::contains()`, while its dimension can be accessed through `ParameterSpace::dimension()`.


-> Zielfunktion (IObjectiveFunction.hpp) -> Kalibrierungsproblem (CalibrationProblem.hpp).

## How is Calibra composed

Essentialy, calibra is composed of two programming languages: C++ and Python. Where C++ is responsible for the gritty, computationally intensive parts of the tool, such as bootstrapping, multiple starting points, matrix operations. The motivation here is also, to to provide sufficient performance even when diagnostics require a large number of repeated model evaluations.

Python, on the other hand, is mainly used for "lighter" calculations and visualisation purposes.

### Bibliography

[1] S. Agarwal, K. Mierle, and The Ceres Solver Team, Ceres Solver, version 2.2, Oct. 2023, software.

[2] S. G. Johnson, The NLopt Nonlinear-Optimization Package, software.

[3] P. Virtanen et al., “SciPy 1.0: Fundamental Algorithms for Scientific Computing in Python,” Nature Methods, vol. 17, pp. 261–272, 2020, doi: 10.1038/s41592-019-0686-2.