# Linear Regression for Vehicle Price and Horsepower

This program fits a linear regression model where:

- `x` = vehicle price
- `y` = horsepower

It calculates the best-fit line for the dataset and predicts horsepower from a given vehicle price.

## Files

- `vehicle_price_horsepower.json`: Sample car dataset
- `linear_regression.py`: Regression analysis and prediction program
- `price_vs_horsepower_plot.svg`: Generated scatter plot with best-fit line after running the script

## Run

```powershell
python .\Regression\linear_regression.py
```

## Predict horsepower for a different price

```powershell
python .\Regression\linear_regression.py --price 65000
```
