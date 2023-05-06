"""Common util functions for URP's various GSW scripts.

Author:
    Harrison Cassar, 5-4-2023
"""

import sys
import os
import logging


logger = logging.getLogger(__name__)


def setup_logger(log_level='INFO', file_log=None, logger=None):
    """Configures logging module with logging level, as well as logging to
    stdout (and file, if desired, at a specified logging directory)."""

    levels = {
        'CRITICAL': logging.CRITICAL,
        'ERROR': logging.ERROR,
        'WARNING': logging.WARNING,
        'INFO': logging.INFO,
        'DEBUG': logging.DEBUG
    }
    level = levels.get(log_level.upper())
    if level is None:
        raise ValueError(f"User-specified log level '{log_level}' invalid; must be one of: {' | '.join(levels.keys())}")

    if file_log:
        # check logging file doesnt already exist as a directory
        if os.path.exists(file_log) and os.path.isdir(file_log):
            raise Exception(f"Specified logging file '{file_log}' already exists as directory.")
        os.makedirs(os.path.dirname(file_log), exist_ok=True)

        console = logging.StreamHandler()
        console_formatter = logging.Formatter("%(asctime)s - %(levelname)s - %(message)s")
        console.setFormatter(console_formatter)

        logging.basicConfig(
            filename=file_log,
            filemode='w+',
            format='%(asctime)s - %(levelname)s - %(message)s',
            level=level)

        logger.addHandler(console)

    else:
        logging.basicConfig(
            stream=sys.stderr,
            format='%(asctime)s - %(levelname)s - %(message)s',
            level=level)