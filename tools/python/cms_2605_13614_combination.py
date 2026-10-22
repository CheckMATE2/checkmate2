#!/usr/bin/env python3
"""Combine CheckMATE runs of cms_2605_13614 and compute limits with Combine.

The control-region yields of all given runs (e.g. the 13 TeV and the 13.6 TeV
run of the same model) are summed, filled into the signal histograms of the
CMS datacards and passed to Combine (AsymptoticLimits). Without --mH/--mA,
the mass points of cms_2605_13614.h on a 10 GeV grid in both masses are
processed, or all of them with --all.

The histograms in the datacards folder are never modified: for every mass
point they are copied to the output folder, updated there and removed after
Combine has run (unless --keep-histograms is given).
"""

import argparse
import re
import shutil
import subprocess
import sys
from pathlib import Path

from info import Info
from combine_control import combine_files, write_combined
from combine_wrapper import mass_point_name, update_histograms


ANALYSIS = "cms_2605_13614"
# Matches the control files of cms_2605_13614 and of its variants for other
# energies, e.g. hhh_cms_2605_13614_control.dat, hhh_cms_2605_13614_13p6_control.dat
CONTROL_FILE_GLOB = f"*_{ANALYSIS}*_control.dat"
COMBINED_CONTROL_FILE = f"{ANALYSIS}_combined_control.txt"
CARDS_FILE = "combined_cards.txt"
LIMITS_FILE = "limits.txt"
# Without --all, only mass points with both masses on this grid are evaluated
DEFAULT_GRID_STEP = 10.0

MASS_LIST_RE = re.compile(
    r"^\s*std::vector<std::pair<float,\s*float>>\s+mHmA\s*=\s*\{(.*?)\};",
    re.DOTALL | re.MULTILINE,
)
MASS_PAIR_RE = re.compile(r"\{\s*([\d.]+)f?\s*,\s*([\d.]+)f?\s*\}")
LIMIT_RE = re.compile(
    r"^(?:Observed Limit|Expected\s+(?P<quantile>[\d.]+)%): r < (?P<value>\S+)",
    re.MULTILINE,
)
LIMIT_COLUMNS = ("obs", "exp2.5", "exp16", "exp50", "exp84", "exp97.5")


def read_mass_points(header):
    """Return the (mH, mA) pairs of the mHmA list in the analysis header."""
    text = Path(header).read_text(encoding="utf-8")
    match = MASS_LIST_RE.search(text)
    if not match:
        raise ValueError(f"{header}: mHmA mass list not found")
    points = []
    for m_h, m_a in MASS_PAIR_RE.findall(match.group(1)):
        point = (float(m_h), float(m_a))
        if point not in points:
            points.append(point)
    if not points:
        raise ValueError(f"{header}: mHmA mass list is empty")
    return points


def find_control_files(results_dir, runs):
    files = []
    for run in runs:
        analysis_dir = results_dir / run / "analysis"
        if not analysis_dir.is_dir():
            raise ValueError(f"Run folder not found: {analysis_dir}")
        run_files = sorted(analysis_dir.glob(CONTROL_FILE_GLOB))
        if not run_files:
            raise ValueError(
                f"No files matching {CONTROL_FILE_GLOB!r} found in {analysis_dir}"
            )
        files.extend(run_files)
    return files


def parse_limits(output):
    limits = {}
    for match in LIMIT_RE.finditer(output):
        quantile = match.group("quantile")
        key = f"exp{float(quantile):g}" if quantile else "obs"
        limits[key] = match.group("value")
    return limits


