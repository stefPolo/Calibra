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
\Theta = \prod_{j=1}^m [l_j, u_j].
$$

Accordingly, every admissible parameter vector must satisfy

$$
\theta \in \Theta.
$$

Membership in the admissible parameter space is checked using `ParameterSpace::contains()`, while its dimension can be accessed through `ParameterSpace::dimension()`.

#### Objective/Loss Functions

Given the output generated by `IModel::evaluate()`, usually we aim to compare this output with observed data (labels), say $y$. The labels at hand should be of the same dimension as our output vector $\hat{y} \in \mathbb{R}^n$. This allows us to assess the possible deviations our model produces compared to the observed reality. These deviations serve as the input for the objective or loss function $\mathcal{L}$.

The objective function generally can take many forms, such as $L_1$-, $L_2$-norms or specific user-defined ones depending on the circumstances and problem at hand. To adress the plethora of possible objective functions, Calibra implements these as interfaces in the `class IObjectiveFunction`:

```cpp

class IObjectiveFunction 
{
    public:
        virtual double evaluate(
            const std::vector<double>& predictions,
            const std::vector<double>& labels            
        ) const = 0;
        virtual ~IObjectiveFunction() = default;
};
```

So, from an arbitrary `IObjectiveFunction`, we only demand its input to be predictions and corresponding labels along with it generally being evaluable. This is a very general framework from which a wide range of _specific_ objective functions can inherit from. One such example of a objective function inheriting from `class IObjectiveFunction` is the `class MeanSquaredError`:

```cpp
class MeanSquaredError : public IObjectiveFunction
{
    public:
        double evaluate(
            const std::vector<double>& predictions,
            const std::vector<double>& labels
        ) const override;
};
```
Thus, any objective function initialised takes the from
$$
    \mathcal{L}(f(\mathbf{X},\theta), \theta) = \mathcal{L}(\hat{y}, y) \in \mathbb{R}.
$$

#### Initialising and Evaluating Calibration Problems

The preceeding subsections regarding the needed data, model, parameter space/vector and objective function form the core arguments needed to initialise an arbitrary `class CalibrationProblem`:

```cpp
enum class OptimisationDirection
{
    minimise, maximise
};

class CalibrationProblem
{
    public:
        CalibrationProblem(
            IModel& model,
            ParameterSpace& paramSpace,
            std::vector<double>& initParameters,
            IObjectiveFunction& objectiveFunction,
            const std::vector<std::vector<double>>& calibrationData,
            const std::vector<double>& labels,
            OptimisationDirection direction = OptimisationDirection::minimise
        )
            : model_(model)
            , paramSpace_(paramSpace)
            , initParameters_(initParameters)
            , objectiveFunction_(objectiveFunction)
            , calibrationData_(calibrationData)
            , labels_(labels)
            , direction_(direction)
        {
            /*
            Initialise a CalibrationProblem only if:
            1. Parameter vector is contained by an admissible parameter space.
            2. Dimensions of parameter vector and model parameters must match.
            3. Dimensions of model input must match number of features.
            4. Number of observations must match number of labels.
            */
        }
}
```
Of course, each calibration problem starts off with an initial parameter vector, say $\theta_0 \in \Theta$ which we aim to calibrate. Depending on the calibration problem and thus on the objective function of interest, we may either minimise or maximise $\mathcal{L}$. The more common choice is to `OptimisationDirection::minimise`. Still, for methods such as maximum likelihood methods we need to pass `OptimisationDirection::maximise` instead.

Having initialised the `CalibrationProblem` we may call `CalibrationProblem::evaluate_objective()`, specifically:

```cpp
double evaluate_objective(const std::vector<double>& params) const
{
            /*
            Check if passed parameters are a member of the initialised parameter space.
            If so, proceed to evaluating the objective function.
            */
            if (!paramSpace_.contains(params))
            {
                throw std::invalid_argument(
                    "Initialised parameter space does not contain passed parameters."
                );
            }

            auto predictions = model_.evaluate(calibrationData_, params);
            double loss = objectiveFunction_.evaluate(predictions, labels_);

            return loss;
}

```

Here, the underlying model generates the predictions, which are then passed to the objective function computing the loss, that is, the aggregated deviations of predictions relative to the labels. Depending on our `OptimisationDirection` with $\mathrm{dir} \in \{\mathrm{min}, \mathrm{max}\}, the calibration problem implemented in Calibra can be summarised as follows:

$$
\underset{\theta \in \Theta}{\mathrm{arg\,min}}
\;\mathcal{L}\!\left(f(\mathbf{X},\theta),\mathbf{y}\right).
$$

This problem is tackled by various, well-documented optimisers already existing and is therefore, again, not the main contribution of Calibra. We treat Calibra essentially as an inference problem regarding both the calibrated parameter vector and overall suitability of the model.

#### Calibration Results

