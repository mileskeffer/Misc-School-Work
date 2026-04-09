from __future__ import annotations

import argparse
import math
import random
from dataclasses import dataclass
from typing import Dict, Iterable, List, Sequence, Tuple


def generate_gaussian(mean: float, std: float) -> float:
    return random.gauss(mean, std)


def generate_obd2_data(num_samples: int) -> List[List[float]]:
    data: List[List[float]] = []

    for _ in range(num_samples):
        r = random.random()

        # Normal (60%)
        if r < 0.6:
            sample = [
                generate_gaussian(2200, 400),  # RPM
                generate_gaussian(0, 3),       # STFT
                generate_gaussian(45, 8),      # MAP
                generate_gaussian(90, 5),      # Temp
            ]

        # Mild Fault (25%)
        elif r < 0.85:
            sample = [
                generate_gaussian(2400, 500),
                generate_gaussian(8, 5),
                generate_gaussian(55, 10),
                generate_gaussian(100, 7),
            ]

        # Severe Fault (15%)
        else:
            sample = [
                generate_gaussian(1800, 700),
                generate_gaussian(18, 8),
                generate_gaussian(65, 12),
                generate_gaussian(110, 10),
            ]

        data.append(sample)

    return data


FEATURE_NAMES = ["RPM", "STFT", "MAP", "Temp"]
CLUSTER_FEATURE_WEIGHTS = [0.35, 1.4, 1.1, 1.1]


@dataclass
class ClusterResult:
    centroids: List[List[float]]
    clusters: List[List[List[float]]]
    outliers: List[List[float]]
    iterations: int
    radius: float
    stabilized: bool


def euclidean_distance(
    point_a: Sequence[float],
    point_b: Sequence[float],
    weights: Sequence[float] | None = None,
) -> float:
    if weights is None:
        weights = [1.0] * len(point_a)
    return math.sqrt(sum(weight * ((a - b) ** 2) for a, b, weight in zip(point_a, point_b, weights)))


def vector_mean(points: Sequence[Sequence[float]]) -> List[float]:
    dimensions = len(points[0])
    return [
        sum(point[index] for point in points) / len(points)
        for index in range(dimensions)
    ]


def describe_point(point: Sequence[float]) -> str:
    return ", ".join(
        f"{name}={value:.2f}" for name, value in zip(FEATURE_NAMES, point)
    )


def normalize_points(points: Sequence[Sequence[float]]) -> Tuple[List[List[float]], List[Tuple[float, float]]]:
    dimensions = len(points[0])
    bounds: List[Tuple[float, float]] = []
    normalized_points: List[List[float]] = []

    for index in range(dimensions):
        values = [point[index] for point in points]
        lower = min(values)
        upper = max(values)
        bounds.append((lower, upper))

    for point in points:
        normalized_point: List[float] = []
        for value, (lower, upper) in zip(point, bounds):
            spread = upper - lower
            if spread == 0:
                normalized_point.append(0.0)
            else:
                normalized_point.append((value - lower) / spread)
        normalized_points.append(normalized_point)

    return normalized_points, bounds


def denormalize_point(point: Sequence[float], bounds: Sequence[Tuple[float, float]]) -> List[float]:
    values: List[float] = []
    for value, (lower, upper) in zip(point, bounds):
        values.append(lower + value * (upper - lower))
    return values


def estimate_radius(points: Sequence[Sequence[float]], cluster_count: int) -> float:
    dimensions = len(points[0])
    return math.sqrt(dimensions) / max(cluster_count, 1)


def generate_seed_points(
    points: Sequence[Sequence[float]],
    cluster_count: int,
) -> Tuple[List[List[float]], float, List[Tuple[float, float]]]:
    if cluster_count <= 0:
        raise ValueError("Number of clusters must be positive.")
    if len(points) < cluster_count:
        raise ValueError("Need at least as many points as clusters.")

    normalized_points, bounds = normalize_points(points)
    macroblocks: Dict[Tuple[int, ...], List[List[float]]] = {}

    for point in normalized_points:
        block_index = []
        for value in point:
            index = min(int(value * cluster_count), cluster_count - 1)
            block_index.append(index)
        macroblocks.setdefault(tuple(block_index), []).append(point)

    densest_blocks = sorted(
        macroblocks.items(),
        key=lambda entry: len(entry[1]),
        reverse=True,
    )

    seeds: List[List[float]] = []
    for _, block_points in densest_blocks:
        candidate = vector_mean(block_points)
        if not seeds:
            seeds.append(candidate)
        else:
            min_distance = min(
                euclidean_distance(candidate, seed, CLUSTER_FEATURE_WEIGHTS) for seed in seeds
            )
            if min_distance >= 0.35:
                seeds.append(candidate)
        if len(seeds) == cluster_count:
            break

    if len(seeds) < cluster_count:
        remaining_points = normalized_points[:]
        random.shuffle(remaining_points)
        for point in remaining_points:
            if not seeds:
                seeds.append(point[:])
            else:
                min_distance = min(
                    euclidean_distance(point, seed, CLUSTER_FEATURE_WEIGHTS) for seed in seeds
                )
                if min_distance >= 0.2:
                    seeds.append(point[:])
            if len(seeds) == cluster_count:
                break

    while len(seeds) < cluster_count:
        seeds.append(normalized_points[len(seeds)][:])

    radius = estimate_radius(normalized_points, cluster_count)
    return seeds, radius, bounds


