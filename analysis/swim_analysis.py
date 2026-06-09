#!/usr/bin/env python3

import csv
import math
import argparse

try:
    import matplotlib.pyplot as plt
except ImportError:
    plt = None


def load_csv(filename):
    rows = []
    with open(filename, newline="") as csvfile:
        reader = csv.DictReader(csvfile)
        for row in reader:
            if not row:
                continue
            rows.append(row)
    return rows


def parse_float(value, default=0.0):
    try:
        return float(value)
    except (ValueError, TypeError):
        return default


def compute_roll_from_accel(ax, ay, az):
    return math.degrees(math.atan2(ay, az))


def moving_average(data, window=5):
    if window <= 1:
        return data[:]
    half = window // 2
    result = []
    for i in range(len(data)):
        low = max(0, i - half)
        high = min(len(data), i + half + 1)
        result.append(sum(data[low:high]) / float(high - low))
    return result


def detect_peaks(values, min_distance=10, threshold=0.1):
    peaks = []
    last_peak_index = -min_distance
    average = sum(values) / len(values) if values else 0.0
    for i in range(1, len(values) - 1):
        if values[i] > values[i - 1] and values[i] > values[i + 1]:
            if values[i] > average + threshold * abs(average if average != 0 else 1.0):
                if i - last_peak_index >= min_distance:
                    peaks.append(i)
                    last_peak_index = i
    return peaks


def summarize_session(times, ax, roll):
    duration = times[-1] - times[0] if len(times) > 1 else 0.0
    peak_indices = detect_peaks(ax, min_distance=10, threshold=0.3)
    stroke_count = len(peak_indices)
    stroke_rate = stroke_count / duration * 60.0 if duration > 0 else 0.0
    print(f"Samples: {len(times)}")
    print(f"Duration: {duration:.2f} s")
    print(f"Estimated strokes: {stroke_count}")
    print(f"Approximate stroke rate: {stroke_rate:.1f} spm")
    if roll:
        print(f"Roll range: {min(roll):.1f} to {max(roll):.1f} deg")


def plot_data(times, ax, roll, peak_indices):
    if plt is None:
        print("matplotlib not installed; skipping plots")
        return

    fig, (ax1, ax2) = plt.subplots(2, 1, sharex=True, figsize=(10, 6))
    ax1.plot(times, ax, label="ax")
    ax1.scatter([times[i] for i in peak_indices], [ax[i] for i in peak_indices], color="red", label="stroke peaks")
    ax1.set_ylabel("ax (g)")
    ax1.legend()

    ax2.plot(times, roll, label="roll")
    ax2.set_ylabel("roll (deg)")
    ax2.set_xlabel("time (s)")
    ax2.legend()

    plt.tight_layout()
    plt.show()


def main():
    parser = argparse.ArgumentParser(description="Analyze TriSense swim CSV data.")
    parser.add_argument("file", help="CSV file to analyze")
    args = parser.parse_args()

    rows = load_csv(args.file)
    if not rows:
        print("No data found in CSV.")
        return

    times = []
    ax = []
    roll = []
    for row in rows:
        t = parse_float(row.get("time_ms"), 0.0) / 1000.0
        ax_val = parse_float(row.get("ax_g"), 0.0)
        ay_val = parse_float(row.get("ay_g"), 0.0)
        az_val = parse_float(row.get("az_g"), 0.0)
        roll_val = parse_float(row.get("roll_deg", ""), None)

        if roll_val is None:
            roll_val = compute_roll_from_accel(ax_val, ay_val, az_val)

        times.append(t)
        ax.append(ax_val)
        roll.append(roll_val)

    if len(times) < 2:
        print("Too little data to analyze.")
        return

    ax_filtered = moving_average(ax, window=7)
    roll_filtered = moving_average(roll, window=7)
    peak_indices = detect_peaks(ax_filtered, min_distance=12, threshold=0.25)

    summarize_session(times, ax_filtered, roll_filtered)
    plot_data(times, ax_filtered, roll_filtered, peak_indices)


if __name__ == "__main__":
    main()
