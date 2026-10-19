#!/usr/bin/env python3
"""Merge control regions, update histograms, and run Combine for a mass point."""

import argparse
import shutil
import subprocess
import sys
from pathlib import Path


PROJECT_ROOT = Path(__file__).resolve().parents[2]
PYTHON_TOOLS = PROJECT_ROOT / "tools" / "python"
DEFAULT_ANALYSIS = "cms_2605_13614"
HISTOGRAM_FILE_TEMPLATE = "histograms/{mass_point}/histograms.root"


def format_mass(mass):
    return f"{mass:g}"


def resolve_path(path):
    path = Path(path)
    return path if path.is_absolute() else PROJECT_ROOT / path


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("mH", type=float, help="Higgs mass")
    parser.add_argument("mA", type=float, help="pseudoscalar mass")
    parser.add_argument(
        "--analysis",
        default=DEFAULT_ANALYSIS,
        help=f"analysis tag in control-file names (default: {DEFAULT_ANALYSIS})",
    )
    parser.add_argument(
        "--control-folder",
        help=(
            "folder containing control-region files "
            "(default: results/combined_files_<mH>_<mA>)"
        ),
    )
    parser.add_argument(
        "--histogram-file",
        help=(
            "ROOT histogram file to update "
            "(default: histograms/<mass-point>/histograms.root)"
        ),
    )
    parser.add_argument(
        "--cards",
        default="combined_cards.txt",
        help="Combine datacard (default: combined_cards.txt)",
    )
    parser.add_argument(
        "--r-max",
        default="10",
        help="maximum signal strength passed to Combine (default: 10)",
    )
    parser.add_argument(
        "--combine-executable",
        default="combine",
        help="Combine executable (default: combine)",
    )
    args = parser.parse_args()

    mass_point = f"mH{args.mH:.1f}_mA{args.mA:.1f}"
    control_folder = resolve_path(
        args.control_folder
        or (
            "results/combined_files_"
            f"{format_mass(args.mH)}_{format_mass(args.mA)}"
        )
    )
    histogram_file = resolve_path(
        args.histogram_file
        or HISTOGRAM_FILE_TEMPLATE.format(mass_point=mass_point)
    )
    cards = resolve_path(args.cards)
    control_suffix = f"_{args.analysis}_control.dat"

    if not control_folder.is_dir():
        parser.error(f"Control-region folder not found: {control_folder}")
    if not any(
        path.is_file() and path.name.endswith(control_suffix)
        for path in control_folder.iterdir()
    ):
        parser.error(
            f"No files ending in {control_suffix!r} found in {control_folder}"
        )
    if not histogram_file.is_file():
        parser.error(f"Histogram ROOT file not found: {histogram_file}")
    if not cards.is_file():
        parser.error(f"Combine datacard not found: {cards}")

    combine_executable = shutil.which(args.combine_executable)
    if combine_executable is None:
        parser.error(f"Combine executable not found: {args.combine_executable}")

    combined_control_file = (
        control_folder / f"{args.analysis}_combined_control.txt"
    )
    commands = [
        [
            sys.executable,
            str(PYTHON_TOOLS / "combine_control.py"),
            str(control_folder),
            args.analysis,
        ],
        [
            sys.executable,
            str(PYTHON_TOOLS / "combine_wrapper.py"),
            str(args.mH),
            str(args.mA),
            "--control-file",
            str(combined_control_file),
            "--histogram-file",
            str(histogram_file),
        ],
        [
            combine_executable,
            str(cards),
            "-M",
            "AsymptoticLimits",
            "--keyword-value",
            f"MA={args.mA:.1f}",
            "--keyword-value",
            f"MH={args.mH:.1f}",
            "--strictBounds",
            "--rMax",
            args.r_max,
        ],
    ]

    step_names = (
        "Merging control regions",
        "Replacing nominal and systematic histograms",
        "Running Combine",
    )
    for name, command in zip(step_names, commands):
        print(f"\n{name}: {' '.join(command)}", flush=True)
        subprocess.run(command, cwd=PROJECT_ROOT, check=True)


if __name__ == "__main__":
    main()
