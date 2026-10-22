#!/usr/bin/env python3
"""Combine control-region event counts from matching files."""

import argparse
import math
import re
from pathlib import Path


MC_EVENTS_RE = re.compile(
	r"^\s*@?MCEvents\s*:\s*([-+]?(?:\d+(?:\.\d*)?|\.\d+)(?:[eE][-+]?\d+)?)"
)
CONTROL_HEADER = ("CR", "Sum_W", "Sum_W2", "Acc", "N_Norm")


def read_mc_events(path):
	with path.open(encoding="utf-8", errors="replace") as file:
		for line in file:
			match = MC_EVENTS_RE.match(line)
			if match:
				mc_events = float(match.group(1))
				if not math.isfinite(mc_events) or mc_events <= 0:
					raise ValueError(
						f"{path}: MCEvents must be finite and greater than zero"
					)
				return mc_events
	raise ValueError(f"{path}: no MCEvents value found")


def read_control_regions(path):
	values = {}
	in_control_region_data = False
	with path.open(encoding="utf-8", errors="replace") as file:
		for line_number, line in enumerate(file, start=1):
			fields = line.split()
			if not in_control_region_data:
				if tuple(fields[:5]) == CONTROL_HEADER:
					in_control_region_data = True
				continue

			if not fields:
				continue
			if len(fields) != 5:
				raise ValueError(
					f"{path}:{line_number}: expected a control region and four numbers"
				)
			try:
				acceptance = float(fields[3])
				norm_events = float(fields[4])
			except ValueError as error:
				raise ValueError(
					f"{path}:{line_number}: invalid acceptance or N_Norm value"
				) from error
			if not math.isfinite(acceptance) or acceptance < 0:
				raise ValueError(
					f"{path}:{line_number}: acceptance must be finite and non-negative"
				)
			if not math.isfinite(norm_events):
				raise ValueError(
					f"{path}:{line_number}: N_Norm must be finite"
				)
			if norm_events < 0:
				raise ValueError(
					f"{path}:{line_number}: N_Norm must not be negative"
				)
			values[fields[0]] = (acceptance, norm_events)

	if not in_control_region_data:
		raise ValueError(f"{path}: control-region header not found")
	if not values:
		raise ValueError(f"{path}: no control regions found after the header")
	return values


def combine_files(files):
	"""Sum control-region yields over files, adding errors in quadrature."""
	files = list(files)
	if not files:
		raise ValueError("No control-region files given")

	first_file_values = read_control_regions(files[0])
	regions = list(first_file_values)
	totals = {region: [0.0, 0.0] for region in regions}

	for index, path in enumerate(files):
		mc_events = read_mc_events(path)
		file_regions = (
			first_file_values if index == 0 else read_control_regions(path)
		)
		for region in regions:
			acceptance, norm_events = file_regions.get(region, (0.0, 0.0))
			totals[region][0] += norm_events
			if norm_events and acceptance == 0:
				raise ValueError(
					f"{path}: control region {region!r} has events but zero acceptance"
				)
			if acceptance:
				error = norm_events / math.sqrt(acceptance * mc_events)
				totals[region][1] += error * error

	return {
		region: (events, math.sqrt(variance))
		for region, (events, variance) in totals.items()
	}


def combine(folder, analysis):
	folder = Path(folder)
	suffix = f"_{analysis}_control.dat"
	files = sorted(
		path for path in folder.iterdir()
		if path.is_file() and path.name.endswith(suffix)
	)
	if not files:
		raise ValueError(f"No files ending in {suffix!r} were found")
	return files, combine_files(files)


def write_combined(path, results):
	with Path(path).open("w", encoding="utf-8") as output_file:
		output_file.write("Control region\tEvents\tError\n")
		for region, (events, error) in results.items():
			output_file.write(f"{region}\t{events:.10g}\t{error:.10g}\n")


def main():
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument("folder", help="Folder containing control-region files")
	parser.add_argument("analysis", help="Analysis tag in the control-file names")
	args = parser.parse_args()

	try:
		files, results = combine(args.folder, args.analysis)
	except (OSError, ValueError) as error:
		parser.error(str(error))

	output_path = Path(args.folder) / f"{args.analysis}_combined_control.txt"
	write_combined(output_path, results)

	print(f"Files combined: {len(files)}")
	print("Control region\tEvents\tError")
	for region, (events, error) in results.items():
		print(f"{region}\t{events:.10g}\t{error:.10g}")


if __name__ == "__main__":
	main()
