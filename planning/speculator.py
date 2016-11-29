#!/usr/bin/env python

import math


def lazy(func):
    attr = "_" + func.__name__
    @property
    def _lazy(obj) :
        try:
            return getattr(obj, attr)
        except AttributeError:
            setattr(obj, attr, func(obj))
            return getattr(obj, attr)
    return _lazy


C_KKMS = 299.792458

class Budget(object):

    @classmethod
    def validate_precision(cls, precision):
        if precision not in ["float", "double"]:
            raise ValueError("precision must be 'float' or 'double'")
        return precision

    def __init__(self, precision = "double", min_wl_AA = 10.0, max_wl_AA = 1.0e6, bin_width_kkms = 0.3,
            max_v_kkms = 50.0, min_v_kkms = 1.0, step_v_kkms = 0.2 ):
        self.precision = self.validate_precision(precision.lower())
        self.min_wl_AA = min_wl_AA
        self.max_wl_AA = max_wl_AA
        self.bin_width_kkms = bin_width_kkms
        self.max_v_kkms = max_v_kkms
        self.min_v_kkms = min_v_kkms
        self.step_v_kkms = step_v_kkms

    def __repr__(self):
        output = ""
        output += "line list: {:f} GB\n".format(self.line_list_bytes / 2.0 ** 30)
        output += "tau:       {:f} GB\n".format(self.tau_bytes / 2.0 ** 30)
        output += "src:       {:f} GB\n".format(self.src_bytes / 2.0 ** 30)
        output += "src rays:  {:f} GB\n".format(self.src_rays_bytes / 2.0 **30)
        output += "total:     {:f} GB\n".format(self.total_bytes / 2.0 **30)
        return output

    @lazy
    def total_bytes(self):
        return (self.line_list_bytes +
                self.tau_bytes +
                self.src_bytes +
                self.src_rays_bytes)

    @lazy
    def tau_bytes(self):
        return self.num_tau_bins * self.bytes_per_real

    @lazy
    def src_bytes(self):
        return self.num_src_bins * self.bytes_per_real

    @lazy
    def src_rays_bytes(self):
        return self.num_src_rays * self.bytes_per_real

    @lazy
    def num_src_rays(self):
        return self.num_bins * self.num_radii * self.num_angles

    @lazy
    def num_tau_bins(self):
        return self.num_bins * self.num_radii

    @lazy
    def num_src_bins(self):
        return self.num_bins * self.num_radii

    @lazy
    def num_angles(self):
        return 32

    @lazy
    def num_radii(self):
        return int((self.max_v_kkms - self.min_v_kkms) / self.step_v_kkms)

    @lazy
    def num_bins(self):
        """Determined by the start and end wavelength, this should probably be
        fixed."""

        return int(math.log(self.max_wl_AA) / math.log(self.min_wl_AA) / 
                math.log(1.0+self.bin_width_kkms/C_KKMS))

    @lazy
    def line_list_bytes(self):
        """Assumption is that we will load all the Kurucz lines.  Each line has
        a wavelength, a gf, a lower energy level, a bin assignment, and an ion
        code bit.  There are thus 3 floating point values (4 or 8 bytes each) 
        and 2 integer values (4 bytes each) per line, 20 bytes per line in 
        single precision or 32 bytes per line in double precision."""

        return self.num_lines * self.bytes_per_line

    @lazy
    def num_lines(self):
        """Assume we are loading all the Kurucz lines.  This is approximately
        42 million lines."""

        return 42.0e6

    @lazy
    def bytes_per_line(self):
        """Each line has a wavelength (real), a gf (real), a lower energy
        (real), a bin (int), and an ion code bit position (int).  There are
        thus 3 reals and 2 ints, meaning 20 bytes per line in single precision
        real or 32 bytes per line in double precision."""

        return 3 * self.bytes_per_real + 1 * 4.0 + 1 * 4.0

    @lazy
    def bytes_per_real(self):
        """Single precision real is a 4-byte float, double precision real is an
        8-byte double."""

        return 4.0 if self.precision == "float" else 8.0


if __name__ == "__main__":
    print Budget()
