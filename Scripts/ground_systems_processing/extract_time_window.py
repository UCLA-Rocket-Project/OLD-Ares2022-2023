import sys
import csv
import logging
import argparse

logger = logging.getLogger(__name__)

def main():
    parser = argparse.ArgumentParser(description="Extracts a subset of the CSV based on user-specified filters on time (column 1).")
    parser.add_argument(
        '-i', '--input-data-csv',
        dest='in_file',
        type=argparse.FileType('r'),
        help="Path to desired data CSV to be extracted from.",
        required=True)
    parser.add_argument(
        '-o', '--output',
        dest='out_file',
        type=argparse.FileType('w'),
        help="File name for outputted CSV extraction. Default: stdout",
        default=sys.stdout)
    parser.add_argument(
        '-s', '--start-time',
        dest='start_time',
        type=int,
        help="Start time (UTC milliseconds) for extracted CSV records. Default: 0",
        default=0)
    parser.add_argument(
        '-e', '--end-time',
        dest='end_time',
        type=int,
        help="End time (UTC milliseconds) for extracted CSV records. Default: None",
        default=None)
    parser.add_argument(
        '-t', '--time-column',
        dest='time_column',
        type=int,
        help="Column index for inputted CSV that contains timestamp. Default: 0",
        default=0)

    args = parser.parse_args()

    # Extract args for convenience.
    in_file = args.in_file
    out_file = args.out_file
    start_time = args.start_time
    end_time = args.end_time
    time_column = args.time_column

    # Filter rows based on specified time value.
    in_reader = csv.reader(in_file)
    out_writer = csv.writer(out_file)

    for row in in_reader:
        if int(row[time_column]) >= start_time and (end_time is None or int(row[time_column]) <= end_time):
            out_writer.writerow(row)

if __name__ == '__main__':
    main()