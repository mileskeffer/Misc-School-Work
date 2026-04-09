from __future__ import annotations

import argparse
import json
import math
import random
import sys
from dataclasses import dataclass
from pathlib import Path
from typing import Dict, Iterable, List, Sequence, Tuple


EPSILON = 1e-12


@dataclass(frozen=True)
class GaussianEmission:
    mean: Tuple[float, ...]
    std: Tuple[float, ...]

    def log_probability(self, observation: Sequence[float], epsilon: float = EPSILON) -> float:
        if len(observation) != len(self.mean):
            raise ValueError(
                "Observation length does not match emission dimension: "
                f"{len(observation)} != {len(self.mean)}"
            )

        log_probability = 0.0
        for value, mean, std in zip(observation, self.mean, self.std):
            sigma = max(float(std), epsilon)
            log_coefficient = -math.log(math.sqrt(2.0 * math.pi) * sigma)
            exponent = -((float(value) - float(mean)) ** 2) / (2.0 * sigma**2)
            log_probability += log_coefficient + exponent
        return log_probability

    def probability(self, observation: Sequence[float], epsilon: float = EPSILON) -> float:
        log_probability = self.log_probability(observation, epsilon)
        if log_probability < math.log(sys.float_info.min):
            return 0.0
        return math.exp(log_probability)


