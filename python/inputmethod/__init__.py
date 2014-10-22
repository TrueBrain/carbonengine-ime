try:
    import blue
    import _ime
except ImportError:
    import binbootstrapper
    binbootstrapper.update_binaries(__file__, binbootstrapper.DLL_BLUE, binbootstrapper.DLL_IME)
    import blue
    import _ime

Ime = _ime.Ime

__all__ = ['Ime']