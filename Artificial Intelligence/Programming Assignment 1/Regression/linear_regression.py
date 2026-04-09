from __future__ import annotations

import argparse
import json
from dataclasses import dataclass
from pathlib import Path
from statistics import mean
from typing import Iterable, List, Sequence, Tuple


@dataclass(frozen=True)
class DataPoint:
    x: float
    y: float


@dataclass(frozen=True)
class LinearRegressionModel:
    slope: float
    intercept: float
    r_squared: float

    def predict(self, x_value: float) -> float:
        return self.slope * x_value + self.intercept

    def equation(self) -> str:
        return f"horsepower = {self.slope:.6f} * price + {self.intercept:.6f}"


def load_vehicle_data(path: Path) -> Tuple[List[DataPoint], str, str]:
    raw = json.loads(path.read_text(encoding="utf-8"))
    points = [
        DataPoint(x=float(item["price"]), y=float(item["horsepower"]))
        for item in raw["data"]
    ]
    if len(points) < 2:
        raise ValueError("At least two data points are required for linear regression.")
    return points, raw.get("x_label", "X"), raw.get("y_label", "Y")


def fit_linear_regression(points: Sequence[DataPoint]) -> LinearRegressionModel:
    x_values = [point.x for point in points]
    y_values = [point.y for point in points]
    x_mean = mean(x_values)
    y_mean = mean(y_values)

    numerator = sum((x - x_mean) * (y - y_mean) for x, y in zip(x_values, y_values))
    denominator = sum((x - x_mean) ** 2 for x in x_values)
    if denominator == 0:
        raise ValueError("Regression cannot be computed when all X values are identical.")

    slope = numerator / denominator
    intercept = y_mean - slope * x_mean

    predicted = [slope * x + intercept for x in x_values]
    residual_sum_squares = sum((actual - estimate) ** 2 for actual, estimate in zip(y_values, predicted))
    total_sum_squares = sum((actual - y_mean) ** 2 for actual in y_values)
    r_squared = 1.0 if total_sum_squares == 0 else 1.0 - (residual_sum_squares / total_sum_squares)

    return LinearRegressionModel(slope=slope, intercept=intercept, r_squared=r_squared)