@dataclass
class HiddenMarkovModel:
    states: List[str]
    initial_probabilities: List[float]
    transition_matrix: List[List[float]]
    emission_models: Dict[str, GaussianEmission]

    def __post_init__(self) -> None:
        state_count = len(self.states)
        if state_count == 0:
            raise ValueError("The model must contain at least one state.")

        if len(self.initial_probabilities) != state_count:
            raise ValueError("Initial probability vector length must match the number of states.")

        if len(self.transition_matrix) != state_count:
            raise ValueError("Transition matrix row count must match the number of states.")

        for row in self.transition_matrix:
            if len(row) != state_count:
                raise ValueError("Transition matrix must be square.")

        for state in self.states:
            if state not in self.emission_models:
                raise ValueError(f"Missing emission model for state '{state}'.")

        emission_lengths = {
            len(self.emission_models[state].mean)
            for state in self.states
        }
        if len(emission_lengths) != 1:
            raise ValueError("All states must share the same emission dimension.")

    @property
    def feature_count(self) -> int:
        return len(self.emission_models[self.states[0]].mean)

    def valid_emissions(self, observation_sequence: Sequence[Sequence[float]]) -> bool:
        return all(len(observation) == self.feature_count for observation in observation_sequence)

    def emission_probability(
        self, state: str, observation: Sequence[float], epsilon: float = EPSILON
    ) -> float:
        return self.emission_models[state].probability(observation, epsilon)

    def log_emission_probability(
        self, state: str, observation: Sequence[float], epsilon: float = EPSILON
    ) -> float:
        return self.emission_models[state].log_probability(observation, epsilon)

    def emission_set(
        self, observation: Sequence[float], epsilon: float = EPSILON
    ) -> List[str]:
        return [
            state
            for state in self.states
            if math.isfinite(self.log_emission_probability(state, observation, epsilon))
        ]

    def valid_transition(self, state_sequence: Sequence[str], epsilon: float = EPSILON) -> bool:
        if not state_sequence:
            return False

        first_index = self.states.index(state_sequence[0])
        if self.initial_probabilities[first_index] < epsilon:
            return False

        for current_state, next_state in zip(state_sequence, state_sequence[1:]):
            i = self.states.index(current_state)
            j = self.states.index(next_state)
            if self.transition_matrix[i][j] < epsilon:
                return False
        return True

    def calculate_probability(
        self,
        state_sequence: Sequence[str],
        observation_sequence: Sequence[Sequence[float]],
        epsilon: float = EPSILON,
    ) -> float:
        if len(state_sequence) != len(observation_sequence):
            raise ValueError("State and observation sequences must have the same length.")
        if not state_sequence:
            return 0.0

        log_probability = self.calculate_log_probability(
            state_sequence,
            observation_sequence,
            epsilon,
        )
        if log_probability < math.log(sys.float_info.min):
            return 0.0
        return math.exp(log_probability)

    def calculate_log_probability(
        self,
        state_sequence: Sequence[str],
        observation_sequence: Sequence[Sequence[float]],
        epsilon: float = EPSILON,
    ) -> float:
        if len(state_sequence) != len(observation_sequence):
            raise ValueError("State and observation sequences must have the same length.")
        if not state_sequence:
            return float("-inf")

        first_index = self.states.index(state_sequence[0])
        log_probability = math.log(max(self.initial_probabilities[first_index], epsilon))
        log_probability += self.log_emission_probability(
            state_sequence[0], observation_sequence[0], epsilon
        )

        for previous_state, current_state, observation in zip(
            state_sequence, state_sequence[1:], observation_sequence[1:]
        ):
            i = self.states.index(previous_state)
            j = self.states.index(current_state)
            log_probability += math.log(max(self.transition_matrix[i][j], epsilon))
            log_probability += self.log_emission_probability(current_state, observation, epsilon)
        return log_probability

    def decode_path(
        self,
        observation_sequence: Sequence[Sequence[float]],
        epsilon: float = EPSILON,
    ) -> Tuple[List[str], float]:
        if not observation_sequence:
            return [], 0.0
        if not self.valid_emissions(observation_sequence):
            raise ValueError(
                "Each observation must contain "
                f"{self.feature_count} OBD2 features to match the model."
            )

        candidate_states = [self.emission_set(observation, epsilon) for observation in observation_sequence]
        delta: List[Dict[str, float]] = []
        backpointer: List[Dict[str, str | None]] = []

        first_scores: Dict[str, float] = {}
        first_previous: Dict[str, str | None] = {}
        first_observation = observation_sequence[0]
        for state in candidate_states[0]:
            index = self.states.index(state)
            first_scores[state] = (
                math.log(max(self.initial_probabilities[index], epsilon))
                + self.log_emission_probability(state, first_observation, epsilon)
            )
            first_previous[state] = None
        delta.append(first_scores)
        backpointer.append(first_previous)

        for time_index in range(1, len(observation_sequence)):
            observation = observation_sequence[time_index]
            time_scores: Dict[str, float] = {}
            time_previous: Dict[str, str | None] = {}

            for current_state in candidate_states[time_index]:
                current_index = self.states.index(current_state)
                emission_log_probability = self.log_emission_probability(
                    current_state, observation, epsilon
                )

                best_probability = float("-inf")
                best_previous_state: str | None = None
                for previous_state, previous_probability in delta[time_index - 1].items():
                    previous_index = self.states.index(previous_state)
                    transition_probability = self.transition_matrix[previous_index][current_index]
                    candidate_probability = (
                        previous_probability
                        + math.log(max(transition_probability, epsilon))
                        + emission_log_probability
                    )
                    if candidate_probability > best_probability:
                        best_probability = candidate_probability
                        best_previous_state = previous_state

                time_scores[current_state] = best_probability
                time_previous[current_state] = best_previous_state

            delta.append(time_scores)
            backpointer.append(time_previous)

        final_state = max(delta[-1], key=delta[-1].get)
        final_probability = delta[-1][final_state]

        best_path = [final_state]
        for time_index in range(len(observation_sequence) - 1, 0, -1):
            previous_state = backpointer[time_index][best_path[-1]]
            if previous_state is None:
                break
            best_path.append(previous_state)
        best_path.reverse()

        return best_path, final_probability


def normalize(values: Iterable[float]) -> List[float]:
    values = [float(value) for value in values]
    total = sum(values)
    if total <= 0.0:
        raise ValueError("Values must sum to a positive number before normalization.")
    return [value / total for value in values]


def load_model(config_path: Path) -> Tuple[HiddenMarkovModel, List[List[float]]]:
    data = json.loads(config_path.read_text(encoding="utf-8"))

    states = data["states"]
    transition_matrix = data["A"]
    initial_probabilities = data["pi"]
    observations = data.get("observations", [])

    emission_models = {
        state: GaussianEmission(
            mean=tuple(model["mean"]),
            std=tuple(model["std"]),
        )
        for state, model in data["emissions"].items()
    }

    model = HiddenMarkovModel(
        states=states,
        initial_probabilities=initial_probabilities,
        transition_matrix=transition_matrix,
        emission_models=emission_models,
    )
    return model, observations


