"""
plot_tof.py  —  DAMSA Time-of-Flight Histogram
Usage:
    python plot_tof.py tof_data.csv --bin-width 0.5
    python plot_tof.py tof_data.csv --bin-width 1.0
"""

import argparse
import sys
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import os

def main():
    parser = argparse.ArgumentParser(description="Plot TOF histogram from DAMSA simulation CSV")
    parser.add_argument("csv", help="Path to tof_data.csv")
    parser.add_argument("--bin-width", type=float, required=True,
                        help="Width of each time bin in ns (e.g. 0.5)")
    args = parser.parse_args()

    # ── Load data ─────────────────────────────────────────────────────────────
    try:
        df = pd.read_csv(args.csv)
    except FileNotFoundError:
        sys.exit(f"Error: could not find '{args.csv}'")

    photons  = df[df["particle"] == "photon" ]["global_time_ns"].values
    neutrons = df[df["particle"] == "neutron"]["global_time_ns"].values

    if len(photons) == 0 and len(neutrons) == 0:
        sys.exit("Error: no photon or neutron rows found in the CSV.")

    # ── Build bins ────────────────────────────────────────────────────────────
    all_times = np.concatenate([photons, neutrons])
    t_min = 0.0
    t_max = np.ceil(all_times.max() / args.bin_width) * args.bin_width
    bins  = np.arange(t_min, t_max + args.bin_width, args.bin_width)

    # Count particles per bin
    p_counts, _ = np.histogram(photons,  bins=bins)
    n_counts, _ = np.histogram(neutrons, bins=bins)

    bin_centers = (bins[:-1] + bins[1:]) / 2
    half_width  = args.bin_width / 2 * 0.45   # slight gap between the two bars

    # ── Plot ──────────────────────────────────────────────────────────────────
    fig, ax = plt.subplots(figsize=(12, 6))

    ax.bar(bin_centers - half_width, p_counts, width=half_width * 2,
           color="steelblue", label=f"Photons (n={len(photons)})",  alpha=0.85)
    ax.bar(bin_centers + half_width, n_counts, width=half_width * 2,
           color="tomato",    label=f"Neutrons (n={len(neutrons)})", alpha=0.85)

    ax.set_xlabel("Global arrival time (ns)", fontsize=13)
    ax.set_ylabel("Counts", fontsize=13)
    ax.set_title(f"DAMSA Time-of-Flight at CsI entrance  |  bin width = {args.bin_width} ns",
                 fontsize=14)
    ax.legend(fontsize=12)
    ax.set_xlim(t_min, t_max)
    ax.grid(axis="y", linestyle="--", alpha=0.4)

    # out = "tof_histogram.png"
    os.makedirs("build", exist_ok=True)
    out = os.path.join("build", "tof_histogram.png")
    plt.tight_layout()
    plt.savefig(out, dpi=150)
    print(f"Saved {out}")
    plt.show()

if __name__ == "__main__":
    main()