According to the preceding statement, Calibra’s responsibilities do not end with an (optimally) calibrated parameter vector, say $\theta^{\star}$. The actual approach -- and indeed its extension relative to the capabilities of existing libraries and tools -- begins with the thorough investigation of $\theta^{\star}$ to determine whether the model in question is adequately specified and consequently delivers reliable results.

This investigation also concerns the optimisation algorithm itself. You see, as long as our objective function of choice is not convex, we might only find some strictly _local_ minimum/maximum [4]. This means that there exists some region around our presumably "optimal" parameter vector $\theta^{\star}$, say $\theta^{\dagger} \in \Theta$, such that for arbitrary entries $x_{ij}$ in that region we have

$$
\mathcal{L}(f(\cdot), \theta^{\dagger}) < \mathcal{L}(f(\cdot), \theta^{\star}).
$$

So, even if the algorithm converges successfully, maybe even to a definite global optimum, this does provide any implications about the model adequacy and performance on itself. This is because there might exist a wide range of parameter vectors, which make our established model yield the same results with respect to the observed system concerned. Therefore, along the optimisation, this introduces an additional aspect concerning the issue of parameter identifiabilty [5]. In plain terms, if we have the choice between two sets of parameters, again $\theta^{\star}$ and $\theta^{\dagger}$, such that

$$
f(x_{ij}, \theta^{\star}) \approx f(x_{ij}, \theta^{\dagger}),
$$

for all $x_{ij}$, which parameter vector do we declare representative with respect to the system we are dealing with and why? So, the operational success of an optimisation algorithm does not guarantee a _unique_ parameter vector and certainly does not imply respective optimisation _quality_ associated with certain model performance. As a result, the processes of calibrating and validating a specific model are fundamentaly different [6].

### Assessing Calibration Quality

Meanwhile, the specific utility of Calibra is also becoming clear; it will be used in particular as part of the model validation methodology, based on some specific calibration results obtained. It remains to answer which insights are needed, and arguably necessary, to judge on the overall adequacy and reliability of model results, given the components introduced in the preceeding sections. To this end, we propose breaking this concept down into the following relevant segments:

    - General algorithm convergence.

    - Model fit to labels.

    - Parameter identifiability, uncertainty, stability and globality.

    - Model adequacy.

The model adequacy should, however, be a logical deduction from the preceding points and thus form the basis for advice.

Below, we present the main methodology of Calibra in the following way: We first provide some basic intuition on why the aspect under discussion is relevant, followed by a brief (visual) example and specific implementation in the architecture of Calibra. However, before we focus on the methodology the implementation of various optimisers is outlined.

#### Inclusion of Optimisation Algorithms in Calibra


#### Algorithm Convergence


#### Model Fit


#### Identifiability, uncertainty, stability and globality.


#### Adequacy



It remains to answer which insights are needed, and arguably necessary, to judge on the overall adequacy and reliability of model results, given the components introduced in the preceeding sections. To this end, we propose breaking this concept down into the following relevant segments:







Nevertheless, the inclusion and employment of specific optimisers in the context of Calibra is described in the following sections.

 

### Optimisers


## How is Calibra composed

Essentialy, calibra is composed of two programming languages: C++ and Python. Where C++ is responsible for the gritty, computationally intensive parts of the tool, such as bootstrapping, multiple starting points, matrix operations. The motivation here is also, to to provide sufficient performance even when diagnostics require a large number of repeated model evaluations.

Python, on the other hand, is mainly used for "lighter" calculations and visualisation purposes.

### Bibliography

[1] S. Agarwal, K. Mierle, and The Ceres Solver Team, Ceres Solver, version 2.2, Oct. 2023, software.

[2] S. G. Johnson, The NLopt Nonlinear-Optimization Package, software.

[3] P. Virtanen et al., “SciPy 1.0: Fundamental Algorithms for Scientific Computing in Python,” Nature Methods, vol. 17, pp. 261–272, 2020, doi: 10.1038/s41592-019-0686-2.

[4] J. Nocedal and S. J. Wright, Numerical Optimization, 2nd ed. New York, NY, USA: Springer, 2006, doi: 10.1007/978-0-387-40065-5.

[5] N. N. Lam, P. D. Docherty, and R. Murray, “Practical identifiability of parametrised models: A review of benefits and limitations of various approaches,” Mathematics and Computers in Simulation, vol. 199, pp. 202–216, Sep. 2022, doi: 10.1016/j.matcom.2022.03.020.

[6] T. G. Trucano, L. P. Swiler, T. Igusa, W. L. Oberkampf, and M. Pilch, “Calibration, validation, and sensitivity analysis: What’s what,” Reliability Engineering & System Safety, vol. 91, nos. 10–11, pp. 1331–1357, Oct.–Nov. 2006, doi: 10.1016/j.ress.2005.11.031.