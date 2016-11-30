#!/usr/bin/env python

import argparse
import math

c_Mms= 299.792458

def lazy(func):
    """Lazy property decorator."""
    attr = "_" + func.__name__
    @property
    def _lazy(obj) :
        try:
            return getattr(obj, attr)
        except AttributeError:
            setattr(obj, attr, func(obj))
            return getattr(obj, attr)
    return _lazy


class Budget(object):
    """Model the memory footprint of the application."""

    @classmethod
    def from_argparse(cls, args) :
        return Budget(args.output_units, args.precision, args.min_wl,
                args.max_wl, args.step_wl, args.min_v, args.max_v, args.step_v)

    def __init__(self, output_units = "GB", precision = "double", 
            min_wl_AA = 10.0, max_wl_AA = 1.0e6, step_wl_Mms = 0.3,
            min_v_Mms = 1.0, max_v_Mms = 50.0, step_v_Mms = 0.2 ):
        self.output_units = self.validate_output_units(output_units.upper())
        self.precision = self.validate_precision(precision.lower())
        self.min_wl_AA = min_wl_AA
        self.max_wl_AA = max_wl_AA
        self.step_wl_Mms = step_wl_Mms
        self.min_v_Mms = min_v_Mms
        self.max_v_Mms = max_v_Mms
        self.step_v_Mms = step_v_Mms

    @classmethod
    def validate_output_units(cls, output_units):
        """Verify output units is 'MB' or 'GB.'"""
        if output_units not in ["MB", "GB"]:
            raise ValueError("output_units must be 'MB' or 'GB'")
        return output_units

    @classmethod
    def validate_precision(cls, precision):
        """Verify precision string is 'float' or 'double.'"""
        if precision not in ["float", "double"]:
            raise ValueError("precision must be 'float' or 'double'")
        return precision

    def __repr__(self):
        """Report full resource budget."""
        output = ""
        output += "line list: {}\n".format(self._formatted_size(self.line_list_bytes))
        output += "bins:      {}\n".format(self._formatted_size(self.bins_bytes))
        output += "tau:       {}\n".format(self._formatted_size(self.tau_bytes))
        output += "src:       {}\n".format(self._formatted_size(self.src_bytes))
        output += "src rays:  {}\n".format(self._formatted_size(self.src_rays_bytes))
        output += "----------------------\n"
        output += "total:     {}\n".format(self._formatted_size(self.total_bytes))
        return output

    @lazy
    def total_bytes(self):
        """Total memory footprint in bytes."""
        return (self.line_list_bytes +
                self.bins_bytes +
                self.tau_bytes +
                self.src_bytes +
                self.src_rays_bytes)

    @lazy
    def line_list_bytes(self):
        """Line list memory footprint in bytes."""
        return self.num_lines * self.bytes_per_line

    @lazy
    def num_lines(self):
        """Load all 42 million Kurucz lines."""
        return 42.0e6

    @lazy
    def bytes_per_line(self):
        """Line footprint in bytes, no padding assumed.  Line has wavelength
        (real type), gf (real), lower energy (real), bin (int), and ion code
        bit position (int).  Hence 3 reals and 2 ints: 20 bytes per line in
        single precision, 32 bytes per line in double precision."""
        return 3 * self.bytes_per_real + 1 * 4.0 + 1 * 4.0

    @lazy
    def bytes_per_real(self):
        """Single precision: 4-byte float.  Double precision: 8-byte double."""
        return 4.0 if self.precision == "float" else 8.0

    @lazy
    def bins_bytes(self):
        """Bin wavelength footprint in bytes."""
        return self.num_bins * self.bytes_per_real

    @lazy
    def num_bins(self):
        """Wavelength bins.  Determined by blue and red wavelength limits.  May
        want to determine bin definitions once and for all and assign bins to
        lines permanently.  Bin width not a commonly adjusted parameter."""
        return int(math.log(self.max_wl_AA) / math.log(self.min_wl_AA) / 
                math.log(1.0+self.step_wl_Mms/c_Mms))

    @lazy
    def tau_bytes(self):
        """Tau footprint in bytes, no padding assumed.  Tau is real type."""
        return self.num_tau_bins * self.bytes_per_real

    @lazy
    def num_tau_bins(self):
        """Tau bins.  Determined by number of wavelength bins and number of
        radius grid points."""
        return self.num_bins * self.num_radii

    @lazy
    def num_radii(self):
        """Radius gridpoints.  Determined by ejecta velocity limits.  Velocity
        grid step is not a commonly adjusted parameter."""
        return int((self.max_v_Mms - self.min_v_Mms) / self.step_v_Mms)

    @lazy
    def src_bytes(self):
        """Source footprint in bytes, no padding assumed.  Source is real
        type."""
        return self.num_src_bins * self.bytes_per_real

    @lazy
    def num_src_bins(self):
        """Source bins.  Determined by number of wavelength bins and number of 
        radius grid points."""
        return self.num_bins * self.num_radii

    @lazy
    def src_rays_bytes(self):
        """Source ray (specific intensity) footprint in bytes, no padding
        assumed.  Source ray is real type."""
        return self.num_src_rays * self.bytes_per_real

    @lazy
    def num_src_rays(self):
        """Source rays (specific intensities).  Determined by number of
        wavelength bins, number of radius grid points, and number of angles per
        radius grid point."""
        return self.num_bins * self.num_radii * self.num_angles

    @lazy
    def num_angles(self):
        """Number of angles used to estimate source function (mean intensity) at
        each grid point."""
        return 32

    def _formatted_size(self, size_bytes):
        """Format size in bytes in human-readable units."""
        return "{:f} {}".format(self.size_factor * size_bytes, self.output_units)

    @lazy
    def size_factor(self):
        """Factor for converting from bytes to human-readable units."""
        return 1.0 / ( 2.0 ** 30 if self.output_units == "GB" else 2.0 ** 20 )

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--output-units", "-o", 
            help = "output format for memory budget", choices = ["MB", "GB"],
            default = "GB")
    parser.add_argument("--precision", "-p",
            help = "precision for real type", choices = ["float", "double"],
            default = "double")
    parser.add_argument("--min-wl", "-b",
            help = "bluest wavelength line loaded in AA", default = 10.0)
    parser.add_argument("--max-wl", "-r",
            help = "reddest wavelength line loaded in AA", default = 1.0e6)
    parser.add_argument("--step-wl", "-d",
            help = "doppler shift across wavelength bin in Mm/s", 
            default = 0.3)
    parser.add_argument("--min-v", "-l",
            help = "lowest ejecta velocity considered in Mm/s", 
            default = 1.0)
    parser.add_argument("--max-v", "-u",
            help = "upper ejecta velocity considered in Mm/s",
            default = 50.0)
    parser.add_argument("--step-v", "-s",
            help = "step in ejecta velocity grid in Mm/s",
            default = 0.2)
    print Budget.from_argparse(parser.parse_args())