def generate_random_model(
    state_count: int,
    feature_count: int,
    observation_count: int,
    seed: int | None = None,
) -> Tuple[HiddenMarkovModel, List[List[float]]]:
    if state_count <= 0 or feature_count <= 0:
        raise ValueError("State count and feature count must be positive.")
    if observation_count < 0:
        raise ValueError("Observation count cannot be negative.")

    rng = random.Random(seed)
    states = [f"State{i + 1}" for i in range(state_count)]
    initial_probabilities = normalize(rng.random() for _ in range(state_count))
    transition_matrix = [
        normalize(rng.random() for _ in range(state_count)) for _ in range(state_count)
    ]

    emission_models: Dict[str, GaussianEmission] = {}
    observations: List[List[float]] = []
    state_means: List[List[float]] = []
    state_stds: List[List[float]] = []

    for state in states:
        mean = [rng.uniform(0.0, 100.0) for _ in range(feature_count)]
        std = [rng.uniform(1.0, 15.0) for _ in range(feature_count)]
        emission_models[state] = GaussianEmission(tuple(mean), tuple(std))
        state_means.append(mean)
        state_stds.append(std)

    for _ in range(observation_count):
        source_index = rng.randrange(state_count)
        observation = [
            rng.gauss(state_means[source_index][feature_index], state_stds[source_index][feature_index])
            for feature_index in range(feature_count)
        ]
        observations.append(observation)

    model = HiddenMarkovModel(
        states=states,
        initial_probabilities=initial_probabilities,
        transition_matrix=transition_matrix,
        emission_models=emission_models,
    )
    return model, observations


def format_observation(observation: Sequence[float]) -> str:
    labels = ["RPM", "BoostPSI", "MAF_gps", "IgnitionRetard_deg"]
    if len(observation) == len(labels):
        return ", ".join(
            f"{label}={value:.2f}" if isinstance(value, float) else f"{label}={value}"
            for label, value in zip(labels, observation)
        )
    return ", ".join(f"x{i + 1}={value:.2f}" for i, value in enumerate(observation))


def print_decoding(
    model: HiddenMarkovModel,
    observations: Sequence[Sequence[float]],
    best_path: Sequence[str],
    best_log_probability: float,
) -> None:
    print("Most probable hidden-state path:")
    for index, (state, observation) in enumerate(zip(best_path, observations), start=1):
        state_probability = model.emission_probability(state, observation)
        emission_display = f"{state_probability:.6e}" if state_probability > 0.0 else "underflow"
        print(
            f"{index:02d}. {state:<12} | emission={emission_display} | "
            f"{format_observation(observation)}"
        )

    if best_log_probability < math.log(sys.float_info.min):
        probability_display = "underflow"
    else:
        probability_display = f"{math.exp(best_log_probability):.6e}"
    print(f"\nLog path probability: {best_log_probability:.6f}")
    print(f"Path probability: {probability_display}")


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description=(
            "Decode the most probable HMM path for OBD2 observations. "
            "The model can be loaded from JSON or generated randomly."
        )
    )
    parser.add_argument(
        "--config",
        type=Path,
        default=Path(__file__).with_name("hmm.json"),
        help="Path to a JSON model file. Defaults to HMM/hmm.json.",
    )
    parser.add_argument(
        "--random",
        action="store_true",
        help="Generate a random graph instead of reading the model from JSON.",
    )
    parser.add_argument("--states", type=int, default=4, help="State count for a random graph.")
    parser.add_argument("--features", type=int, default=4, help="Feature count for a random graph.")
    parser.add_argument(
        "--observations",
        type=int,
        default=10,
        help="Observation count for a random graph.",
    )
    parser.add_argument("--seed", type=int, default=None, help="Random seed for repeatable graphs.")
    return parser.parse_args()


def main() -> None:
    args = parse_args()

    if args.random:
        model, observations = generate_random_model(
            state_count=args.states,
            feature_count=args.features,
            observation_count=args.observations,
            seed=args.seed,
        )
    else:
        model, observations = load_model(args.config)

    best_path, best_probability = model.decode_path(observations)
    print_decoding(model, observations, best_path, best_probability)


if __name__ == "__main__":
    main()
