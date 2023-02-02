import ctypes
import sys
import os

ECOSYSTEM_ROOT = os.environ["ECOSYSTEM_ROOT"]

lib_path = os.path.join(ECOSYSTEM_ROOT, "simulation", "build", "osx-clang-rel", "src")
wrapper_path = os.path.join(lib_path, "libecosystem_wrapper.dylib")
sys.path.append(lib_path)

ecosystem_lib = ctypes.CDLL(wrapper_path)

ecosystem_lib.create_god(0)

ecosystem_lib.set_initial_organisms(0, ctypes.c_char_p(b"deer"), 10, 10)
ecosystem_lib.set_initial_organisms(0, ctypes.c_char_p(b"deer"), 20, 100)
ecosystem_lib.set_initial_organisms(0, ctypes.c_char_p(b"deer"), 30, 50)

ecosystem_lib.create_world()
ecosystem_lib.run_simulation(100)