def create_svg_plot(
    points: Sequence[DataPoint],
    model: LinearRegressionModel,
    output_path: Path,
    x_label: str,
    y_label: str,
) -> None:
    width = 900
    height = 600
    margin_left = 90
    margin_right = 40
    margin_top = 50
    margin_bottom = 90

    plot_width = width - margin_left - margin_right
    plot_height = height - margin_top - margin_bottom

    x_values = [point.x for point in points]
    y_values = [point.y for point in points]

    x_min = min(x_values)
    x_max = max(x_values)
    y_min = min(y_values)
    y_max = max(y_values)

    x_padding = max((x_max - x_min) * 0.08, 1.0)
    y_padding = max((y_max - y_min) * 0.12, 1.0)

    x_min -= x_padding
    x_max += x_padding
    y_min -= y_padding
    y_max += y_padding

    def scale_x(value: float) -> float:
        return margin_left + ((value - x_min) / (x_max - x_min)) * plot_width

    def scale_y(value: float) -> float:
        return height - margin_bottom - ((value - y_min) / (y_max - y_min)) * plot_height

    tick_count = 5
    x_ticks = [x_min + ((x_max - x_min) / tick_count) * index for index in range(tick_count + 1)]
    y_ticks = [y_min + ((y_max - y_min) / tick_count) * index for index in range(tick_count + 1)]

    line_start_x = min(x_values)
    line_end_x = max(x_values)
    line_start_y = model.predict(line_start_x)
    line_end_y = model.predict(line_end_x)

    point_elements = "\n".join(
        (
            f'<circle cx="{scale_x(point.x):.2f}" cy="{scale_y(point.y):.2f}" r="5" '
            'fill="#2563eb" fill-opacity="0.85" stroke="#0f172a" stroke-width="1.5" />'
        )
        for point in points
    )

    x_grid = "\n".join(
        (
            f'<line x1="{scale_x(value):.2f}" y1="{margin_top}" '
            f'x2="{scale_x(value):.2f}" y2="{height - margin_bottom}" '
            'stroke="#cbd5e1" stroke-width="1" stroke-dasharray="4 6" />'
        )
        for value in x_ticks
    )
    y_grid = "\n".join(
        (
            f'<line x1="{margin_left}" y1="{scale_y(value):.2f}" '
            f'x2="{width - margin_right}" y2="{scale_y(value):.2f}" '
            'stroke="#cbd5e1" stroke-width="1" stroke-dasharray="4 6" />'
        )
        for value in y_ticks
    )

    x_labels = "\n".join(
        (
            f'<text x="{scale_x(value):.2f}" y="{height - margin_bottom + 28}" '
            'text-anchor="middle" font-size="14" fill="#1e293b">'
            f'${value:,.0f}</text>'
        )
        for value in x_ticks
    )
    y_labels = "\n".join(
        (
            f'<text x="{margin_left - 16}" y="{scale_y(value) + 5:.2f}" '
            'text-anchor="end" font-size="14" fill="#1e293b">'
            f'{value:.0f}</text>'
        )
        for value in y_ticks
    )

    svg = f"""<svg xmlns="http://www.w3.org/2000/svg" width="{width}" height="{height}" viewBox="0 0 {width} {height}">
  <rect width="100%" height="100%" fill="#f8fafc" />
  <text x="{width / 2:.0f}" y="30" text-anchor="middle" font-size="24" font-weight="700" fill="#0f172a">
    Vehicle Price vs Horsepower Linear Regression
  </text>
  <text x="{width / 2:.0f}" y="56" text-anchor="middle" font-size="15" fill="#334155">
    {model.equation()} | R^2 = {model.r_squared:.4f}
  </text>

  {x_grid}
  {y_grid}

  <line x1="{margin_left}" y1="{height - margin_bottom}" x2="{width - margin_right}" y2="{height - margin_bottom}" stroke="#0f172a" stroke-width="2" />
  <line x1="{margin_left}" y1="{margin_top}" x2="{margin_left}" y2="{height - margin_bottom}" stroke="#0f172a" stroke-width="2" />

  <line
    x1="{scale_x(line_start_x):.2f}"
    y1="{scale_y(line_start_y):.2f}"
    x2="{scale_x(line_end_x):.2f}"
    y2="{scale_y(line_end_y):.2f}"
    stroke="#dc2626"
    stroke-width="3"
  />

  {point_elements}
  {x_labels}
  {y_labels}

  <text x="{width / 2:.0f}" y="{height - 22}" text-anchor="middle" font-size="18" fill="#0f172a">{x_label}</text>
  <text x="24" y="{height / 2:.0f}" text-anchor="middle" font-size="18" fill="#0f172a" transform="rotate(-90, 24, {height / 2:.0f})">{y_label}</text>
</svg>
"""

    output_path.write_text(svg, encoding="utf-8")


def summarize_dataset(points: Iterable[DataPoint]) -> str:
    count = sum(1 for _ in points)
    return f"Loaded {count} vehicle samples."


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description=(
            "Fit a linear regression model using vehicle price as the independent variable "
            "and horsepower as the dependent variable."
        )
    )
    parser.add_argument(
        "--data",
        type=Path,
        default=Path(__file__).with_name("vehicle_price_horsepower.json"),
        help="Path to the JSON dataset.",
    )
    parser.add_argument(
        "--price",
        type=float,
        default=50000.0,
        help="Vehicle price to use for horsepower prediction.",
    )
    parser.add_argument(
        "--plot",
        type=Path,
        default=Path(__file__).with_name("price_vs_horsepower_plot.svg"),
        help="Output path for the regression plot.",
    )
    return parser.parse_args()


def main() -> None:
    args = parse_args()
    points, x_label, y_label = load_vehicle_data(args.data)
    model = fit_linear_regression(points)
    predicted_horsepower = model.predict(args.price)

    create_svg_plot(points, model, args.plot, x_label, y_label)

    print(summarize_dataset(points))
    print(f"Regression equation: {model.equation()}")
    print(f"Coefficient of determination (R^2): {model.r_squared:.4f}")
    print(f"Predicted horsepower at price ${args.price:,.2f}: {predicted_horsepower:.2f}")
    print(f"Plot written to: {args.plot}")
    print("\nInterpretation:")
    print(
        "This model estimates how changes in vehicle price are associated with horsepower. "
        "A positive slope means higher-priced vehicles in this dataset tend to have more horsepower."
    )


if __name__ == "__main__":
    main()