def run_combine(combine_executable, output_dir, m_h, m_a, r_max):
    mass_point = mass_point_name(m_h, m_a)
    command = [
        combine_executable,
        CARDS_FILE,
        "-M",
        "AsymptoticLimits",
        "--keyword-value",
        f"MA={m_a:.1f}",
        "--keyword-value",
        f"MH={m_h:.1f}",
        "--strictBounds",
        "--rMax",
        r_max,
        "-n",
        f"_{mass_point}",
    ]
    log_file = output_dir / "logs" / f"{mass_point}.log"
    log_file.parent.mkdir(exist_ok=True)
    print(f"Running Combine: {' '.join(command)}", flush=True)
    process = subprocess.run(
        command,
        cwd=output_dir,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        universal_newlines=True,
    )
    log_file.write_text(process.stdout, encoding="utf-8")
    if process.returncode != 0:
        raise RuntimeError(
            f"Combine failed with exit code {process.returncode}, see {log_file}"
        )
    limits = parse_limits(process.stdout)
    if not limits:
        raise RuntimeError(f"No limits found in the Combine output, see {log_file}")
    return limits


def read_limits(path):
    """Return the rows of an existing limits file, keyed by (mH, mA)."""
    rows = {}
    if not path.is_file():
        return rows
    with path.open(encoding="utf-8") as limits_file:
        next(limits_file, None)
        for line in limits_file:
            fields = line.split()
            if len(fields) != len(LIMIT_COLUMNS) + 3:
                continue
            limits = {
                column: value
                for column, value in zip(LIMIT_COLUMNS, fields[2:-1])
                if value != "-"
            }
            rows[(float(fields[0]), float(fields[1]))] = (limits, fields[-1])
    return rows


def write_limits(path, rows):
    with path.open("w", encoding="utf-8") as limits_file:
        limits_file.write(
            "\t".join(("mH", "mA") + LIMIT_COLUMNS + ("status",)) + "\n"
        )
        for (m_h, m_a), (limits, status) in sorted(rows.items()):
            values = [limits.get(column, "-") for column in LIMIT_COLUMNS]
            limits_file.write(
                "\t".join([f"{m_h:g}", f"{m_a:g}"] + values + [status]) + "\n"
            )


