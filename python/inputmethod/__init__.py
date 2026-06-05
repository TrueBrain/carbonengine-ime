# Copyright © 2014 CCP ehf.

import blue
import sys

sys.modules[__name__] = blue.LoadExtension("_ime")
