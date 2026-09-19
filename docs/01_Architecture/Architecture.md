## The main architecture of Calibra

Calibra is able to describe and solve various calibration problems modularly, followed by a diagnostic evaluation of the calibration result and procedure. In essence, the architecture distinguishes between the _definition_, _evaluation_ and the _optimisation_ itself.

### 1. CalibrationProblem

An arbitrary calibration problem, $\mathcal{P}$ is generally represented as the composition of the following components:

\[
\mathcal P = (f, J, X_{\mathrm{cal}},y_{\mathrm{cal}},\theta_0,\Theta,\mathcal{C}, s)
\]

where:

* \(f\): is the Model to be calibrated,
* \(J\): the loss function to be minimised or maximised accordigly,
* \(X_{\mathrm{cal}}\): Data used for calibrating the model,
* \(y_{\mathrm{cal}}\): observed data, i.e "labels",
* \(\theta_0\in\mathbb R^p\): initial vector of parameters of interest,
* \(\Theta\subseteq\mathbb R^p\): the admissable parameterspace in conjunction with the predefinded boundaries,
* \(\mathcal C\): additional constraints involving equations and inequalities,
* \(s\): the direction of optimisation, that is, either minimisation or maximisation.

Therefore, the class `CalibrationProblem` solely describes, **what** should be calibrated and serves as the "blueprint" for the underlying optimisation problem at hand.

### 2. Main abstractions

We abstract the main mathematical components as Interfaces. So that,

* `IModel`, \(f\), represents a model of the general form:

\[
    f:\mathbb R^m\times\mathbb R^p\rightarrow\mathbb R.
\]

This might be, for instance, a simple, linear regression or more sophisticated models depending on the circumstances.

* `IObjectiveFunction`, \(J\), represents the loss function.

* `IConstraint`, \(\mathcal {C}\) instanciates general constraints of the form

\[
    g_i(\theta)\leq 0
\]

or

\[
    h_j(\theta) = 0.
\]

As a result, specific models, loss functions and constraints can be implemented and interchanged independently. Thus serving the modularity of Calibra.

### 3. Data and parameters

For the time being, only real-valued data is permitted. Specifically:

\[
    X_{\mathrm{cal}}\in\mathbb R^{n\times m},
    \qquad
    y_{\mathrm{cal}}\in\mathbb R^n,
    \qquad
    \theta\in\mathbb R^p.
\]

So, we intent to seperate calibration and validation data for now. Of course, the validation data does not influence the estimation of \(\hat\theta\). Instead, its purpose lies ion the diagnosis of calibration results.

### 4. ProblemEvaluator

The mentioned components of `CalibrationProblem`, i.e the members of $\mathcal{P}$, shall not depend on one another. Therefore, a distinct object, `ProblemEvaluator`, orchestrates the coordination between the components. In this way, the components can be linked together. Consequently, a calibration process can be illustrated as follows:

\[
    \theta \longrightarrow f(X;\theta) \longrightarrow \hat y \longrightarrow J(\hat y,y).
\]

Ultimately, the `ProblemEvaluator` gathers the said components of $\mathcal{P}$, calls the respective model `IModel` and loss function `IObjectiveFunction` and finally yields its output. This output is mainly the object of interest, 

\[
    E_{\mathcal P}:\mathbb R^p\rightarrow\mathbb R,
\]

which is afterwards fed to the `Optimiser` outlined below.

### 5. Optimiser

The `Optimiser` is seperated from the internal model and data structure. It adresses and governs the behaviour and structure of the underlying `ObjectiveFunction`, given the calibrated parameter vector \(\theta\):

$$
\theta\mapsto E_{\mathcal P}(\theta)
$$

under the consideration of specified constraints mentioned above. However, how these are treated in general, remains the hands of the chosen optimisationalgorithm.

### 6. CalibrationResult

Finally, we intend to save the results from calibrations seperately. Thus, a `CalibrationResult` at least holds on to the following objects: The calibrated parameter vector \(\theta\), the final value of the objective/loss function \(J(\hat\theta)\), status of convergence, reason of termination and the number of iterations.

Darauf aufbauend sollen später diagnostische Größen ergänzt werden, beispielsweise Gradientennorm, Sensitivität gegenüber Daten oder Startwerten, lokale Krümmung und Konditionierung.

### Architecture overview

```text
CalibrationProblem
│
├── IModel
├── IObjectiveFunction
├── CalibrationData
├── InitialParameters
├── ParameterBounds
├── Constraints
└── OptimizationDirection
        │
        ▼
ProblemEvaluator
        │
        ▼
Optimizer
        │
        ▼
CalibrationResult
        │
        ▼
Diagnostics
```

To briefly sum up, the essential train of though regarding the architecture of Calibra is the following:

* `CalibrationProblem` _describes_ the problem at hand.
* `ProblemEvaluator` _evaluates_ the problem for a particular parameter vector.
* `Optimizer` _seeks_ a fitting parameter vector.
* `CalibrationResult` _saves_ the optimisation result.
* Finally, `Diagnostics` _assesses_ the quality, stabilty and reliability of the calibration.