def main():
    Info.fill_standard_paths_and_files()
    results_dir = Path(Info.paths["results"])
    default_datacards = Path(Info.paths["data"]) / ANALYSIS / "datacards"

    parser = argparse.ArgumentParser(
        description=__doc__,
        formatter_class=argparse.RawDescriptionHelpFormatter,
    )
    parser.add_argument(
        "runs",
        nargs="+",
        metavar="run",
        help=f"names of the CheckMATE runs to combine (folders in {results_dir})",
    )
    parser.add_argument("--mH", type=float, help="Higgs mass of a single mass point")
    parser.add_argument(
        "--mA", type=float, help="pseudoscalar mass of a single mass point"
    )
    parser.add_argument(
        "--all",
        action="store_true",
        help=(
            "evaluate all mass points of the analysis instead of those on a "
            f"{DEFAULT_GRID_STEP:g} GeV grid"
        ),
    )
    parser.add_argument(
        "--datacards",
        type=Path,
        default=default_datacards,
        help=f"Combine datacards folder (default: {default_datacards})",
    )
    parser.add_argument(
        "--output",
        type=Path,
        help=(
            "output folder "
            f"(default: {results_dir}/combined_<run1>_<run2>...)"
        ),
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
    parser.add_argument(
        "--prepare-only",
        action="store_true",
        help="only prepare the histograms, do not run Combine",
    )
    parser.add_argument(
        "--keep-histograms",
        action="store_true",
        help="keep the updated histograms after running Combine",
    )
    args = parser.parse_args()

    if (args.mH is None) != (args.mA is None):
        parser.error("--mH and --mA have to be given together")
    if args.all and args.mH is not None:
        parser.error("--all cannot be combined with --mH/--mA")

    header = Info.files["analysis_header"].get(ANALYSIS)
    if header is None:
        parser.error(f"Analysis {ANALYSIS} is not known to CheckMATE")
    try:
        mass_points = read_mass_points(header)
        control_files = find_control_files(results_dir, args.runs)
    except (OSError, ValueError) as error:
        parser.error(str(error))

    if args.mH is not None:
        requested = (args.mH, args.mA)
        if requested not in mass_points:
            parser.error(
                f"Mass point {mass_point_name(*requested)} is not defined in "
                f"{header}"
            )
        mass_points = [requested]
    elif not args.all:
        mass_points = [
            (m_h, m_a) for m_h, m_a in mass_points
            if m_h % DEFAULT_GRID_STEP == 0 and m_a % DEFAULT_GRID_STEP == 0
        ]

    cards_dir = args.datacards / "input"
    cards = cards_dir / CARDS_FILE
    if not cards.is_file():
        parser.error(f"Combine datacard not found: {cards}")
    missing = [
        point for point in mass_points
        if not (
            cards_dir / "histograms" / mass_point_name(*point) / "histograms.root"
        ).is_file()
    ]
    if missing and args.mH is not None:
        parser.error(
            f"No histograms for {mass_point_name(*missing[0])} in "
            f"{cards_dir / 'histograms'}"
        )
    for point in missing:
        print(f"Warning: no histograms for {mass_point_name(*point)}, skipping it")
    mass_points = [point for point in mass_points if point not in missing]

    combine_executable = None
    if not args.prepare_only:
        combine_executable = shutil.which(args.combine_executable)
        if combine_executable is None:
            parser.error(f"Combine executable not found: {args.combine_executable}")

    output_dir = args.output or results_dir / ("combined_" + "_".join(args.runs))
    output_dir.mkdir(parents=True, exist_ok=True)
    shutil.copy2(cards, output_dir / CARDS_FILE)

    print("Combining control regions of:")
    for path in control_files:
        print(f"  {path}")
    combined_control_file = output_dir / COMBINED_CONTROL_FILE
    try:
        write_combined(combined_control_file, combine_files(control_files))
    except (OSError, ValueError) as error:
        parser.error(str(error))
    print(f"Combined control regions written to {combined_control_file}")

    # Results of earlier runs into the same output folder are kept
    limits_file = output_dir / LIMITS_FILE
    rows = read_limits(limits_file)
    failed = []
    for index, (m_h, m_a) in enumerate(mass_points, start=1):
        mass_point = mass_point_name(m_h, m_a)
        print(f"\n[{index}/{len(mass_points)}] {mass_point}", flush=True)
        histogram_file = output_dir / "histograms" / mass_point / "histograms.root"
        histogram_file.parent.mkdir(parents=True, exist_ok=True)
        limits = {}
        try:
            shutil.copy2(
                cards_dir / "histograms" / mass_point / "histograms.root",
                histogram_file,
            )
            update_histograms(m_h, m_a, combined_control_file, histogram_file)
            if args.prepare_only:
                status = "prepared"
            else:
                limits = run_combine(
                    combine_executable, output_dir, m_h, m_a, args.r_max
                )
                status = "ok"
                print(
                    "  " + "  ".join(
                        f"{column}={limits.get(column, '-')}"
                        for column in LIMIT_COLUMNS
                    )
                )
        except (OSError, ValueError, RuntimeError) as error:
            status = "failed"
            failed.append((m_h, m_a))
            print(f"Error for {mass_point}: {error}")
        finally:
            # Also on Ctrl-C or unexpected errors, never leave a modified
            # histogram file behind
            if not args.prepare_only and not args.keep_histograms:
                shutil.rmtree(histogram_file.parent, ignore_errors=True)
        rows[(m_h, m_a)] = (limits, status)
        write_limits(limits_file, rows)

    print(f"\nResults written to {limits_file}")
    if failed:
        print(f"{len(failed)} of {len(mass_points)} mass points failed:")
        for point in failed:
            print(f"  {mass_point_name(*point)}")
        sys.exit(1)


if __name__ == "__main__":
    print("Run this script via bin/combine_cms_2605_13614.py", file=sys.stderr)
    sys.exit(1)
