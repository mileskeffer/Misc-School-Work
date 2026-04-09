# K-means Clustering for OBD2 Data

This program generates random OBD2 readings and applies K-means clustering to group vehicle behavior into practical operating categories.

## Problem setup

The generated samples simulate:

- normal operating range
- outside standard range with mild fault behavior
- outside standard range with severe fault behavior

Each sample contains:

1. RPM
2. Short-term fuel trim (`STFT`)
3. Manifold absolute pressure (`MAP`)
4. Engine temperature (`Temp`)

## Files

- `kmeans.py`: Data generation, seed selection, clustering loop, and terminal reporting

## Run

```powershell
python .\KMeans\kmeans.py
```

## Example with custom settings

```powershell
python .\KMeans\kmeans.py --samples 150 --clusters 3 --seed 11
```

The implementation follows the provided pseudocode by:

- generating seed points and a clustering radius
- iterating through centroids and assigning in-radius points to clusters
- recalculating centroid means
- tracking points that remain outliers
- stopping when centroid movement is below the allowed shift threshold or the loop limit is reached
