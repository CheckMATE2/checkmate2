#!/usr/bin/env python3
"""Fill signal histograms from combined control-region yields."""

import argparse
import math
import re
from pathlib import Path


YEARS = ("2016", "2017", "2018", "2022")
CHANNELS = {"EE": "Electron", "MM": "Muon"}
FILE_PATH_TEMPLATE = "histograms/{mass_point}/histograms.root"
CONTROL_FILE = "results/combined_files/cms_2605_13614_combined_control.txt"
SUB_PATH_TEMPLATE = "{year}/SR/{channel}/idm"
CONTROL_HEADER = ("Control", "region", "Events", "Error")
CONTROL_REGION_SUFFIX_RE = re.compile(
    r"^(?P<year>\d{4})_(?P<channel>EE|MM)_bin_(?P<bin>\d+)$"
)


def mass_point_name(m_h, m_a):
    return f"mH{m_h:.1f}_mA{m_a:.1f}"


def read_control_regions(path, mass_point):
    """Read control-region yields and errors for one mass point."""
    path = Path(path)
    region_values = {}
    prefix = f"{mass_point}_"

    with path.open(encoding="utf-8") as control_file:
        header = tuple(control_file.readline().split())
        if header != CONTROL_HEADER:
            raise ValueError(
                f"{path}: expected header "
                f"'Control region Events Error', got {' '.join(header)!r}"
            )

        for line_number, line in enumerate(control_file, start=2):
            fields = line.split()
            if not fields:
                continue
            if len(fields) != 3:
                raise ValueError(
                    f"{path}:{line_number}: expected a region, events, and error"
                )

            region_name = fields[0]
            if not region_name.startswith(prefix):
                continue

            suffix_match = CONTROL_REGION_SUFFIX_RE.fullmatch(
                region_name[len(prefix):]
            )
            if not suffix_match:
                raise ValueError(
                    f"{path}:{line_number}: invalid region name {region_name!r}"
                )

            year = suffix_match.group("year")
            channel = suffix_match.group("channel")
            bin_index = int(suffix_match.group("bin"))
            if year not in YEARS:
                raise ValueError(
                    f"{path}:{line_number}: unsupported year {year!r}"
                )

            try:
                events, error = map(float, fields[1:])
            except ValueError as exception:
                raise ValueError(
                    f"{path}:{line_number}: events and error must be numbers"
                ) from exception
            if (
                not math.isfinite(events)
                or events < 0
                or not math.isfinite(error)
                or error < 0
            ):
                raise ValueError(
                    f"{path}:{line_number}: events and error must be finite "
                    "and non-negative"
                )

            key = (year, channel, bin_index)
            if key in region_values:
                raise ValueError(
                    f"{path}:{line_number}: duplicate control region "
                    f"{region_name!r}"
                )
            region_values[key] = (events, error)

    if not region_values:
        raise ValueError(f"{path}: no control regions found for {mass_point}")
    return region_values


def update_histograms(
    m_h,
    m_a,
    control_file=CONTROL_FILE,
    histogram_file=None,
):
    import ROOT

    mass_point = mass_point_name(m_h, m_a)
    histogram_path = Path(histogram_file) if histogram_file else Path(
        FILE_PATH_TEMPLATE.format(mass_point=mass_point)
    )
    control_values = read_control_regions(control_file, mass_point)

    root_file = ROOT.TFile.Open(str(histogram_path), "UPDATE")
    if not root_file or root_file.IsZombie():
        raise OSError(f"Could not open histogram file {histogram_path}")

    try:
        histograms = []
        for year in YEARS:
            for control_channel, channel in CHANNELS.items():
                directory_path = SUB_PATH_TEMPLATE.format(
                    year=year, channel=channel
                )
                current_dir = root_file.GetDirectory(directory_path)
                if not current_dir:
                    raise ValueError(
                        f"Histogram directory not found: {directory_path}"
                    )

                nominal = current_dir.Get("nominal")
                if not nominal or not nominal.InheritsFrom("TH1"):
                    raise ValueError(
                        f"'nominal' histogram not found in {directory_path}"
                    )
                if nominal.GetDimension() != 1:
                    raise ValueError(
                        f"'nominal' histogram in {directory_path} is not 1D"
                    )

                n_bins = nominal.GetNbinsX()
                expected_bins = set(range(n_bins))
                actual_bins = {
                    bin_index
                    for (row_year, row_channel, bin_index) in control_values
                    if row_year == year and row_channel == control_channel
                }
                if actual_bins != expected_bins:
                    raise ValueError(
                        f"Control-region bins for {year} {control_channel} "
                        f"do not match {directory_path}: expected "
                        f"{sorted(expected_bins)}, got {sorted(actual_bins)}"
                    )

                systematic_histograms = {}
                for key in current_dir.GetListOfKeys():
                    obj = key.ReadObj()
                    if (
                        key.GetName() != "nominal"
                        and obj.InheritsFrom("TH1")
                    ):
                        systematic_histograms[key.GetName()] = obj

                for name, systematic in systematic_histograms.items():
                    if (
                        systematic.GetDimension() != 1
                        or systematic.GetNbinsX() != n_bins
                    ):
                        raise ValueError(
                            f"Systematic histogram {directory_path}/{name} "
                            "does not match the nominal histogram binning"
                        )

                histograms.append(
                    (
                        directory_path,
                        current_dir,
                        nominal,
                        systematic_histograms,
                        year,
                        control_channel,
                    )
                )

        for (
            directory_path,
            current_dir,
            nominal,
            systematic_histograms,
            year,
            channel,
        ) in histograms:
            if not current_dir.cd():
                raise OSError(
                    f"Could not select histogram directory {directory_path}"
                )

            n_bins = nominal.GetNbinsX()
            for root_bin in range(n_bins + 2):
                if root_bin == 0 or root_bin == n_bins + 1:
                    events, error = 0.0, 0.0
                else:
                    events, error = control_values[(year, channel, root_bin - 1)]
                nominal.SetBinContent(root_bin, events)
                nominal.SetBinError(root_bin, error)
            if nominal.Write("nominal", ROOT.TObject.kOverwrite) <= 0:
                raise OSError(f"Failed to write nominal histogram in {directory_path}")

            for name, systematic in systematic_histograms.items():
                for root_bin in range(n_bins + 2):
                    systematic.SetBinContent(
                        root_bin, nominal.GetBinContent(root_bin)
                    )
                    systematic.SetBinError(
                        root_bin, nominal.GetBinError(root_bin)
                    )
                if systematic.Write(name, ROOT.TObject.kOverwrite) <= 0:
                    raise OSError(
                        f"Failed to write histogram {directory_path}/{name}"
                    )

            print(
                f"Updated {directory_path}: nominal and "
                f"{len(systematic_histograms)} systematic histograms"
            )
    finally:
        root_file.Close()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("mH", type=float, help="Higgs mass")
    parser.add_argument("mA", type=float, help="pseudoscalar mass")
    parser.add_argument(
        "--control-file",
        default=CONTROL_FILE,
        help=f"combined control-region file (default: {CONTROL_FILE})",
    )
    parser.add_argument(
        "--histogram-file",
        help="ROOT file to update (defaults to histograms/<mass-point>/histograms.root)",
    )
    args = parser.parse_args()

    try:
        update_histograms(
            args.mH,
            args.mA,
            args.control_file,
            args.histogram_file,
        )
    except (OSError, ValueError) as error:
        parser.error(str(error))


if __name__ == "__main__":
    main()
