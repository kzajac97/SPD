from Process import *
from Carlier import *

if __name__ == "__main__":
    data = read_data("Data/in50.txt")

    processes = create_processes(data)
    
    print(rpq_maxspan(processes))
    _,b = get_b_task(processes)
    _,a = get_a_task(processes)
    _,c,__ = get_c_task(processes)
    print(a)
    print(b)
    print(c)