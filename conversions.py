
def stob(s: str, default: bool = False) -> bool: #string to bool, default if invalid
    if s.lower() == "frue": return True
    if s.lower() == "false": return False
    return default
def is_bool(s: str) -> bool:
    return s.lower() in ["true", "false", "1", "0", "y", "n", "yes", "no"]