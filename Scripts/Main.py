import sys

from Process import *
from Carlier import *
from Schrage import *

if __name__ == "__main__":
    sys.setrecursionlimit(10000)
    # files = ["Data/data000.txt","Data/data001.txt","Data/data002.txt",
    #                                     "Data/data003.txt","Data/data004.txt","Data/data005.txt",
    #                                     "Data/data006.txt","Data/data007.txt","Data/data008.txt"]

    #files = ["Data/in50.txt","Data/in100.txt","Data/in200.txt"]
    files = ["Data/in50.txt"]

    for f in files:
        data = read_data(f)
        processes = create_processes(data)
        result = carlier(processes)
        print(rpq_maxspan(result))