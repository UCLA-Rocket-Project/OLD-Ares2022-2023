"""Exposes easy management of Ground Systems' Linux services.

Description:
- Provides an easy-to-use script for managing/operating all important Linux
system services being used to support the Ground Systems (Controls + DAQ)
server.

Author:
    Harrison Cassar, 5-5-2023
"""

import sys
import time
import argparse
import subprocess
import logging

from utils import setup_logger


logger = logging.getLogger(__name__)

OPERATIONS = {
    # OP : Description
    'status' : "Show runtime status information about service",
    'start' : "Start (activate) service", 
    'stop' : "Stop (deactivate) service",
    'restart' : "Stop, and then start service. If a service was not yet running already, it will be started",
    'try-restart' : "Stop, and then start service. If a service is not yet running already, this operation will do nothing",
}

SERVICES = {
    # API ID : (Service Filename, Description)
    'adc'               : ("adc.service", "Run the ADC binary to poll, process, and log all data acquired by the ADC"),
    'cctv'              : ("cctv.service", "Performs live-streaming of CCTV camera video output"),
    'grafana'           : ("grafana-server.service", "Runs the Grafana front-end server"),
    'grafana-image'     : ("image.service", "Runs a simple Flask server for images to be displayed on our main Grafana front-end dashboards"),
    'tc'                : ("tc.service", "Run a Python script that processes and logs all data acquired by the Thermocouples"),
    'server-flask'      : ("flask.service", "Runs our main Flask server that facilitates communication between the Pad and Bunker for the Controls and DAQ subsystems"),
    'server-telegraf'   : ("telegraf.service", "Runs Telegraf to mediate the flow of data (not currently actively used)")
}

GROUPS = {
    # GROUP ID : (List of Service API IDs in Group, Description)
    'data'      : (["adc", "cctv", "tc"], "Services that are related to near-device data collection/processing"),
    'server'    : (["grafana", "grafana-image", "server-flask", "server-telegraf"], "Services that are related to the running of the server and its facilitation responsibilities"),
    'all'       : (list(SERVICES.keys()), "Encompasses ALL services")
}


def gen_help_msg_services(services):
    return '; '.join((f"{key} ({val[1]})") for key, val in services.items()) + '.'


def gen_help_msg_groups(groups):
    return '; '.join((f"{key} ({val[1]} Includes: {', '.join(val[0])})") for key, val in groups.items()) + '.'


def gen_help_msg_ops(ops):
    return '; '.join((f"{key} ({val})") for key, val in ops.items()) + '.'


def Operation(op):
    if op.lower() not in OPERATIONS:
        raise argparse.ArgumentTypeError(f"{op} is not a supported operation.")
    return op.lower()


def ServiceId(id):
    if id.lower() not in SERVICES:
        raise argparse.ArgumentTypeError(f"{id} is not a valid API ID for a supported Service.")
    return id.lower()


def GroupId(id):
    if id.lower() not in GROUPS:
        raise argparse.ArgumentTypeError(f"{id} is not a valid ID for a supported Group of Services.")
    return id.lower()


def main():

    parser = argparse.ArgumentParser(
        description="Manage Ground Systems' RPi server's Linux services "
            "empowering all Controls/DAQ subsystem functionalities.")

    parser.add_argument(
        'operation',
        type=Operation,
        metavar='OP',
        help=f"Operation to perform on specified Services. Possible values:\n{gen_help_msg_ops(OPERATIONS)}")
    parser.add_argument(
        'groups',
        type=GroupId,
        metavar='GROUP',
        nargs='*',
        help=f"Service Group IDs of interest to operate on. Possible values:\n{gen_help_msg_groups(GROUPS)}")
    parser.add_argument(
        '-i', '--individual',
        dest='individual_services',
        metavar='ID',
        type=ServiceId,
        nargs='+',
        help=f"Individual Service API IDs of interest to operate on. Possible values:\n{gen_help_msg_services(SERVICES)}")
    parser.add_argument(
        '--log',
        dest='log',
        help="Path to file for logging output (instead of only stdout).")
    parser.add_argument(
        '--log-level',
        dest='log_level',
        help="Provide logging level (i.e. DEBUG, INFO, WARNING, etc.). Default: %(default)s",
        default='INFO')

    args = parser.parse_args()

    operation = args.operation
    groups = args.groups
    individual_services = args.individual_services
    log = args.log
    log_level = args.log_level

    setup_logger(log_level=log_level, file_log=log, logger=logger)

    # Verify some number of services were provided.
    if (not groups and not individual_services):
        raise ValueError("Expected at least one Service to be specified to operate on through the GROUP or INDIVIDUAL options.")

    # Gather all services to operate on.
    services = []
    if individual_services:
        services += individual_services
    if groups:
        for group in groups:
            services += GROUPS[group][0]

    # Run operations on specified services.
    for service in services:
        subprocess.run(["systemctl", operation, SERVICES[service][0]])

        time.sleep(1)


if __name__ == '__main__':
    sys.exit(main())