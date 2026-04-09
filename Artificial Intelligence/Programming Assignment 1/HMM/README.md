# Hidden Markov Model for Turbo OBD2 Data

This folder contains a Python implementation of a hidden Markov model that follows the screenshot algorithm's structure while using Gaussian emissions for continuous OBD2 readings.

## Files

- `hmm.json`: Turbo-engine HMM definition and the provided OBD2 observation sequence.
- `hmm.py`: Loads the model, validates observations, and decodes the most probable hidden-state path.

## Turbo-engine interpretation

Each observation is a 4-value OBD2 sample:

1. Engine RPM
2. Boost pressure
3. Mass airflow
4. Ignition retard

The hidden states in `hmm.json` describe likely operating conditions:

- `Normal`
- `Boosting`
- `MildFault`
- `SevereFault`

## Run the provided model

```powershell
python .\HMM\hmm.py
```

## Generate a random graph

```powershell
python .\HMM\hmm.py --random --states 5 --features 4 --observations 12 --seed 7
```

This satisfies the assignment requirement that the graph size is parameterized and that graph weights can either be read from a file or generated randomly.