def k_means_clustering(
    points: Sequence[Sequence[float]],
    cluster_count: int,
    max_loops: int = 25,
    max_shift: float = 0.02,
) -> ClusterResult:
    normalized_points, bounds = normalize_points(points)
    centroids, radius, _ = generate_seed_points(points, cluster_count)
    iteration = 0
    stabilized = False
    latest_clusters_normalized: List[List[List[float]]] = [[] for _ in range(cluster_count)]
    latest_outliers_normalized: List[List[float]] = []

    print(f"\n****** Starting Iterations ******")
    
    while iteration < max_loops and not stabilized:
        outliers = [list(point) for point in normalized_points]
        clusters: List[List[List[float]]] = [[] for _ in range(cluster_count)]

        for point in normalized_points:
            distances = [
                euclidean_distance(centroid, point, CLUSTER_FEATURE_WEIGHTS)
                for centroid in centroids
            ]
            nearest_index = min(range(cluster_count), key=lambda index: distances[index])
            nearest_distance = distances[nearest_index]

            if nearest_distance < radius:
                clusters[nearest_index].append(list(point))
                if list(point) in outliers:
                    outliers.remove(list(point))

        new_centroids: List[List[float]] = []
        print(f"\nIteration {iteration + 1}")
        for index, cluster in enumerate(clusters, start=1):
            if cluster:
                centroid_normalized = vector_mean(cluster)
            else:
                centroid_normalized = centroids[index - 1][:]
            new_centroids.append(centroid_normalized)
            centroid = denormalize_point(centroid_normalized, bounds)
            print(
                f" |   cluster #{index}, new centroid: ({', '.join(f'{value:.2f}' for value in centroid)})"
            )

        print(f"Current outliers: {len(outliers)}")

        stabilized = True
        for old_centroid, new_centroid in zip(centroids, new_centroids):
            centroid_shift = euclidean_distance(
                old_centroid,
                new_centroid,
                CLUSTER_FEATURE_WEIGHTS,
            )
            if centroid_shift > max_shift:
                stabilized = False

        centroids = new_centroids
        latest_clusters_normalized = clusters
        latest_outliers_normalized = outliers
        iteration += 1

    final_centroids = [denormalize_point(centroid, bounds) for centroid in centroids]
    final_clusters = [
        [denormalize_point(point, bounds) for point in cluster]
        for cluster in latest_clusters_normalized
    ]
    final_outliers = [denormalize_point(point, bounds) for point in latest_outliers_normalized]

    return ClusterResult(
        centroids=final_centroids,
        clusters=final_clusters,
        outliers=final_outliers,
        iterations=iteration,
        radius=radius,
        stabilized=stabilized,
    )


def classify_cluster(centroid: Sequence[float]) -> str:
    stft = centroid[1]
    map_value = centroid[2]
    temp = centroid[3]

    if stft < 4 and map_value < 50 and temp < 96:
        return "Normal operating range"
    if stft < 12 and map_value < 60 and temp < 105:
        return "Outside range: mild fault tendency"
    return "Outside range: severe fault tendency"


def print_run_summary(result: ClusterResult, points: Sequence[Sequence[float]]) -> None:
    print("\nK-means clustering summary")
    print(f" |  Samples analyzed: {len(points)}")
    print(f" |  Clusters requested: {len(result.centroids)}")
    print(f" |  Radius used: {result.radius:.4f}")
    print(f" |  Iterations completed: {result.iterations}")
    print(f" |  Stabilized: {result.stabilized}")


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description=(
            "Run K-means clustering on generated OBD2 vehicle data to separate "
            "normal behavior from readings outside the standard operating range."
        )
    )
    parser.add_argument("--samples", type=int, default=120, help="Number of OBD2 samples to generate.")
    parser.add_argument("--clusters", type=int, default=3, help="Number of clusters.")
    parser.add_argument("--max-loops", type=int, default=25, help="Maximum clustering iterations.")
    parser.add_argument(
        "--max-shift",
        type=float,
        default=0.02,
        help="Maximum centroid shift allowed before the algorithm is considered stable.",
    )
    parser.add_argument(
        "--seed",
        type=int,
        default=None,
        help="Optional random seed for repeatable data generation.",
    )
    return parser.parse_args()


def main() -> None:
    args = parse_args()
    if args.seed is not None:
        random.seed(args.seed)

    points = generate_obd2_data(args.samples)
    result = k_means_clustering(
        points=points,
        cluster_count=args.clusters,
        max_loops=args.max_loops,
        max_shift=args.max_shift,
    )

    print("\n------------ Final Clusters and Outliers ------------")
    for index, (centroid, cluster) in enumerate(zip(result.centroids, result.clusters), start=1):
        print(
            f"Cluster {index}: size={len(cluster)}, centroid=({', '.join(f'{value:.2f}' for value in centroid)})"
        )
        print(f"Interpretation: {classify_cluster(centroid)}")

    print(f"Outliers: {len(result.outliers)}")
    if result.outliers:
        print("Sample outliers:")
        for outlier in result.outliers[:5]:
            print(f"  {describe_point(outlier)}")

    print_run_summary(result, points)


if __name__ == "__main__":
    main()
