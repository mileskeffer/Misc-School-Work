# AI Project Collection

This repository contains three small AI and data analysis programs built around practical vehicle and automotive-style datasets.

## Projects

### `HMM`

Hidden Markov model for turbo-engine OBD2 readings.

- Reads a model from `HMM/hmm.json`
- Decodes the most probable hidden-state path from an observation sequence
- Can also generate a random parameterized HMM

Run:

```powershell
python .\HMM\hmm.py
```

### `KMeans`

K-means clustering for generated OBD2 vehicle data.

- Generates random OBD2 samples
- Groups readings into operating clusters such as normal, mild fault, and severe fault
- Prints the clustering results and final summary in the terminal

Run:

```powershell
python .\KMeans\kmeans.py
```

### `Regression`

Linear regression for vehicle price versus horsepower.

- Uses `x = vehicle price`
- Uses `y = horsepower`
- Fits the best-fit line and predicts horsepower from a given price

Run:

```powershell
python .\Regression\linear_regression.py
```

## Folder Structure

```text
ai/
├── HMM/
├── KMeans/
└── Regression/
```

## Notes

- Each project has its own `README.md` with more detail.
- The automotive examples use OBD2-style sensor values and vehicle performance data.
- Most scripts can be run directly from this root folder with Python.
