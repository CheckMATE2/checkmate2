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
# Years whose histograms are summed in the comparison plots
PLOT_YEAR_GROUPS = (("2016", "2017", "2018"), ("2022",))
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


def read_summed_nominal(root_file, years, channel, name):
    """Return the sum of the nominal histograms of the given years."""
    total = None
    for year in years:
        directory_path = SUB_PATH_TEMPLATE.format(year=year, channel=channel)
        nominal = root_file.Get(f"{directory_path}/nominal")
        if not nominal or not nominal.InheritsFrom("TH1"):
            raise ValueError(
                f"'nominal' histogram not found in {root_file.GetName()}:"
                f"{directory_path}"
            )
        if total is None:
            total = nominal.Clone(name)
            total.SetDirectory(0)
        else:
            if nominal.GetNbinsX() != total.GetNbinsX():
                raise ValueError(
                    f"Binning of {directory_path} differs from the other years"
                )
            total.Add(nominal)
    return total


def plot_histograms(m_h, m_a, original_file, updated_file, output_dir):
    """Compare the original CMS signal histograms with the updated ones.

    For each channel, one plot is made for the sum of 2016-2018 and one for
    2022. Return the paths of the written plots.
    """
    import ROOT

    ROOT.gROOT.SetBatch(True)
    ROOT.gStyle.SetOptStat(0)
    mass_point = mass_point_name(m_h, m_a)
    output_dir = Path(output_dir)
    output_dir.mkdir(parents=True, exist_ok=True)

    files = {}
    try:
        for label, path in (("original", original_file), ("updated", updated_file)):
            files[label] = ROOT.TFile.Open(str(path), "READ")
            if not files[label] or files[label].IsZombie():
                raise OSError(f"Could not open histogram file {path}")

        plots = []
        for control_channel, channel in CHANNELS.items():
            for years in PLOT_YEAR_GROUPS:
                year_label = "-".join((years[0], years[-1])) if len(years) > 1 else years[0]
                tag = f"{mass_point}_{control_channel}_{year_label}"
                original = read_summed_nominal(
                    files["original"], years, channel, f"original_{tag}"
                )
                updated = read_summed_nominal(
                    files["updated"], years, channel, f"updated_{tag}"
                )
                if original.GetNbinsX() != updated.GetNbinsX():
                    raise ValueError(
                        f"Binning of the original and the updated {control_channel} "
                        f"{year_label} histograms differs"
                    )

                canvas = ROOT.TCanvas(f"canvas_{tag}", "", 800, 800)
                top = ROOT.TPad(f"top_{tag}", "", 0, 0.3, 1, 1)
                bottom = ROOT.TPad(f"bottom_{tag}", "", 0, 0, 1, 0.3)
                for pad in (top, bottom):
                    pad.SetLeftMargin(0.13)
                    pad.SetRightMargin(0.05)
                top.SetBottomMargin(0.02)
                bottom.SetTopMargin(0.02)
                bottom.SetBottomMargin(0.3)
                top.Draw()
                bottom.Draw()

                top.cd()
                original.SetLineColor(ROOT.kBlue + 1)
                original.SetMarkerColor(ROOT.kBlue + 1)
                original.SetLineWidth(2)
                updated.SetLineColor(ROOT.kRed + 1)
                updated.SetMarkerColor(ROOT.kRed + 1)
                updated.SetMarkerStyle(20)
                updated.SetLineWidth(2)
                maximum = max(
                    original.GetBinContent(i) + original.GetBinError(i)
                    for i in range(1, original.GetNbinsX() + 1)
                )
                maximum = max(
                    [maximum] + [
                        updated.GetBinContent(i) + updated.GetBinError(i)
                        for i in range(1, updated.GetNbinsX() + 1)
                    ]
                )
                original.SetTitle(f"{mass_point}, {control_channel}, {year_label}")
                original.GetYaxis().SetTitle("Signal events")
                original.GetYaxis().SetTitleSize(0.05)
                original.GetYaxis().SetTitleOffset(1.1)
                original.GetXaxis().SetLabelSize(0)
                original.SetMaximum(1.3 * maximum if maximum > 0 else 1.0)
                original.SetMinimum(0.0)
                original.Draw("E")
                updated.Draw("E1 SAME")
                legend = ROOT.TLegend(0.15, 0.72, 0.5, 0.88)
                legend.SetBorderSize(0)
                legend.SetFillStyle(0)
                legend.AddEntry(original, "CMS", "le")
                legend.AddEntry(updated, "CheckMATE", "lep")
                legend.Draw()

                bottom.cd()
                ratio = updated.Clone(f"ratio_{tag}")
                ratio.SetDirectory(0)
                ratio.Divide(original)
                ratio.SetTitle("")
                ratio.SetMinimum(0.0)
                ratio.SetMaximum(2.0)
                ratio.GetYaxis().SetTitle("CheckMATE / CMS")
                ratio.GetYaxis().SetNdivisions(505)
                ratio.GetYaxis().SetTitleSize(0.1)
                ratio.GetYaxis().SetTitleOffset(0.45)
                ratio.GetYaxis().SetLabelSize(0.08)
                ratio.GetXaxis().SetTitle(original.GetXaxis().GetTitle())
                ratio.GetXaxis().SetTitleSize(0.1)
                ratio.GetXaxis().SetLabelSize(0.08)
                ratio.Draw("E1")
                line = ROOT.TLine(
                    ratio.GetXaxis().GetXmin(), 1.0, ratio.GetXaxis().GetXmax(), 1.0
                )
                line.SetLineStyle(2)
                line.Draw()

                plot_path = output_dir / f"{tag}.pdf"
                canvas.SaveAs(str(plot_path))
                canvas.Close()
                plots.append(plot_path)
    finally:
        for root_file in files.values():
            if root_file:
                root_file.Close()
    return plots


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


# combine call
# ulimit -s unlimited; ulimit -u unlimited  # might be necessary
# combine combined_cards.txt -M AsymptoticLimits --keyword-value MA=140.0 --keyword-value MH=70.0 --strictBounds --rMax 10

# bins Muon
# [0.9, 0.9174299999999207, 0.9267199999998784, 0.9352799999998395, 0.9441499999997991, 0.9554399999997477, 0.9649899999997043, 0.9742899999996619, 0.9865999999996059, 0.9914799999995837, 0.9951799999995578, 1.0]
# bins Electron
# [0.9, 0.9285099999998703, 0.945039999999795, 0.9622399999997168, 0.9795799999996379, 0.9867199999996054, 0.991159999999567, 1.0]
