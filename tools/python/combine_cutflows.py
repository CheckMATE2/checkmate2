#!/usr/bin/env python3
"""Calculate MCEvents-weighted cutflow values from matching files."""

import argparse
import math
import re
from pathlib import Path


MC_EVENTS_RE = re.compile(
	r"^\s*MCEvents\s*:\s*([-+]?(?:\d+(?:\.\d*)?|\.\d+)(?:[eE][-+]?\d+)?)"
)


def read_mc_events(path):
	with path.open(encoding="utf-8", errors="replace") as file:
		for line in file:
			match = MC_EVENTS_RE.match(line)
			if match:
				return float(match.group(1))
	return 0.0


def read_regions(path, wanted_regions):
	values = {}
	with path.open(encoding="utf-8", errors="replace") as file:
		for line in file:
			fields = line.split()
			if not fields or fields[0] not in wanted_regions:
				continue
			try:
				numbers = [float(value) for value in fields[1:]]
			except ValueError:
				continue
			# Each line is: region name, then four or more numeric columns.
			if len(numbers) >= 4:
				values[fields[0]] = (numbers[2], numbers[3])
	return values


def combine(folder, analysis, regions):
	folder = Path(folder)
	files = sorted(
		path for path in folder.iterdir()
		if path.is_file() and analysis in path.name and "cutflow" in path.name
	)
	if not files:
		raise ValueError("No files containing both the analysis and 'cutflow' were found")

	weighted_totals = {region: [0.0, 0.0] for region in regions}
	total_weight = 0.0
	region_set = set(regions)

	for path in files:
		weight = read_mc_events(path)
		total_weight += weight
		file_values = read_regions(path, region_set)
		for region in regions:
			# An absent line contributes zero, while the file's weight is retained.
			column3, column4 = file_values.get(region, (0.0, 0.0))
			weighted_totals[region][0] += weight * column3
			weighted_totals[region][1] += weight * column4

	if total_weight == 0:
		raise ValueError("The matching files have a total MCEvents weight of zero")

	averages = {
		region: (totals[0] / total_weight, math.sqrt(totals[0])/total_weight, totals[1] / total_weight, totals[1] / total_weight / math.sqrt(totals[0]))
		for region, totals in weighted_totals.items()
	}
	return files, averages


def main():
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument("folder", help="Folder containing cutflow files")
	parser.add_argument("analysis", help="Analysis pattern to match in filenames")
	parser.add_argument("regions", nargs="+", help="Cutflow region names to combine")
	args = parser.parse_args()

	try:
		files, averages = combine(args.folder, args.analysis, args.regions)
	except (OSError, ValueError) as error:
		parser.error(str(error))

	with open(Path(args.folder) / f"{args.analysis}_combined_cutflow.txt", "w") as output_file:
		output_file.write("Region\tEfficiency\tError\tNormalized events\tError\n")
		for region, (column3, error3, column4, error4) in averages.items():
			output_file.write(f"{region}\t{column3:.10g}\t{error3:.10g}\t{column4:.10g}\t{error4:.10g}\n")

	print(f"Files combined: {len(files)}")
	print("Region\tEfficiency\tError\tNormalized events\tError")
	for region, (column3, error3, column4, error4) in averages.items():
		print(f"{region}\t{column3:.10g}\t{error3:.10g}\t{column4:.10g}\t{error4:.10g}")


if __name__ == "__main__":
	main()
